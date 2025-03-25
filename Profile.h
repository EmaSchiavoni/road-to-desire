#pragma once
#include <vector>
#include <string>
#include <utility>
#include "Constants.h"

namespace rtd {

	class Profile
	{
		char m_map_name[ rtd::MAP_NAME_MAX_LENGTH ] = "";
		char m_player_name[ rtd::PLAYER_NAME_MAX_LENGTH ] = "";
		float m_levelScores[ rtd::MAP_MAX_LEVELS_COUNT ];
		bool m_dragonBalls[7];
		bool m_won_game = false;
		void initValues();
		bool addToList();
		bool isListed() const;

	public:
		Profile();
		Profile(std::string mapName, std::string playerName);
		void setMap(std::string name);
		void setPlayer(std::string name);
		void setDragonBalls(std::vector<int> ballsNumbers);
		void setLevelScore(int level, float score);
		void setWonGame(bool value);
		void reset();
		bool loadFromFile(std::string mapName, std::string playerName);
		bool save();
		void updateHighScores();
		void saveHighScores(std::vector<std::pair<std::string, float>> updatedHighScores);
		std::vector<std::pair<std::string, float>> getHighScores() const;
		std::vector<std::string> getSavedProfileNames() const;

		char* getMapName() const;
		char* getPlayerName() const;
		std::vector<int> getDragonBalls() const;
		std::vector<float> getLevelScores() const;
		float getTotalScore() const;
		bool wonGame() const;
	};

	//Objeto global para manejar el perfil actual
	extern rtd::Profile currentProfile;

}