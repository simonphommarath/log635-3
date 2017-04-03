#include <iostream>
#include <vector>
#include <fstream>
#include <iostream>
#include <string>
#include <cassert>
#include <utility>
#include <vector>
#include <algorithm>
#include <cstdint>
#include <sstream>
#include <string>
#include <iterator>
#include <vector>
#include <cmath>
#include <ctime>

#include "nn.h"
#include "layer.h"
#include "neuron.h"


class Player
{
public:
	int GameID;
	int LeagueIndex;
	std::vector<double> Guess;
	int ActualLeagueIndex;
	int Age;
	int HoursPerWeek;
	int TotalHours;
	double APM;
	double SelectByHotkeys;
	double AssignToHotkeys;
	double UniqueHotkeys;
	double MinimapAttacks;
	double MinimapRightClicks;
	double NumberOfPACs;
	double GapBetweenPACs;
	double ActionLatency;
	double ActionsInPAC;
	double TotalMapExplored;
	double WorkersMade;
	double UniqueUnitsMade;
	double ComplexUnitsMade;
	double ComplexAbilitiesUsed;

	std::vector<double> toArray()
	{
		std::vector<double> array;
		array.reserve(9);
		array.push_back(APM);
		array.push_back(SelectByHotkeys);
		array.push_back(AssignToHotkeys);
		array.push_back(MinimapAttacks);
		array.push_back(NumberOfPACs);
		array.push_back(GapBetweenPACs);
		array.push_back(ActionLatency);
		array.push_back(TotalMapExplored);
		array.push_back(WorkersMade);

		return array;
	}
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
	std::string buff;
	std::vector<std::string> v;

	for (auto n : s)
	{
		if (n != c) {
			buff += n;
		}
		else if (n == c && buff != "")
		{
			v.push_back(buff);
			buff = "";
		}
	}
	if (buff != "") {
		v.push_back(buff);
	}

	return v;
}

std::string remove_char(std::string str, char ch)
{
	// remove all occurrences of char ch from str
	str.erase(std::remove(str.begin(), str.end(), ch), str.end());
	return str;
}

bool replace(std::string& str, const std::string& from, const std::string& to) {
		size_t start_pos = str.find(from);
		if(start_pos == std::string::npos)
				return false;
		str.replace(start_pos, from.length(), to);
		return true;
}

std::vector<Player> read_csv(const std::string& path)
{
	std::vector<Player> players;
	std::string text;
	std::string line;
	std::ifstream myfile(path);
	if (myfile.is_open())
	{
		getline(myfile, line);
		while (getline(myfile, line))
		{
			std::size_t found = line.find("NULL");
			if (found != std::string::npos) {
				continue;
			}

			line = remove_char(line, '"');
			std::vector<std::string> data = split(line, ',');
			Player player{};
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

			players.push_back(player);

			text += line + '\n';
		}
		myfile.close();
	}

	//else cout << "Unable to open file";

	return players;
}

