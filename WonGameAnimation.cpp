#include "WonGameAnimation.h"
#include "Constants.h"
#include "SoundPaths.h"
#include "ResourceHandler.h"
#include "Settings.h"
#include "SoundsHandler.h"

using namespace std;
using namespace rtd;
using namespace sf;

extern SoundsHandler globalSoundsHandler;

WonGameAnimation::WonGameAnimation()
{
	prepareRects();
	prepareBalls();
	prepareText();
	prepareAnimations();
	prepareSound();
	globalSoundsHandler.fadeOutMusic();
}

void WonGameAnimation::prepareSound()
{
	extern Settings settings;
	m_ballSoundBuffer.loadFromFile(string(MAP_SOUNDS_PATH) + "new.ogg");
	m_ballSound.setBuffer(m_ballSoundBuffer);
	m_ballSound.setVolume(settings.getSoundsVolume());
}

void WonGameAnimation::prepareBalls()
{
	for (int n = 0; n < 7; ++n) {
		if (!m_ballTextures[n].loadFromFile("Textures/Collectables/DragonBall" + to_string(n + 1) + ".png") )
			throw runtime_error("Ocurrió un error al intentar leer el archivo 'Textures/Collectables/DragonBall" + to_string(n + 1) + ".png'");
		m_ballTextures[n].setSmooth(true);
		m_ballSprites[n].setTexture(m_ballTextures[n]);
		m_ballSprites[n].setOrigin(0, m_ballSprites[n].getLocalBounds().height / 2);
		m_ballSprites[n].setPosition(80 + n * m_ballSprites[n].getLocalBounds().width + n * 85 / 6, WIN_HEIGHT / 2);
		m_ballSprites[n].setColor({ 255,255,255,255 });
	}
}

void WonGameAnimation::prepareRects()
{
	m_topBgRect = RectangleShape({ WIN_WIDTH, WIN_HEIGHT / 2 });
	m_topBgRect.setFillColor({ 0,0,0 });
	m_botBgRect = m_topBgRect;
	m_topBgRect.setOrigin(0, m_topBgRect.getLocalBounds().height);
	m_botBgRect.setPosition(0, WIN_HEIGHT);
}

void WonGameAnimation::prepareText()
{
	extern ResourceHandler resourceHandler;
	m_text.setFont(*resourceHandler.getMenuFont());
	m_text.setCharacterSize(150);
	m_text.setScale(.5, .5);
	m_text.setString("HAS GANADO!");
	m_text.setFillColor(m_pureMagenta);
	m_text.setOrigin(m_text.getLocalBounds().width / 2, m_text.getLocalBounds().height / 2 + 50);
	m_text.setPosition(WIN_WIDTH / 2, WIN_HEIGHT / 5);
	m_shadow = m_text;
	m_shadow.setFillColor(m_pureYellow);
	m_shadow.setPosition(WIN_WIDTH / 2 - 1.2, WIN_HEIGHT / 5 + 5);
}

void WonGameAnimation::prepareAnimations()
{
	m_rectsAnimation.start = 0;
	m_rectsAnimation.end = m_rectsAnimation.start + 1.6;
	m_rectsAnimation.factor = (WIN_HEIGHT / 2) / (m_rectsAnimation.end - m_rectsAnimation.start);

	m_ballAnimations[0].start = m_rectsAnimation.end + 1;
	m_ballAnimations[0].end = m_ballAnimations[0].start + m_ball_animation_duration;
	m_ballAnimations[0].factor = 255 / (m_ballAnimations[0].end - m_ballAnimations[0].start);

	for (int n = 1; n < 7; ++n) {
		m_ballAnimations[n].start = m_ballAnimations[n - 1].end + .5;
		m_ballAnimations[n].end = m_ballAnimations[n].start + m_ball_animation_duration;
		m_ballAnimations[n].factor = 255 / (m_ballAnimations[n].end - m_ballAnimations[n].start);
	}

	m_textAnimation.start = m_ballAnimations[6].end + 1;
	m_textAnimation.end = m_textAnimation.start + .5;
	m_textAnimation.factor = 255 / (m_textAnimation.end - m_textAnimation.start);
}

