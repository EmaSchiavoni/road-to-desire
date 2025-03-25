#include "GameOverAnimation.h"
#include "Constants.h"
#include "SoundPaths.h"
#include "ResourceHandler.h"
#include "Settings.h"

using namespace std;
using namespace rtd;
using namespace sf;

GameOverAnimation::GameOverAnimation()
{
	prepareRects();
	prepareText();
	prepareAnimations();
	prepareSound();
}

void GameOverAnimation::prepareSound()
{
	extern Settings settings;
	m_ballSoundBuffer.loadFromFile(string(LEVEL_SOUNDS_PATH) + "game_over.ogg");
	m_ballSound.setBuffer(m_ballSoundBuffer);
	m_ballSound.setVolume(settings.getSoundsVolume());
}

void GameOverAnimation::prepareRects()
{
	m_topBgRect = RectangleShape({ WIN_WIDTH, WIN_HEIGHT / 2 });
	m_topBgRect.setFillColor({ 0,0,0 });
	m_topBgRect.setOutlineColor(m_pureYellow);
	m_topBgRect.setOutlineThickness(1);
	m_botBgRect = m_topBgRect;
	m_topBgRect.setOrigin(0, m_topBgRect.getLocalBounds().height);
	m_botBgRect.setPosition(0, WIN_HEIGHT);
}

void GameOverAnimation::prepareText()
{
	extern ResourceHandler resourceHandler;
	m_text.setFont(*resourceHandler.getMenuFont());
	m_text.setCharacterSize(150);
	m_text.setScale(.5, .5);
	m_text.setString("GAME OVER");
	m_text.setFillColor(m_pureMagenta);
	m_text.setOrigin(m_text.getLocalBounds().width / 2, m_text.getLocalBounds().height / 2 + 50);
	m_text.setPosition(WIN_WIDTH / 2, WIN_HEIGHT / 2);
	m_shadow = m_text;
	m_shadow.setFillColor(m_pureYellow);
	m_shadow.setPosition(WIN_WIDTH / 2 - 1.2, WIN_HEIGHT / 2 + 5);
	m_shadow.setOutlineColor({ 0,0,0 });
	m_shadow.setOutlineThickness(55);
}

void GameOverAnimation::prepareAnimations()
{
	m_rectsAnimation.start = 0;
	m_rectsAnimation.end = m_rectsAnimation.start + 1.6;
	m_rectsAnimation.factor = (WIN_HEIGHT / 2) / (m_rectsAnimation.end - m_rectsAnimation.start);

	m_textAnimation.start = m_rectsAnimation.end + .2;
	m_textAnimation.end = m_textAnimation.start + .2;
	m_textAnimation.factor = 255 / (m_textAnimation.end - m_textAnimation.start);
}

void GameOverAnimation::updateRectsAnimation()
{
	auto newTopRectPos = m_topBgRect.getPosition();
	auto newBotRectPos = m_botBgRect.getPosition();
	float value = m_rectsAnimation.factor * (m_elapsed_time - m_rectsAnimation.start);
	newTopRectPos.y = value;
	newBotRectPos.y = WIN_HEIGHT - value;
	m_topBgRect.setPosition(newTopRectPos);
	m_botBgRect.setPosition(newBotRectPos);
}

void GameOverAnimation::updateTextAnimation()
{
	Color magenta = m_pureMagenta;
	magenta.a = m_textAnimation.factor * (m_elapsed_time - m_textAnimation.start);
	m_text.setFillColor(magenta);

	Color yellow = m_pureYellow;
	yellow.a = magenta.a;
	m_shadow.setFillColor(yellow);
}

bool GameOverAnimation::animationsAreCompleted() const
{
	return m_elapsed_time >= m_textAnimation.end + 2.5;
}

void GameOverAnimation::update(const float& delta_time)
{
	m_elapsed_time += delta_time;

	if (m_elapsed_time >= m_rectsAnimation.start && m_elapsed_time <= m_rectsAnimation.end) {
		if (!m_rectsAnimation.sound_was_played) {
			m_ballSound.play();
			m_rectsAnimation.sound_was_played = true;
		}
		updateRectsAnimation();
		return;
	}

	if (m_elapsed_time >= m_textAnimation.start && m_elapsed_time <= m_textAnimation.end)
		updateTextAnimation();
}

void GameOverAnimation::draw(RenderWindow& window)
{
	window.draw(m_topBgRect);
	window.draw(m_botBgRect);
	if (m_elapsed_time > m_textAnimation.start) {
		window.draw(m_shadow);
		window.draw(m_text);
	}
}