std::vector<Player> read_csv_evaluation(const std::string& path)
{
	std::vector<Player> players;
	std::string text;
	std::string line;
	std::ifstream myfile(path);
	if (myfile.is_open())
	{
		getline(myfile, line);
		while (getline(myfile, line))
		{
			while(replace(line, "NULL", "0"));

//			std::size_t found = line.find("NULL");
//			if (found != std::string::npos) {
//				continue;
//			}

			line = remove_char(line, '"');
			std::vector<std::string> data = split(line, ',');
			Player player{};
			player.GameID = std::stoi(data[0]);
			player.Age = std::stoi(data[1]);
			player.HoursPerWeek = std::stoi(data[1]);
			player.TotalHours = std::stoi(data[2]);
			player.APM = std::stof(data[3]);
			player.SelectByHotkeys = std::stof(data[4]);
			player.AssignToHotkeys = std::stof(data[5]);
			player.UniqueHotkeys = std::stof(data[6]);
			player.MinimapAttacks = std::stof(data[7]);
			player.MinimapRightClicks = std::stof(data[8]);
			player.NumberOfPACs = std::stof(data[9]);
			player.GapBetweenPACs = std::stof(data[10]);
			player.ActionLatency = std::stof(data[11]);
			player.ActionsInPAC = std::stof(data[12]);
			player.TotalMapExplored = std::stof(data[13]);
			player.WorkersMade = std::stof(data[14]);
			player.UniqueUnitsMade = std::stof(data[15]);
			player.ComplexUnitsMade = std::stof(data[16]);
			player.ComplexAbilitiesUsed = std::stof(data[17]);

			//player.LeagueIndex = UNKNOWN;
			//player.ActualLeagueIndex = UNKNOWN;

			players.push_back(player);

			text += line + '\n';
		}
		myfile.close();
	}

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



struct orderAPM {
	inline bool operator() (const Player& player1, const Player& player2) {
		return (player1.APM < player2.APM);
	}
};

/*
 *
struct orderSelectByHotkeys {
	inline bool operator() (const Player& player1, const Player& player2) {
		return (player1.SelectByHotkeys < player2.SelectByHotkeys);
	}
};

struct orderAssignToHotkeys {
	inline bool operator() (const Player& player1, const Player& player2) {
		return (player1.AssignToHotkeys < player2.AssignToHotkeys);
	}
};

struct orderUniqueHotkeys {
	inline bool operator() (const Player& player1, const Player& player2) {
		return (player1.UniqueHotkeys < player2.UniqueHotkeys);
	}
};

struct orderNumberOfPACs {
	inline bool operator() (const Player& player1, const Player& player2) {
		return (player1.NumberOfPACs < player2.NumberOfPACs);
	}
};

struct orderGapBetweenPACs {
	inline bool operator() (const Player& player1, const Player& player2) {
		return (player1.GapBetweenPACs < player2.GapBetweenPACs);
	}
};

struct orderActionLatency {
	inline bool operator() (const Player& player1, const Player& player2) {
		return (player1.ActionLatency < player2.ActionLatency);
	}
};

struct orderTotalMapExplored {
	inline bool operator() (const Player& player1, const Player& player2) {
		return (player1.TotalMapExplored < player2.TotalMapExplored);
	}
};

struct orderWorkersMade {
	inline bool operator() (const Player& player1, const Player& player2) {
		return (player1.WorkersMade < player2.WorkersMade);
	}
};
*/

int MedianCalculator(std::vector<Player>* players)
{
	float median(0);

	if ((players->size() % 2) == 0) {
		//even
		Player player1(players->at((players->size() / 2) - 1));
		Player player2(players->at((players->size() / 2) + 1));
		median = (player1.APM + player2.APM) / 2;
	}
	else {
		// odd
		Player player(players->at(players->size() / 2));
		median = (player.APM);
	}
	return median;
}

/*
void AverageCalculator(std::vector<Player>* players, float& totalAPMSub, float& totalSelectByHotkeysSub, float& totalAssignToHotkeysSub, float& totalMinimapAttacksSub, float& totalNumberOfPACsSub, float& totalGapBetweenPACsSub, float& totalActionLatencySub, float& totalTotalMapExploredSub, float& totalWorkersMadeSub)
{
float totalAPM(0.0);
float totalSelectByHotkeys(0.0);
float totalAssignToHotkeys(0.0);
float totalMinimapAttacks(0.0);
float totalNumberOfPACs(0.0);
float totalGapBetweenPACs(0.0);
float totalActionLatency(0.0);
float totalTotalMapExplored(0.0);
float totalWorkersMade(0.0);

for (auto& player : *players) {
totalAPM += player.APM;
totalSelectByHotkeys += player.SelectByHotkeys;
totalAssignToHotkeys += player.AssignToHotkeys;
totalMinimapAttacks += player.MinimapAttacks;
totalNumberOfPACs += player.NumberOfPACs;
totalGapBetweenPACs += player.GapBetweenPACs;
totalActionLatency += player.ActionLatency;
totalTotalMapExplored += player.TotalMapExplored;
totalWorkersMade += player.WorkersMade;
}

float averageAPM(totalAPM / players->size());
float averageSelectByHotkeys(totalSelectByHotkeys / players->size());
float averageAssignToHotkeys(totalAssignToHotkeys / players->size());
float averageMinimapAttacks(totalMinimapAttacks / players->size());
float averageNumberOfPACs(totalNumberOfPACs / players->size());
float averageGapBetweenPACs(totalGapBetweenPACs / players->size());
float averageActionLatency(totalActionLatency / players->size());
float averageTotalMapExplored(totalTotalMapExplored / players->size());
float averageWorkersMade(totalWorkersMade / players->size());

std::cout << std::endl << "Average" << std::endl;

std::cout << averageAPM << std::endl;
std::cout << averageSelectByHotkeys << std::endl;
std::cout << averageAssignToHotkeys << std::endl;
std::cout << averageMinimapAttacks << std::endl;
std::cout << averageNumberOfPACs << std::endl;
std::cout << averageGapBetweenPACs << std::endl;
std::cout << averageActionLatency << std::endl;
std::cout << averageTotalMapExplored << std::endl;
std::cout << averageWorkersMade << std::endl;

totalAPMSub = 0.0;
totalSelectByHotkeysSub = 0.0;
totalAssignToHotkeysSub = 0.0;
totalMinimapAttacksSub = 0.0;
totalNumberOfPACsSub = 0.0;
totalGapBetweenPACsSub = 0.0;
totalActionLatencySub = 0.0;
totalTotalMapExploredSub = 0.0;
totalWorkersMadeSub = 0.0;

for (auto& player : *players) {
totalAPMSub += pow((player.APM - averageAPM), 2);
totalSelectByHotkeysSub += pow(player.SelectByHotkeys - averageSelectByHotkeys, 2);
totalAssignToHotkeysSub += pow(player.AssignToHotkeys - averageAssignToHotkeys, 2);
totalMinimapAttacksSub += pow(player.MinimapAttacks - averageMinimapAttacks, 2);
totalNumberOfPACsSub += pow(player.NumberOfPACs - averageNumberOfPACs, 2);
totalGapBetweenPACsSub += pow(player.GapBetweenPACs - averageGapBetweenPACs, 2);
totalActionLatencySub += pow(player.ActionLatency - averageActionLatency, 2);
totalTotalMapExploredSub += pow(player.TotalMapExplored - averageTotalMapExplored, 2);
totalWorkersMadeSub += pow(player.WorkersMade - averageWorkersMade, 2);
}
}


void StandardDeviation(std::vector<Player>* players, float totalAPMSub, float totalSelectByHotkeysSub, float totalAssignToHotkeysSub, float totalMinimapAttacksSub, float totalNumberOfPACsSub, float totalGapBetweenPACsSub, float totalActionLatencySub, float totalTotalMapExploredSub, float totalWorkersMadeSub)
{
std::cout << std::endl << "SUM of value minus average powered by 2" << std::endl;
std::cout << totalAPMSub << std::endl;
std::cout << totalSelectByHotkeysSub << std::endl;
std::cout << totalAssignToHotkeysSub << std::endl;
std::cout << totalMinimapAttacksSub << std::endl;
std::cout << totalNumberOfPACsSub << std::endl;
std::cout << totalGapBetweenPACsSub << std::endl;
std::cout << totalActionLatencySub << std::endl;
std::cout << totalTotalMapExploredSub << std::endl;
std::cout << totalWorkersMadeSub << std::endl;

int size = players->size();
std::cout << std::endl << "Size" << std::endl;
std::cout << size << std::endl;

float StandardDevAPM(sqrt((1.0 / size)*totalAPMSub));
float StandardDevSelectByHotkeys(sqrt((1.0 / size)*totalSelectByHotkeysSub));
float StandardDevAssignToHotkeys(sqrt((1.0 / size)*totalAssignToHotkeysSub));
float StandardDevMinimapAttacks(sqrt((1.0 / size)*totalMinimapAttacksSub));
float StandardDevNumberOfPACs(sqrt((1.0 / size)*totalNumberOfPACsSub));
float StandardDevGapBetweenPACs(sqrt((1.0 / size)*totalGapBetweenPACsSub));
float StandardDevActionLatency(sqrt((1.0 / size)*totalActionLatencySub));
float StandardDevTotalMapExplored(sqrt((1.0 / size)*totalTotalMapExploredSub));
float StandardDevWorkersMade(sqrt((1.0 / size)*totalWorkersMadeSub));

std::cout << std::endl << "Standard Deviation" << std::endl;

std::cout << StandardDevAPM << std::endl;
std::cout << StandardDevSelectByHotkeys << std::endl;
std::cout << StandardDevAssignToHotkeys << std::endl;
std::cout << StandardDevMinimapAttacks << std::endl;
std::cout << StandardDevNumberOfPACs << std::endl;
std::cout << StandardDevGapBetweenPACs << std::endl;
std::cout << StandardDevActionLatency << std::endl;
std::cout << StandardDevTotalMapExplored << std::endl;
std::cout << StandardDevWorkersMade << std::endl;
}
*/

void NoiseRemoval(std::vector<Player> *players)
{
	/*
	float totalAPMSub;
	float totalSelectByHotkeysSub;
	float totalAssignToHotkeysSub;
	float totalMinimapAttacksSub;
	float totalNumberOfPACsSub;
	float totalGapBetweenPACsSub;
	float totalActionLatencySub;
	float totalTotalMapExploredSub;
	float totalWorkersMadeSub;

	AverageCalculator(players, totalAPMSub, totalSelectByHotkeysSub, totalAssignToHotkeysSub, totalMinimapAttacksSub, totalNumberOfPACsSub, totalGapBetweenPACsSub, totalActionLatencySub, totalTotalMapExploredSub, totalWorkersMadeSub);
	StandardDeviation(players, totalAPMSub, totalSelectByHotkeysSub, totalAssignToHotkeysSub, totalMinimapAttacksSub, totalNumberOfPACsSub, totalGapBetweenPACsSub, totalActionLatencySub, totalTotalMapExploredSub, totalWorkersMadeSub);
	*/

	std::sort(players->begin(), players->end(), orderAPM());

	int quartileSize(players->size() / 8);

	std::vector<Player>::const_iterator first = players->begin();
	std::vector<Player>::const_iterator last = players->begin() + quartileSize;
	std::vector<Player> firstQ(first, last);
	int APMFirstQMedian(MedianCalculator(&firstQ));

	std::vector<Player>::const_iterator first2 = players->begin() + (players->size() - quartileSize);
	std::vector<Player>::const_iterator last2 = players->begin() + players->size();
	std::vector<Player> lastQ(first2, last2);
	int APMLastQMedian(MedianCalculator(&lastQ));
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
			float distance = sqrt(
						pow((nonRankPlayer.APM - rankedPlayer.APM), 2) +
						pow((nonRankPlayer.SelectByHotkeys - rankedPlayer.SelectByHotkeys), 2) +
						pow((nonRankPlayer.AssignToHotkeys - rankedPlayer.AssignToHotkeys), 2) +
						pow((nonRankPlayer.MinimapAttacks - rankedPlayer.MinimapAttacks), 2) +
						pow((nonRankPlayer.NumberOfPACs - rankedPlayer.NumberOfPACs), 2) +
						pow((nonRankPlayer.GapBetweenPACs - rankedPlayer.GapBetweenPACs), 2) +
						pow((nonRankPlayer.ActionLatency - rankedPlayer.ActionLatency), 2) +
						pow((nonRankPlayer.TotalMapExplored - rankedPlayer.TotalMapExplored), 2) +
						pow((nonRankPlayer.WorkersMade - rankedPlayer.WorkersMade), 2)
						);
			PairDistance pairDistance{};
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
		int highestLeague(0);
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

		for (int i = 0; i < 8; i++) {
			nonRankPlayer.Guess.push_back(((double)rank[i]) / k);
		}


		if (highestLeague == nonRankPlayer.LeagueIndex) {
			perfectMatch++;
		}
		else if (highestLeague == nonRankPlayer.LeagueIndex + 1 || highestLeague == nonRankPlayer.LeagueIndex - 1) {
			closeMatch++;
		}
		else {
			wrongMatch++;
		}
	}
	std::cout << std::endl << "k: " << k;
	std::cout << std::endl << "Perfect Match: " << perfectMatch << std::endl;
	std::cout << "Close Match: " << closeMatch << std::endl;
	std::cout << "Wrong Match: " << wrongMatch << std::endl;

	std::cout << "Perfect Match %: " << (perfectMatch / nonRankPlayers->size()) * 100 << std::endl;
	std::cout << "Close Match %: " << ((perfectMatch + closeMatch) / nonRankPlayers->size()) * 100 << std::endl;
}

