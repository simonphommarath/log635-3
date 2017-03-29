#include <iostream>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <cassert>
#include <vector>
#include <utility>
#include <algorithm>
#include <cstdint>
#include <string>
#include <sstream>
#include <vector>
#include <iterator>

class Player
{
public:
	int GameID;
	int LeagueIndex;
	int ActualLeagueIndex;
	int Age;
	int HoursPerWeek;
	int TotalHours;
	float APM;
	float SelectByHotkeys;
	float AssignToHotkeys;
	float UniqueHotkeys;
	float MinimapAttacks;
	float MinimapRightClicks;
	float NumberOfPACs;
	float GapBetweenPACs;
	float ActionLatency;
	float ActionsInPAC;
	float TotalMapExplored;
	float WorkersMade;
	float UniqueUnitsMade;
	float ComplexUnitsMade;
	float ComplexAbilitiesUsed;
	float score;

};

class PairDistance
{
public:
	float Distance;
	int RankGamerId;
	int NonRankGamerId;
	int RankGamerLeagueIndex;
	int NonRankGamerLeagueIndex;
};

struct less_than_key
{
	inline bool operator() (const PairDistance& pairDistance1, const PairDistance& pairDistance2)
	{
		return (pairDistance1.Distance < pairDistance2.Distance);
	}
};

std::vector<std::string> split(const std::string &s, char c) {
	std::string buff{ "" };
	std::vector<std::string> v;

	for (auto n : s)
	{
		if (n != c) buff += n;
		else if (n == c && buff != "")
		{
			v.push_back(buff);
			buff = "";
		}
	}
	if (buff != "")
		v.push_back(buff);

	return v;
}

std::string remove_char(std::string str, char ch)
{
	// remove all occurrences of char ch from str
	str.erase(std::remove(str.begin(), str.end(), ch), str.end());
	return str;
}

std::vector<Player> read_csv(const std::string path)
{
	std::vector<Player> players;
	std::string text("");
	std::string line;
	std::ifstream myfile(path);
	if (myfile.is_open())
	{
		getline(myfile, line);
		while (getline(myfile, line))
		{
			std::size_t found = line.find("NULL");
			if (found != std::string::npos)
				continue;

			line = remove_char(line, '"');
			std::vector<std::string> data = split(line, ',');
			Player player;
			player.GameID = std::stoi(data[0]);
			player.LeagueIndex = std::stoi(data[1]);
			player.ActualLeagueIndex = std::stoi(data[1]);
			player.Age = std::stoi(data[2]);
			player.HoursPerWeek = std::stoi(data[3]);
			player.TotalHours = std::stoi(data[4]);
			player.APM = std::stof(data[5]);
			player.SelectByHotkeys = std::stof(data[6]);
			player.AssignToHotkeys = std::stof(data[7]);
			player.UniqueHotkeys = std::stof(data[8]);
			player.MinimapAttacks = std::stof(data[9]);
			player.MinimapRightClicks = std::stof(data[10]);
			player.NumberOfPACs = std::stof(data[11]);
			player.GapBetweenPACs = std::stof(data[12]);
			player.ActionLatency = std::stof(data[13]);
			player.ActionsInPAC = std::stof(data[14]);
			player.TotalMapExplored = std::stof(data[15]);
			player.WorkersMade = std::stof(data[16]);
			player.UniqueUnitsMade = std::stof(data[17]);
			player.ComplexUnitsMade = std::stof(data[18]);
			player.ComplexAbilitiesUsed = std::stof(data[19]);
			player.score = 0;

			players.push_back(player);

			text += line + '\n';
		}
		myfile.close();
	}

	//else cout << "Unable to open file"; 

	return players;
}

void split_data(std::vector<Player> *players, int nb_fold, int i,
	std::vector<Player> *training, std::vector<Player> *test)
{
	int nb_element = players->size() / nb_fold;


	if (i > 0)
	{
		std::vector<Player>::const_iterator first = players->begin();
		std::vector<Player>::const_iterator last = players->begin() + nb_element * i;
		//*training = std::vector<Player>(first, last);
		training->insert(training->end(), first, last);

	}

	std::vector<Player>::const_iterator first = players->begin() + nb_element * i;
	std::vector<Player>::const_iterator last = players->begin() + nb_element * (i + 1);
	*test = std::vector<Player>(first, last);

	if (i < nb_fold - 1)
	{
		std::vector<Player>::const_iterator first = players->begin() + nb_element * (i + 1);
		std::vector<Player>::const_iterator last = players->end();
		//*training = std::vector<Player>(first, last);
		training->insert(training->end(), first, last);
	}
}

void trim(std::vector<Player> *players)
{

}

