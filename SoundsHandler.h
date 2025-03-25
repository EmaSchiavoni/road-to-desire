#pragma once
#include <string>
#include <SFML/Audio/Music.hpp>
#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio/SoundBuffer.hpp>

namespace rtd {

	class SoundsHandler
	{
		sf::Music m_music;
		std::string m_musicPath;

		sf::SoundBuffer m_soundBuffer;
		sf::Sound m_sound;
		std::string m_soundPath;
		
	public:
		SoundsHandler();
		void setMusic(std::string path, bool loop = true);
		void setMusicVolume(float volume);
		void pauseMusic();
		void playMusic();
		void stopMusic();
		void fadeOutMusic();
		std::string getMusicPath() const;

		void setSound(std::string path, bool loop = false);
		void setSoundVolume(float volume);
		void stopSound();
		void fadeOutSound();
		std::string getSoundPath() const;
	};

	//Objeto global para controlar los sonidos globales
	extern rtd::SoundsHandler globalSoundsHandler;

}