void train(NN *nn, std::vector<Player> *training, std::vector<Player> *test)
{
	int run = 500;
	double learn_rate = 0.2;
	double score = 0;
	double succes_rate = 0.0;
	for (int j = 0; j < 500 / run; j++) {
		double perfectMatch(0);
		double closeMatch(0);
		double wrongMatch(0);
		learn_rate /= 2;

		for (int i = 0; i < run; i++)
		{
			for (auto& player : *training)
			{

				std::vector<double> input = player.toArray();


				std::vector<double> output = { 0, 0, 0, 0, 0 ,0 ,0 ,0 };
				int index = player.ActualLeagueIndex;
				output[index - 1] = 1;

				nn->train(&input, &output, learn_rate);

			}
		}

		score = 0;
		for (auto& player : *test)
		{
			std::vector<double> input = player.toArray();
			std::vector<double> guess = nn->run(&input);

			for (int i = 0; i<8; i++) {
				player.Guess[i] += guess[i];
				player.Guess[i] /= 2;
			}
			double val = 0;
			int index = 0;
			for (int i = 0; i<8; i++)
			{
				double tval = guess[i];

				if (tval > val)
				{
					index = i;
					val = tval;
				}
			}
			player.LeagueIndex = index + 1;
			if (player.ActualLeagueIndex == player.LeagueIndex) {
				perfectMatch++;
			}
			else if (player.ActualLeagueIndex == player.LeagueIndex + 1 || player.ActualLeagueIndex == player.LeagueIndex - 1) {
				closeMatch++;
			}
			else {
				wrongMatch++;
			}
		}
		succes_rate = score / test->size();
		//std::cout << "NN run: " << j * run << " the score is " << score  << " (" << succes_rate << ")" << std::endl;
		std::cout << std::endl << "NN" << std::endl;
		std::cout << "Perfect Match: " << perfectMatch << std::endl;
		std::cout << "Close Match: " << closeMatch << std::endl;
		std::cout << "Wrong Match: " << wrongMatch << std::endl;

		std::cout << "Perfect Match %: " << (perfectMatch / test->size()) * 100 << std::endl;
		std::cout << "Close Match %: " << ((perfectMatch + closeMatch) / test->size()) * 100 << std::endl;
	}

}

