#include "SoundsHandler.h"
#include "Settings.h"

using namespace std;
using namespace rtd;
using namespace sf;

extern Settings settings;

SoundsHandler rtd::globalSoundsHandler;

SoundsHandler::SoundsHandler()
{
	m_sound.setBuffer(m_soundBuffer);
}

void SoundsHandler::setMusic(string path, bool loop)
{
	m_music.setLoop(loop);

	if (path == m_musicPath && m_music.getStatus() == SoundSource::Status::Playing)
		return;

	if (path == m_musicPath)
		m_music.play();

	if (m_musicPath != path) {
		if (m_music.getStatus() == SoundSource::Status::Playing)
			fadeOutMusic();

		m_music.stop();
		setMusicVolume(100);
		m_music.openFromFile(path);
		m_musicPath = path;
		m_music.play();
	}
}

void SoundsHandler::setMusicVolume(float volume)
{
	volume = volume / 100 * settings.getMusicVolume();
	m_music.setVolume(volume);
}

void SoundsHandler::pauseMusic()
{
	m_music.pause();
}

void SoundsHandler::playMusic()
{
	m_music.play();
}


void SoundsHandler::stopMusic()
{
	m_music.stop();
}

void SoundsHandler::fadeOutMusic()
{
	float i = m_music.getVolume();
	while (i > 1) {
		i -= .00002f;
		m_music.setVolume(i);
	}
	m_music.setVolume(0);
}

string SoundsHandler::getMusicPath() const
{
	return m_musicPath;
}

void SoundsHandler::setSound(string path, bool loop)
{
	m_sound.setLoop(loop);

	if (path == m_soundPath)
		m_sound.play();

	if (m_soundPath != path) {
		m_sound.stop();
		setSoundVolume(100);
		m_soundBuffer.loadFromFile(path);
		m_soundPath = path;
		m_sound.play();
	}
}

void SoundsHandler::setSoundVolume(float volume)
{
	volume = volume / 100 * settings.getSoundsVolume();
	m_sound.setVolume(volume);
}

void SoundsHandler::stopSound()
{
	m_sound.stop();
}

void SoundsHandler::fadeOutSound()
{
	float i = m_sound.getVolume();
	while (i > 1) {
		i -= .00005f;
		m_sound.setVolume(i);
	}
	m_sound.setVolume(0);
}

string SoundsHandler::getSoundPath() const
{
	return m_soundPath;
}