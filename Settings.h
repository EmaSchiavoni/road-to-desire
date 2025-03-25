#pragma once

namespace rtd{

	class Settings
	{
		const char* m_filename = "settings.cfg";
		float m_music_volume = 70;
		float m_sounds_volume = 100;

		void saveToFile();
		void loadFromFile();

	public:
		Settings();
		void setMusicVolume(float volume);
		void setSoundsVolume(float volume);
		float getMusicVolume() const;
		float getSoundsVolume() const;
	};

	//Objeto global para acceder a los ajustes del usuario
	extern rtd::Settings settings;

}