void WonGameAnimation::updateBallsAnimation()
{
	int& n = m_current_ball_animation;

	if (m_elapsed_time >= m_ballAnimations[n].end && m_current_ball_animation < 6)
		++m_current_ball_animation;

	Color spriteColor = m_ballSprites[n].getColor();
	spriteColor.a = m_ballAnimations[n].factor * (m_elapsed_time - m_ballAnimations[n].start);
	m_ballSprites[n].setColor(spriteColor);

	if (!m_ballAnimations[n].sound_was_played && m_elapsed_time > m_ballAnimations[n].start + m_ball_animation_duration/2) {
		m_ballSound.play();
		m_ballAnimations[n].sound_was_played = true;
	}
}

void WonGameAnimation::updateRectsAnimation()
{
	auto newTopRectPos = m_topBgRect.getPosition();
	auto newBotRectPos = m_botBgRect.getPosition();
	float value = m_rectsAnimation.factor * (m_elapsed_time - m_rectsAnimation.start);
	newTopRectPos.y = value;
	newBotRectPos.y = WIN_HEIGHT - value;
	m_topBgRect.setPosition(newTopRectPos);
	m_botBgRect.setPosition(newBotRectPos);
}

void WonGameAnimation::updateTextAnimation()
{
	Color magenta = m_pureMagenta;
	magenta.a = m_textAnimation.factor * (m_elapsed_time - m_textAnimation.start);
	m_text.setFillColor(magenta);

	Color yellow = m_pureYellow;
	yellow.a = magenta.a;
	m_shadow.setFillColor(yellow);
}


void WonGameAnimation::updateBackground(const float& delta_time)
{
	if (m_bg_color)
		m_bg_color_time += delta_time;
	else
		m_bg_color_time -= delta_time;

	Color color(m_bg_color_time * 22, m_bg_color_time * 13, 0);

	m_topBgRect.setFillColor(color);
	m_botBgRect.setFillColor(color);

	if (m_bg_color_time > 7)
		m_bg_color = false;
	if (m_bg_color_time < 2)
		m_bg_color = true;
}

bool WonGameAnimation::animationsAreCompleted() const
{
	return m_elapsed_time >= m_textAnimation.end + 1;
}

void WonGameAnimation::update(const float& delta_time)
{
	m_elapsed_time += delta_time;

	if (m_elapsed_time >= m_rectsAnimation.start && m_elapsed_time <= m_rectsAnimation.end)
		updateRectsAnimation();

	if (m_elapsed_time >= m_textAnimation.start && m_elapsed_time <= m_textAnimation.end)
		updateTextAnimation();

	if (m_elapsed_time >= m_ballAnimations[0].start && m_elapsed_time <= m_ballAnimations[6].end)
		updateBallsAnimation();

	if (!m_music_was_set && m_elapsed_time > m_textAnimation.end) {
		if (settings.getMusicVolume() < 1)
			settings.setMusicVolume(2);

		globalSoundsHandler.setMusicVolume(100);
		globalSoundsHandler.setMusic(string(MUSIC_PATH) + "won.ogg");
		m_music_was_set = true;
	}

	if (animationsAreCompleted())
		updateBackground(delta_time);
}

void WonGameAnimation::draw(RenderWindow& window)
{
	window.draw(m_topBgRect);
	window.draw(m_botBgRect);

	for (int n = 0; n < 7; ++n)
		if (m_elapsed_time > m_ballAnimations[n].start)
			window.draw(m_ballSprites[n]);

	if (m_elapsed_time > m_textAnimation.start) {
		window.draw(m_shadow);
		window.draw(m_text);
	}
}