void nn_evaluation(NN *nn, std::vector<Player> *test)
{
	for (auto& player : *test)
	{
		std::vector<double> input = player.toArray();
		std::vector<double> guess = nn->run(&input);

		for (int i = 0; i<8; i++) {
			player.Guess[i] += guess[i];
			player.Guess[i] /= 2;
		}

	}
}

template<class Property>
void normaliz_attribute(std::vector<Player> * players, Property  prop)
{
	auto bounds = std::minmax_element(players->begin(), players->end(),
																		[prop](Player p1, Player p2) {return *prop(&p1) <= *prop(&p2); });

	double min = *prop(&*bounds.first);
	double max = *prop(&*bounds.second);

	for (auto& player : *players) {
		if (*prop(&player) == 0) { continue; }
		*prop(&player) = (*prop(&player) - min) / (max - min);
	}
}

void normaliz(std::vector<Player> *players)
{
	normaliz_attribute(players, [](Player* p) -> double* {return &(p->APM); });
	normaliz_attribute(players, [](Player* p) -> double* {return &(p->SelectByHotkeys); });
	normaliz_attribute(players, [](Player* p) -> double* {return &(p->AssignToHotkeys); });
	normaliz_attribute(players, [](Player* p) -> double* {return &(p->MinimapAttacks); });
	normaliz_attribute(players, [](Player* p) -> double* {return &(p->NumberOfPACs); });
	normaliz_attribute(players, [](Player* p) -> double* {return &(p->GapBetweenPACs); });
	normaliz_attribute(players, [](Player* p) -> double* {return &(p->ActionLatency); });
	normaliz_attribute(players, [](Player* p) -> double* {return &(p->TotalMapExplored); });
	normaliz_attribute(players, [](Player* p) -> double* {return &(p->WorkersMade); });
}

