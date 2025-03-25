#include <stdexcept>
#include <cstring>
#include <fstream>
#include "Profile.h"
#include "Decrypter.h"

using namespace std;
using namespace rtd;

Profile rtd::currentProfile;

Profile::Profile()
{
	initValues();
}

Profile::Profile(string map_name, string player_name)
{
	strcpy_s(m_map_name, map_name.c_str());
	strcpy_s(m_player_name, player_name.c_str());

	if (!loadFromFile(m_map_name, m_player_name)) {
		initValues();
		if (!save())
			throw runtime_error("Ocurrió un error al intentar crear el perfil de '" + string(player_name) + "'");
	}
}

void Profile::initValues()
{
	for (int i = 0; i < 7; ++i)
		m_dragonBalls[i] = false;

	for (int i = 0; i < MAP_MAX_LEVELS_COUNT; ++i)
		m_levelScores[i] = 0;
}

bool Profile::isListed() const
{
	ifstream file("Saved games/" + string(m_map_name) + "/profile_list.txt");

	if (!file.is_open())
		return false;

	string profile_name;

	while (getline(file, profile_name))
		if (strcmp(profile_name.c_str(), m_player_name) == 0)
			return true;

	return false;
}

bool Profile::addToList()
{
	ofstream file("Saved games/" + string(m_map_name) + "/profile_list.txt", ios::app);
	
	if (!file.is_open())
		return false;

	file << m_player_name << endl;
	return true;
}

void Profile::setMap(string name)
{
	strcpy_s(m_map_name, name.c_str());
}

void Profile::setPlayer(string name)
{
	strcpy_s(m_player_name, name.c_str());
}

void Profile::setDragonBalls(vector<int> ballsNumbers)
{
	for (auto it = ballsNumbers.begin(); it != ballsNumbers.end(); ++it)
		if (1 <= *it && *it <= 7)
			m_dragonBalls[ (*it)-1 ] = true;
}

void Profile::setLevelScore(int level, float score)
{
	if (score > 0)
		m_levelScores[level] = score;
}

void Profile::setWonGame(bool value)
{
	m_won_game = value;
}

void Profile::reset()
{
	if (string(m_map_name).empty() || string(m_player_name).empty())
		return;

	initValues();
	save();
}

bool Profile::loadFromFile(string map_name, string player_name)
{
	if (map_name.empty() || player_name.empty())
		return false;

	strcpy_s(m_map_name, map_name.c_str());
	strcpy_s(m_player_name, player_name.c_str());

	string path = "Saved games/" + string(m_map_name) + "/" + string(m_player_name) + ".rdsave";

	ifstream file(path, ios::binary);

	if (!file.is_open())
		return false;

	file.read(reinterpret_cast<char*>(this), sizeof(Profile));
	file.close();
}

bool Profile::save()
{
	if (strcmp(m_map_name, "") == 0)
		return false;

	string path = "Saved games/" + string(m_map_name) + "/" + string(m_player_name) + ".rdsave";

	ofstream binFile(path, ios::binary|ios::trunc);

	if (!binFile.is_open())
		return false;

	binFile.write(reinterpret_cast<char*>(this), sizeof(Profile));
	binFile.close();

	if (!isListed())
		if (!addToList())
			return false;

	return true;
}

void Profile::updateHighScores()
{
	auto total_score = getTotalScore();
	auto highScores = getHighScores();
	int position = 0;
	int new_position = -1;
	auto newPositionIt = highScores.end();
	auto currentPositionIt = highScores.end();

	if (highScores.size() == 0)
		new_position = 0;

	for (auto scoreIt = highScores.begin(); scoreIt != highScores.end(); ++scoreIt)
	{
		if ( strcmp(scoreIt->first.c_str(), getPlayerName()) == 0 )
			currentPositionIt = scoreIt;

		if (new_position == -1 && total_score > scoreIt->second)
			new_position = position;

		++position;
	}

	if (currentPositionIt != highScores.end())
		highScores.erase(currentPositionIt);

	if (new_position >= 0)
		highScores.insert( next(highScores.begin(), new_position), {m_player_name, total_score});
	else
		highScores.insert( highScores.end(), { m_player_name, total_score });

	if (highScores.size() > 10)
		highScores.resize(10);

	saveHighScores(highScores);
}

void Profile::saveHighScores(vector< pair<string, float> > updatedHighScores)
{
	string path = "Saved games/" + string(m_map_name) + "/high_scores.txt";
	ofstream file(path, ios::trunc);
	for (auto score = updatedHighScores.begin(); score != updatedHighScores.end(); ++score)
		file << score->first << " " << to_string(score->second) << endl;
}

vector< pair<string, float> > Profile::getHighScores() const
{
	vector< pair<string, float> > highScores;
	string buffer, player_name;
	float score;

	string path = "Saved games/" + string(m_map_name) + "/high_scores.txt";
	ifstream file(path, ios::app);

	if (!file.is_open())
		return highScores;

	while (getline(file, buffer))
	{
		auto separator_pos = buffer.find(' ', 0);
		if(separator_pos == string::npos)
			throw runtime_error("El archivo de puntajes del mapa " + string(m_map_name) + 
				" está corrupto. Ingrese a la carpeta del juego siga la ruta 'Saved games/" +
				string(m_map_name) + "' y elimine 'high_scores.txt'");

		player_name = buffer.substr(0, separator_pos);
		score = stof(buffer.substr(separator_pos+1, buffer.size()-1-separator_pos));
		highScores.push_back({ player_name, score });
	}

	return highScores;
}

vector<string> Profile::getSavedProfileNames() const
{
	vector<string> profileList;
	ifstream file("Saved games/" + string(m_map_name) + "/profile_list.txt");

	if (file.is_open()) {
		string profile_name;

		while (getline(file, profile_name))
			profileList.push_back(profile_name);
	}

	return profileList;
}

char* Profile::getMapName() const
{
	char* cstr = new char[MAP_NAME_MAX_LENGTH + 1];
	strcpy_s(cstr, MAP_NAME_MAX_LENGTH, m_map_name);
	return cstr;
}

char* Profile::getPlayerName() const
{
	char* cstr = new char[PLAYER_NAME_MAX_LENGTH + 1];
	strcpy_s(cstr, PLAYER_NAME_MAX_LENGTH, m_player_name);
	return cstr;
}

vector<int> Profile::getDragonBalls() const
{
	vector<int> collectedBalls;

	for (int i = 0; i < 7; ++i)
		if (*(m_dragonBalls + i) == true) {
			collectedBalls.push_back(i + 1);
		}

	return collectedBalls;
}

vector<float> Profile::getLevelScores() const
{
	vector<float> scores;

	//Sale del bucle al primer 0 porque si un score es 0 no se terminó ese nivel
	//por lo tanto los que siguen no pueden estar desbloqueados.

	for (int i = 0; i < MAP_MAX_LEVELS_COUNT; ++i)
		if (m_levelScores[i] == 0)
			break;
		else
			scores.push_back(m_levelScores[i]);

	return scores;
}

float Profile::getTotalScore() const
{
	float total_score = 0;
	auto scores = getLevelScores();
	for (auto score = scores.begin(); score != scores.end(); ++score)
		total_score += *score;
	return total_score;
}

bool Profile::wonGame() const
{
	return m_won_game;
}