#include <fstream>
#include <string>
#include <vector>
#include "Settings.h"
#include "SoundsHandler.h"

using namespace std;
using rtd::Settings;

Settings rtd::settings;

Settings::Settings()
{
	loadFromFile();
}

void Settings::saveToFile()
{
	ofstream file(m_filename, ios::trunc);

	file << "music_volume " << m_music_volume << endl
		 << "sounds_volume " << m_sounds_volume << endl;
}

void Settings::loadFromFile()
{
	string readString;
	ifstream file(m_filename);

	if (!file.is_open())
		return;

	while (getline(file, readString))
	{
		if (count(readString.begin(), readString.end(), ' ') != 1)
			continue;

		size_t space_index = readString.find(' ', 0);
		string var = readString.substr(0, space_index);
		string val = readString.substr(space_index + 1, readString.size() - 1 - space_index);

		if (var == "music_volume") {
			m_music_volume = stof(val);
			continue;
		}

		if (var == "sounds_volume") {
			m_sounds_volume = stof(val);
			continue;
		}
	}
}

void Settings::setMusicVolume(float volume)
{
	extern SoundsHandler globalSoundsHandler;
	m_music_volume = volume;
	globalSoundsHandler.setMusicVolume(100);
	saveToFile();
}

void Settings::setSoundsVolume(float volume)
{
	extern SoundsHandler globalSoundsHandler;
	m_sounds_volume = volume;
	globalSoundsHandler.setSoundVolume(100);
	saveToFile();
}

float Settings::getMusicVolume() const
{
	return m_music_volume;
}

float Settings::getSoundsVolume() const
{
	return m_sounds_volume;
}