void rate(std::vector<Player> *players)
{
	int score = 0;
	for (auto& player : *players)
		score += player.LeagueIndex == player.LeagueIndex;
	//std::cout << "the score is " << score << std::endl;
}

void normalizePlayerFieldIndexOf(std::vector<Player> *players, int playerFieldValue) 
{
	float minValue = 99999.0;
	float maxValue = 0.0;
	float etendueValeur = 0.0;

	switch (playerFieldValue) {//								IMPORTANCE : 0(low) to 10(high).
	case 1://GameID												PAS IMPORTANT
		break;
	case 2://LeagueIndex										10/10
		   //Pas besoin de normalisation ecart trop petit
		break;
	case 3://Age												PAS IMPORTANT
		break;
	case 4://HoursPerWeek										PAS IMPORTANT
		break;
	case 5://TotalHours											PAS IMPORTANT
		break;
	case 6://APM													9/10
		   // Find min max etendu
		for (auto& player : *players) {
			if (player.APM < minValue)
				minValue = player.APM;
			if (player.APM > maxValue)
				maxValue = player.APM;
		}
		etendueValeur = maxValue - minValue;

		//Si ecart >50, on normalise avec Solution 1 normalisation Note de cours : Vi' = (Vi-Vmin)/Ek, cours 10-11 diapo 31
		if (etendueValeur > 50) {
			for (auto& player : *players) {
				//float playerOldValueTest = player.APM;				//UNCOMMENT FOR TEST
				player.APM = (((float)player.APM - (float)minValue) / etendueValeur);
				//std::cout << "apm of a player was: " << playerOldValueTest << " and now is: " << player.APM << std::endl;		//UNCOMENT FOR TEST
			}
		}
		break;
	case 7://SelectByHotkeys										3/10
		break;
	case 8://AssignToHotkeys										1/10
		break;
	case 9://UniqueHotkeys											5/10
		break;
	case 10://MinimapAttacks									PAS IMPORTANT
		break;
	case 11://MinimapRightClicks								PAS IMPORTANT
		break;
	case 12://NumberOfPACs											7/10
		break;
	case 13://GapBetweenPACs									PAS IMPORTANT (trop petite donnee ms = precision mediocre)
		break;
	case 14://ActionLatency										PAS IMPORTANT (trop petite donnee ms = precision mediocre)
		break;
	case 15://ActionsInPAC											4/10
		break;
	case 16://TotalMapExplored										6/10
		break;
	case 17://WorkersMade											2/10
		break;
	case 18://UniqueUnitsMad										3/10
		break;
	case 19://ComplexUnitsMade									PAS IMPORTANT
		break;
	case 20://ComplexAbilitiesUsed									8/10
		break;
	}
}

/*
Normalize players informations :
Solution 1 Note de cours : Vi' = (Vi-Vmin)/Ek
+
Solution 3 Note de cours : Approx, connaissance du domaine
*/
void normalizePlayers(std::vector<Player> *players)
{
	int nb_element = players->size();
	int minValue = 0;

	//Second param is the index of the field value
	normalizePlayerFieldIndexOf(players, 6);

	/*for (auto& player : *players)		uncomment for test
	std::cout << player.APM << std::endl;		uncomment for test*/
}

template<class Property>
void normalize_attribute(std::vector<Player> * players, Property  prop)
{
	auto bounds = std::minmax_element(players->begin(), players->end(),
		[prop](Player p1, Player p2) {return *prop(&p1) < *prop(&p2);});

	for (auto player : *players) {
		int min = *prop(&*bounds.first);
		int max = *prop(&*bounds.second);
		*prop(&player) = (*prop(&player) - min) / (max - min);
	}
}

void normalize(std::vector<Player> *players)
{
	normalize_attribute(players, [](Player* p) -> float* {return &(p->APM);});
	normalize_attribute(players, [](Player* p) -> float* {return &(p->SelectByHotkeys);});
	normalize_attribute(players, [](Player* p) -> float* {return &(p->AssignToHotkeys);});
	normalize_attribute(players, [](Player* p) -> float* {return &(p->MinimapAttacks);});
	normalize_attribute(players, [](Player* p) -> float* {return &(p->NumberOfPACs);});
	normalize_attribute(players, [](Player* p) -> float* {return &(p->GapBetweenPACs);});
	normalize_attribute(players, [](Player* p) -> float* {return &(p->ActionLatency);});
	normalize_attribute(players, [](Player* p) -> float* {return &(p->TotalMapExplored);});
	normalize_attribute(players, [](Player* p) -> float* {return &(p->WorkersMade);});
}

