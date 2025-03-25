#include "LevelEndAnimation.h"
#include "Constants.h"
#include "SoundPaths.h"
#include "ResourceHandler.h"
#include "Settings.h"

using namespace std;
using namespace rtd;
using namespace sf;

LevelEndAnimation::LevelEndAnimation(int lives_left, float time_left, int coins_count, int balls_count)
{
	m_player_score = lives_left * 100 + time_left * 25 + coins_count * 50 + balls_count * 1000;
	m_blackScreen = RectangleShape({ WIN_WIDTH, WIN_HEIGHT });
	m_blackScreen.setPosition(0, 0);
	m_blackScreen.setFillColor({0,0,0,0});
	prepareBrush();
	prepareTexts();
	prepareAnimations();
	prepareSound();
}

void LevelEndAnimation::prepareSound()
{
	extern Settings settings;
	m_spraySoundBuffer.loadFromFile( string(LEVEL_SOUNDS_PATH) + "spray.ogg");
	m_scoreSoundBuffer.loadFromFile( string(LEVEL_SOUNDS_PATH) + "score_count.ogg");
	m_spraySound.setBuffer(m_spraySoundBuffer);
	m_scoreSound.setBuffer(m_scoreSoundBuffer);
	m_spraySound.setVolume(settings.getSoundsVolume());
	m_scoreSound.setVolume(settings.getSoundsVolume());
}

void LevelEndAnimation::prepareBrush()
{
	if (!m_brushTexture.loadFromFile("Textures/Statics/score_brush.png"))
		throw runtime_error("Ocurrió un error al intentar leer el archivo 'Textures/Statics/score_brush.png'");

	m_brushSprite.setTexture(m_brushTexture);
	m_brushSprite.setScale(m_brush_width / m_brushTexture.getSize().x, m_brush_width / m_brushTexture.getSize().x);
	m_brushSprite.setOrigin(m_brushSprite.getLocalBounds().width / 2, m_brushSprite.getLocalBounds().height / 2);
	m_brushSprite.setPosition(WIN_WIDTH / 2, WIN_HEIGHT / 2);
	m_brushSprite.setTextureRect(IntRect{ 0, 0, 0, m_brushSprite.getTextureRect().height });
}

void LevelEndAnimation::prepareTexts()
{
	extern ResourceHandler resourceHandler;

	m_scoreText.setString("SCORE");

	m_scoreText.setFont(*resourceHandler.getTitleFont());
	m_scoreText.setScale(.5, .5);
	m_scoreText.setCharacterSize(75);
	m_scoreText.setOutlineColor({ 0,0,0 });
	m_scoreText.setOutlineThickness(5);
	m_scoreText.setFillColor({ 255,255,255 });
	m_scoreText.setOrigin(0, m_scoreText.getLocalBounds().height);
	m_scoreText.setPosition((WIN_WIDTH - m_brush_width) / 2 + 120, WIN_HEIGHT / 2);

	m_scoreCountText = m_scoreText;
	m_scoreCountText.setString("0");
	m_scoreCountText.setOrigin(m_scoreCountText.getLocalBounds().width, m_scoreCountText.getLocalBounds().height);
	m_scoreCountText.setPosition((WIN_WIDTH + m_brush_width) / 2 - 120, WIN_HEIGHT / 2);
	m_scoreCountText.setLetterSpacing(2.2);
}

void LevelEndAnimation::prepareAnimations()
{
	m_brushAnimation.start = 0;
	m_brushAnimation.end = m_brushAnimation.start + .5;
	m_brushAnimation.factor = m_brushTexture.getSize().x / m_brushAnimation.end;

	m_scoreAnimation.start = m_brushAnimation.end + .8;
	m_scoreAnimation.end = m_scoreAnimation.start + 2.2;
	m_scoreAnimation.factor = m_player_score / (m_scoreAnimation.end - m_scoreAnimation.start);

	m_fadeoutAnimation.start = m_scoreAnimation.end + 2;
	m_fadeoutAnimation.end = m_fadeoutAnimation.start + 1;
	m_fadeoutAnimation.factor = 255 / (m_fadeoutAnimation.end - m_fadeoutAnimation.start);
}

void LevelEndAnimation::updateBrushAnimation()
{
	auto currentRect = m_brushSprite.getTextureRect();
	currentRect.width = m_brushAnimation.factor * m_elapsed_time;
	m_brushSprite.setTextureRect(currentRect);
}

void LevelEndAnimation::updateScoreAnimation()
{
	m_scoreCountText.setString(to_string((int)ceil(m_scoreAnimation.factor*(m_elapsed_time - m_scoreAnimation.start)) ) );
	m_scoreCountText.setOrigin(m_scoreCountText.getLocalBounds().width, m_scoreCountText.getLocalBounds().height);
}

void LevelEndAnimation::updateFadeoutAnimation()
{
	sf::Uint8 alpha = m_fadeoutAnimation.factor * (m_elapsed_time - m_fadeoutAnimation.start);
	m_blackScreen.setFillColor({0,0,0, alpha});
}

float LevelEndAnimation::getScore() const
{
	return m_player_score;
}

bool LevelEndAnimation::animationsAreCompleted() const
{
	return m_elapsed_time >= m_fadeoutAnimation.end;
}

void LevelEndAnimation::update(const float& delta_time)
{
	m_elapsed_time += delta_time;

	if (m_elapsed_time >= m_brushAnimation.start && m_elapsed_time <= m_brushAnimation.end) {
		if (!m_brushAnimation.sound_was_played) {
			m_spraySound.play();
			m_brushAnimation.sound_was_played = true;

		}
		updateBrushAnimation();
		return;
	}
	
	if (m_elapsed_time >= m_scoreAnimation.start && m_elapsed_time <= m_scoreAnimation.end) {
		if (!m_scoreAnimation.sound_was_played) {
			m_scoreSound.play();
			m_scoreAnimation.sound_was_played = true;
		}
		updateScoreAnimation();
		return;
	}

	if (m_elapsed_time >= m_fadeoutAnimation.start &&
		m_elapsed_time <= m_fadeoutAnimation.end)
		updateFadeoutAnimation();
}

void LevelEndAnimation::draw(RenderWindow& window)
{
	window.draw(m_brushSprite);
	if (m_elapsed_time > m_scoreAnimation.start - .6) {
		window.draw(m_scoreText);
		window.draw(m_scoreCountText);
		window.draw(m_blackScreen);
	}
}