double evaluate(std::vector<Player> *players)
{
	double perfectMatch(0);
	double closeMatch(0);
	double wrongMatch(0);
	for (auto& player : *players) {
		double league(0);
		int league_index(0);

		int i(1);
		for (auto l : player.Guess) {
			if (l > league) {
				league = l;
				league_index = i;
			}
			++i;
		}

		if (player.ActualLeagueIndex == league_index) {
			perfectMatch++;
		}
		else if (player.ActualLeagueIndex == league_index + 1 || player.ActualLeagueIndex == league_index - 1) {
			closeMatch++;
		}
		else {
			wrongMatch++;
		}
	}

	std::cout << std::endl << "Moyen des 2 algo" << std::endl;
	std::cout << "Perfect Match: " << perfectMatch << std::endl;
	std::cout << "Close Match: " << closeMatch << std::endl;
	std::cout << "Wrong Match: " << wrongMatch << std::endl;

	std::cout << "Perfect Match %: " << (perfectMatch / players->size()) * 100 << std::endl;
	std::cout << "Close Match %: " << ((perfectMatch + closeMatch) / players->size()) * 100 << std::endl;

	return (perfectMatch / players->size());
}

void output(std::vector<Player> *players)
{
	std::cout << std::endl << "Output : " << std::endl;

	for (auto&& player : *players) {
		double league(0);
		int league_index(0);

		int i(1);
		for (auto l : player.Guess) {
			if (l > league) {
				league = l;
				league_index = i;
			}
			++i;
		}

		std::cout << "Id : " << player.GameID << " League : "<< league_index << std::endl;
	}


}