/*
SCORE FUNCTION WITH 6 IMPORTANTS PARAMS !!!
*/
void calculScoresOfPlayers(std::vector<Player> *players) 
{
	// Avec top 6 priority data field : 
	for (auto& player : *players) {
		player.score = ((player.ComplexAbilitiesUsed + player.NumberOfPACs + player.TotalMapExplored + player.UniqueHotkeys) / player.APM);
		std::cout << "player has score of : " << player.score << std::endl;
	}
}

void KNNAlgorithm(std::vector<Player> *rankedPlayers, std::vector<Player> *nonRankPlayers, int k)
{
	double perfectMatch(0);
	double closeMatch(0);
	double wrongMatch(0);

	for (auto& nonRankPlayer : *nonRankPlayers) {
		std::vector<PairDistance> playerDistances;

		// Calculated distance between players
		for (auto& rankedPlayer : *rankedPlayers) {
			// Number of attributs doesn't really affect the result
			float distance = sqrt(
				pow(2, (nonRankPlayer.APM - rankedPlayer.APM)) +
				pow(2, (nonRankPlayer.SelectByHotkeys - rankedPlayer.SelectByHotkeys)) +
				pow(2, (nonRankPlayer.AssignToHotkeys - rankedPlayer.AssignToHotkeys)) +
				pow(2, (nonRankPlayer.MinimapAttacks - rankedPlayer.MinimapAttacks)) +
				pow(2, (nonRankPlayer.NumberOfPACs - rankedPlayer.NumberOfPACs)) +
				pow(2, (nonRankPlayer.GapBetweenPACs - rankedPlayer.GapBetweenPACs)) +
				pow(2, (nonRankPlayer.ActionLatency - rankedPlayer.ActionLatency)) +
				pow(2, (nonRankPlayer.TotalMapExplored - rankedPlayer.TotalMapExplored)) +
				pow(2, (nonRankPlayer.WorkersMade - rankedPlayer.WorkersMade))
			);

			PairDistance pairDistance;
			pairDistance.Distance = distance;
			pairDistance.RankGamerId = rankedPlayer.GameID;
			pairDistance.NonRankGamerId = nonRankPlayer.GameID;

			pairDistance.RankGamerLeagueIndex = rankedPlayer.LeagueIndex;
			pairDistance.NonRankGamerLeagueIndex = nonRankPlayer.LeagueIndex;

			playerDistances.push_back(pairDistance);
		}

		// Sort distance
		std::sort(playerDistances.begin(), playerDistances.end(), less_than_key());
		playerDistances.resize(k);

		int rank[8] = { 0,0,0,0,0,0,0,0 };
		int highestLeague;
		int maxValue(0);

		// Get closest league
		for (auto& playerDistance : playerDistances) {
			rank[playerDistance.RankGamerLeagueIndex - 1]++;
			if (rank[playerDistance.RankGamerLeagueIndex - 1] > maxValue) {
				maxValue = rank[playerDistance.RankGamerLeagueIndex - 1];
				//TO FIX: multiple highestLeague
				highestLeague = playerDistance.RankGamerLeagueIndex;
			}
		}

		//std::cout << "PLAYER:" << nonRankPlayer.GameID << " League:" << highestLeague << " Real League:" << nonRankPlayer.LeagueIndex << std::endl;

		if (highestLeague == nonRankPlayer.LeagueIndex)
			perfectMatch++;
		else if (highestLeague == nonRankPlayer.LeagueIndex + 1 || highestLeague == nonRankPlayer.LeagueIndex - 1)
			closeMatch++;
		else
			wrongMatch++;
	}

	std::cout << "Perfect Match: " << perfectMatch << std::endl;
	std::cout << "Close Match: " << closeMatch << std::endl;
	std::cout << "Wrong Match: " << wrongMatch << std::endl;

	std::cout << "Perfect Match %: " << (perfectMatch / nonRankPlayers->size()) * 100 << std::endl;
	std::cout << "Close Match %: " << ((perfectMatch + closeMatch) / nonRankPlayers->size()) * 100 << std::endl;
}

int main() 
{
	std::cout << "Hello World!" << std::endl;
	std::string path("data.csv");
	//String path = chose_file()
	//std::string file = read_file(path );
	std::vector<Player> players = read_csv(path);
	int nb_fold = 5;

	for (int i = 0; i < nb_fold; ++i)
	{
		std::vector<Player> training;
		std::vector<Player> test;

		normalize(&training);
		normalize(&test);

		split_data(&players, nb_fold, i, &training, &test);

		//normalizePlayers(&players);
		//calculScoresOfPlayers(&players);

		//trim(&training);

		std::cout << "KNNAlgorithm Fold: "<< i << std::endl;
		KNNAlgorithm(&training, &test, 16);
		//validate(&test);

		//rate(&players);

		std::cout << std::endl;
	}
	
	return 0;
}
