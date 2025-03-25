#include "MapHUD.h"
#include "Constants.h"
#include "Profile.h"

using namespace std;
using namespace rtd;
using namespace sf;

extern Profile currentProfile;

MapHUD::MapHUD() : HUD()
{
	prepareDragonBallsHUD();
	preparePointsHUD();
}

void MapHUD::prepareDragonBallsHUD()
{
	for (int i = 0; i < 7; i++) {
		m_dragonBallCircles[i] = CircleShape(8, 25);
		m_dragonBallCircles[i].setFillColor({250, 152, 66, 10});
		m_dragonBallCircles[i].setOutlineColor({ 250, 152, 66 });
		m_dragonBallCircles[i].setOutlineThickness(1);
		m_dragonBallCircles[i].setPosition(
			m_ballsStartPos.x + i * m_dragonBallCircles->getGlobalBounds().width + m_space_between_balls * i,
			m_ball_top
		);
		m_dragonBallSprites[i].setPosition(
			m_ballsStartPos.x + i * m_dragonBallCircles->getGlobalBounds().width + m_space_between_balls * i-1,
			m_ball_top-1
		);
	}
	m_dragonBallsElement.setWidth(m_dragonBallCircles[0].getGlobalBounds().width * 7 + m_space_between_balls * 7);
	m_dragonBallsElement.setHeight(m_dragonBallCircles[0].getGlobalBounds().height + m_ball_top*2);
	setHUDElement(m_dragonBallsElement);
}

void MapHUD::preparePointsHUD()
{
	for (int i = 0; i < 2; ++i) {
		Text& text = (i)? m_pointsNumberText : m_pointsLettersText;
		text.setFont(*m_font);
		text.setCharacterSize(40);
		text.setFillColor({ 255,255,255 });
		text.setString("0");
		text.setScale(.5, .5);
		text.setOrigin(text.getLocalBounds().width, text.getLocalBounds().height);
		setText(m_pointsNumberText);
	}
	m_pointsLettersText.setString("PTS");
	m_pointsLettersText.scale(.5, .5);
	m_pointsLettersText.setPosition(WIN_WIDTH - 25, m_points_top);
	m_pointsNumberText.setPosition(m_pointsLettersText.getGlobalBounds().left - 5, m_points_top);

	auto m_dragonBallsElementBounds = m_dragonBallsElement.getGlobalBounds();
	m_pointsElement.invert(true);
	m_pointsElement.setHeight(m_dragonBallsElementBounds.height);
	m_pointsElement.setWidth(m_dragonBallsElementBounds.width);
	m_pointsElement.setPosition(WIN_WIDTH, 0);
	setHUDElement(m_pointsElement);
}

void MapHUD::setPoints()
{
	m_pointsNumberText.setString( to_string( (int)round(currentProfile.getTotalScore()) ) );
	m_pointsNumberText.setOrigin(m_pointsNumberText.getLocalBounds().width, m_pointsNumberText.getLocalBounds().height);
}

void MapHUD::setDragonBalls()
{
	auto dragonBalls = currentProfile.getDragonBalls();
	for (int i : dragonBalls) {
		i -= 1;
		if(!m_dragonBallTextures[i].loadFromFile("Textures/Collectables/DragonBall" + to_string(i+1) + ".png"))
			throw runtime_error("Error al intentar leer el archivo 'Textures/Collectables/DragonBall" + to_string(i+1) + ".png'");
		m_dragonBallTextures[i].setSmooth(true);
		m_dragonBallSprites[i].setTexture(m_dragonBallTextures[i]);
		m_dragonBallSprites[i].setScale(
			m_dragonBallCircles->getGlobalBounds().width / m_dragonBallTextures[i].getSize().x,
			m_dragonBallCircles->getGlobalBounds().width / m_dragonBallTextures[i].getSize().x
		);
	}
}

void MapHUD::draw(RenderWindow& window)
{
	HUD::draw(window);
	drawPoints(window);
	drawDragonBalls(window);
}

void MapHUD::drawPoints(RenderWindow& window)
{
	window.draw(m_pointsLettersText);
	window.draw(m_pointsNumberText);
}

void MapHUD::drawDragonBalls(RenderWindow& window)
{
	for (int i = 0; i < 7; ++i){
		window.draw(m_dragonBallCircles[i]);
		window.draw(m_dragonBallSprites[i]);
	}
}

void MapHUD::rescaleIcon(Sprite& sprite)
{
	float new_height = 15;
	float new_scale = new_height / sprite.getGlobalBounds().height;
	sprite.setScale(new_scale, new_scale);
}