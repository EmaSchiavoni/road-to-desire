#include "LevelHUD.h"
#include "Constants.h"

using namespace std;
using namespace rtd;
using namespace sf;

LevelHUD::LevelHUD(int max_lives)
{
	m_fullHeartTexture.loadFromFile("Textures/Statics/full_heart.png");
	m_emptyHeartTexture.loadFromFile("Textures/Statics/empty_heart.png");
	m_max_lives = max_lives;
	m_lives = m_max_lives;
	prepareHeartsHUD();
	prepareCoinsHUD();
	prepareTimer();
}

void LevelHUD::prepareHeartsHUD()
{
	m_fullHeart.setTexture(m_fullHeartTexture);
	m_emptyHeart.setTexture(m_emptyHeartTexture);
	rescaleIcon(m_fullHeart);
	rescaleIcon(m_emptyHeart);

	m_heartsElement.setHeight(m_fullHeart.getGlobalBounds().height + m_heartsStartPos.y * 2);
	m_heartsElement.setWidth(m_heartsStartPos.x + (m_fullHeart.getGlobalBounds().width + m_space_between_hearts) * m_max_lives);
	setHUDElement(m_heartsElement);
}

void LevelHUD::prepareCoinsHUD()
{
	m_collectables[Collectable::Type::Coin] = CollectableInHUD();
	auto& coinData = m_collectables[Collectable::Type::Coin];

	if (!coinData.texture.loadFromFile("Textures/Collectables/" + toString(Collectable::Type::Coin) + ".png"))
		throw runtime_error("Ocurrió un error al intentar cargar la textura Textures/Collectables/" + toString(Collectable::Type::Coin) + ".png");
	
	coinData.sprite.setTexture(coinData.texture);
	coinData.sprite.setOrigin(coinData.sprite.getLocalBounds().width, 0);
	coinData.sprite.setPosition(WIN_WIDTH - 8, m_coin_top);
	rescaleIcon(coinData.sprite);
	setSprite(coinData.sprite);

	m_coinsText.setFont(*m_font);
	m_coinsText.setCharacterSize(33);
	m_coinsText.setFillColor({ 255,255,255 });
	m_coinsText.setString("0");
	m_coinsText.setScale(.5, .5);
	m_coinsText.setOrigin(m_coinsText.getLocalBounds().width, 0);
	m_coinsText.setPosition( coinData.sprite.getPosition().x - coinData.sprite.getGlobalBounds().width - m_space_between_coin, m_coin_top - 3);
	setText(m_coinsText);

	auto heartsElemBounds = m_heartsElement.getGlobalBounds();
	m_coinsElement.invert(true);
	m_coinsElement.setHeight( heartsElemBounds.height );
	m_coinsElement.setWidth( heartsElemBounds.width );
	m_coinsElement.setPosition( WIN_WIDTH, 0 );
	setHUDElement(m_coinsElement);
}

void LevelHUD::prepareTimer()
{
	m_timerText.setFont(*m_font);
	m_timerText.setCharacterSize(33);
	m_timerText.setFillColor({ 255,255,255 });
	m_timerText.setString("0");
	m_timerText.setScale(.5, .5);
	m_timerText.setOrigin(m_timerText.getLocalBounds().width/2, 0);
	m_timerText.setPosition(WIN_WIDTH/2, m_coin_top - 3);
}

void LevelHUD::setLives(int current_lives)
{
	m_lives = (current_lives < m_max_lives)? current_lives : m_max_lives;
}

void LevelHUD::setCoins()
{
	m_coinsText.setString( to_string(m_collectables[Collectable::Type::Coin].count) );
	m_coinsText.setOrigin(m_coinsText.getLocalBounds().width, 0);
}

void LevelHUD::setTimer(float time_as_seconds)
{
	int seconds = (int)floor(time_as_seconds) % 60;
	int minutes = ((int)floor(time_as_seconds) - seconds) / 60;
	string sec_string = ((seconds < 10) ? "0" : "") + to_string(seconds);
	string min_string = (minutes > 60)? "+60" : (((minutes < 10) ? "0" : "") + to_string(minutes));

	m_timerText.setString(min_string + ":" + sec_string);
	m_timerText.setOrigin(m_timerText.getLocalBounds().width/2, 0);
}

void LevelHUD::setCollectable(rtd::Collectable::Type type)
{
	if (!m_collectables.count(type)) {
		m_collectables[type] = CollectableInHUD();
		m_collectables[type].texture.setSmooth(true);

		if (!m_collectables[type].texture.loadFromFile("Textures/Collectables/" + toString(type) + ".png"))
			throw runtime_error("Ocurrió un error al intentar cargar la textura Textures/Collectables/" + toString(type) + ".png");
		
		m_collectables[type].sprite.setTexture(m_collectables[type].texture);
		m_collectables[type].sprite.setOrigin(0, m_collectables[type].sprite.getLocalBounds().height / 2);
		rescaleIcon(m_collectables[type].sprite);
	}

	m_collectables[type].count++;

	if (type == Collectable::Type::Coin)
		setCoins();
}

void LevelHUD::draw(RenderWindow& window)
{
	HUD::draw(window);
	drawHearts(window);
	drawCollectables(window);
	drawTimer(window);
}

void LevelHUD::drawHearts(RenderWindow& window)
{
	Vector2f currentPos = m_heartsStartPos;
	float heartWidth = m_fullHeart.getGlobalBounds().width;

	for (int i = 0; i < m_max_lives; ++i) {
		Sprite& sprite = (i < m_lives) ? m_fullHeart : m_emptyHeart;
		sprite.setPosition(currentPos);
		window.draw(sprite);
		currentPos.x += (heartWidth + m_space_between_hearts);
	}
}

void LevelHUD::drawCollectables(RenderWindow& window)
{
	float collecteds_bar_width = 0;
	float collecteds_bar_x = 0;
	float current_collected_x = 0;

	for (auto collected = m_collectables.begin(); collected != m_collectables.end(); ++collected)
		if (collected->first != Collectable::Type::Coin)
			collecteds_bar_width += collected->second.sprite.getGlobalBounds().width + m_space_between_collecteds;

	collecteds_bar_width -= m_space_between_collecteds; //Restar el espacio despues del ultimo
	collecteds_bar_x = current_collected_x = WIN_WIDTH / 2 - collecteds_bar_width / 2;

	for (auto collected = m_collectables.begin(); collected != m_collectables.end(); ++collected)
	{
		if (collected->first == Collectable::Type::Coin)
			continue;

		auto& sprite = collected->second.sprite;

		sprite.setPosition(current_collected_x, m_collecteds_top);
		window.draw(sprite);

		current_collected_x += sprite.getGlobalBounds().width + m_space_between_collecteds;
	}
}

void LevelHUD::drawTimer(RenderWindow& window)
{
	auto color = m_timerText.getFillColor();
	auto pos = m_timerText.getPosition();

	//Dibujar sombra
	m_timerText.setFillColor({0,0,0,125});
	m_timerText.setPosition(pos.x + 1, pos.y + 2);
	window.draw(m_timerText);

	m_timerText.setFillColor(color);
	m_timerText.setPosition(pos);
	window.draw(m_timerText);
}

void LevelHUD::rescaleIcon(Sprite& sprite)
{
	float new_height = 15;
	float new_scale = new_height / sprite.getGlobalBounds().height;
	sprite.setScale(new_scale, new_scale);
}