int main()
{
	std::cout << "Hello World!" << std::endl;
	std::string path("data.csv");
	std::string evalPath("Evaluations.csv");	//Added recently Yan

	std::vector<Player> players = read_csv(path);
	std::vector<Player> evalPlayers = read_csv_evaluation(evalPath);	//Added recently Yan

	int nb_fold = 5;

	//normaliz(&players);	// Commented because cant compile on my computer somehow !
	//normaliz(&evalPlayers);

	std::srand(std::time(nullptr));
	NN nn(9, 1, 8, 8);

	double score(0);

	//Train
	for (int i = 0; i < nb_fold; ++i)
	{
		NN nn2(9, 1, 8, 8);


		std::vector<Player> training;
		std::vector<Player> test;
		split_data(&players, nb_fold, i, &training, &test);

		//http://www.wikihow.com/Calculate-Outliers
		// NoiseRemoval(&training);

		std::cout << std::endl << "KNN Algorithm Fold: " << i << std::endl;

		KNNAlgorithm(&training, &test, 8);

		train(&nn2, &training, &test);

		double score2 = evaluate(&test);
		if (score2 > score){
			score = score2;
			nn = nn2;
		}

		std::cout << std::endl;
	}

	//evaluation
	KNNAlgorithm(&players, &evalPlayers, 8);
	nn_evaluation(&nn, &evalPlayers);

	output(&evalPlayers);


	return 0;
}
