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
    double score;

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
        if (n != c) { buff += n;
        } else if (n == c && buff != "")
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

void NoiseRemoval(std::vector<Player> *players)
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

    float totalAPMSub(0.0);
    float totalSelectByHotkeysSub(0.0);
    float totalAssignToHotkeysSub(0.0);
    float totalMinimapAttacksSub(0.0);
    float totalNumberOfPACsSub(0.0);
    float totalGapBetweenPACsSub(0.0);
    float totalActionLatencySub(0.0);
    float totalTotalMapExploredSub(0.0);
    float totalWorkersMadeSub(0.0);

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


    float size(players->size());
    std::cout << std::endl << "Size" << std::endl;
    std::cout << size << std::endl;

    float StandardDevAPM(sqrt((1.0 /size)*totalAPMSub));
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
                        pow((nonRankPlayer.APM - rankedPlayer.APM),2) +
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

        //std::cout << "PLAYER:" << nonRankPlayer.GameID << " League:" << highestLeague << " Real League:" << nonRankPlayer.LeagueIndex << std::endl;

        if (highestLeague == nonRankPlayer.LeagueIndex) {
            perfectMatch++;
        } else if (highestLeague == nonRankPlayer.LeagueIndex + 1 || highestLeague == nonRankPlayer.LeagueIndex - 1) {
            closeMatch++;
        } else {
            wrongMatch++;
        }
    }

    std::cout << "Perfect Match: " << perfectMatch << std::endl;
    std::cout << "Close Match: " << closeMatch << std::endl;
    std::cout << "Wrong Match: " << wrongMatch << std::endl;

    std::cout << "Perfect Match %: " << (perfectMatch / nonRankPlayers->size()) * 100 << std::endl;
    std::cout << "Close Match %: " << ((perfectMatch + closeMatch) / nonRankPlayers->size()) * 100 << std::endl;
}

void train(std::vector<Player> *training, std::vector<Player> *test)
{

    std::srand(std::time(nullptr));
    NN nn(9, 1, 8, 8);
    int run = 50;
    double learn_rate = 0.01;
    double score = 0;
    double succes_rate = 0.0;
    for (int j =0; j < 20000 / run; j++) {
        //        if (succes_rate > 0.28f)
        //            learn_rate = 0.1;
        //        if (succes_rate > 0.35f)
        //            learn_rate = 0.05;
        //        if (succes_rate > 0.40f)
        //            learn_rate = 0.01;
        //        if (succes_rate > 0.45f)
        //            learn_rate = 0.005;
        //        if (succes_rate > 0.50f)
        //            learn_rate = 0.001;

        for (int i =0; i < run; i++)
        {
            for (auto& player : *training)
            {

                std::vector<double> input = player.toArray();


                std::vector<double> output = {0, 0, 0, 0, 0 ,0 ,0 ,0};
                int index = player.ActualLeagueIndex;
                output[index - 1] = 1;

                nn.train(&input, &output, learn_rate);

            }
        }

        score = 0;
        for (auto& player : *test)
        {
            std::vector<double> input = player.toArray();
            std::vector<double> guess = nn.run(&input);

            double val=0;
            int index=0;
            for (int i= 0; i<8; i++)
            {
                double tval = guess[i];

                if (tval > val)
                {
                    index = i;
                    val = tval;
                }
            }
            player.LeagueIndex = index + 1;
            score += static_cast<double>(player.LeagueIndex == player.ActualLeagueIndex);
            //display result
            //            std::cout << " DESIRED OUTPUT: " << player.ActualLeagueIndex << " NET RESULT: "
            //                      << player.LeagueIndex << std::endl;
        }
        succes_rate = score / test->size();
        std::cout << "NN run: " << j * run << " the score is " << score  << " (" << succes_rate << ")" << std::endl;
    }

    //nn_free(nn);
}

template<class Property>
void normaliz_attribute(std::vector<Player> * players, Property  prop )
{
    auto bounds = std::minmax_element(players->begin(), players->end(),
                                      [prop](Player p1, Player p2) {return *prop(&p1) <= *prop(&p2);});

    for (auto& player : *players) {
        if (*prop(&player) == 0) { continue;
        }
        double min = *prop(&*bounds.first);
        double max = *prop(&*bounds.second);
        *prop(&player) = (*prop(&player) - min) / (max - min);
    }
}

void normaliz(std::vector<Player> *players)
{
    normaliz_attribute(players,  [](Player* p) -> double* {return &(p->APM);});
    normaliz_attribute(players,  [](Player* p) -> double* {return &(p->SelectByHotkeys);});
    normaliz_attribute(players,  [](Player* p) -> double* {return &(p->AssignToHotkeys);});
    normaliz_attribute(players,  [](Player* p) -> double* {return &(p->MinimapAttacks);});
    normaliz_attribute(players,  [](Player* p) -> double* {return &(p->NumberOfPACs);});
    normaliz_attribute(players,  [](Player* p) -> double* {return &(p->GapBetweenPACs);});
    normaliz_attribute(players,  [](Player* p) -> double* {return &(p->ActionLatency);});
    normaliz_attribute(players,  [](Player* p) -> double* {return &(p->TotalMapExplored);});
    normaliz_attribute(players,  [](Player* p) -> double* {return &(p->WorkersMade);});
}

int main()
{
    std::cout << "Hello World!" << std::endl;
    std::string path("data.csv");
    std::vector<Player> players = read_csv(path);
    int nb_fold = 5;

    normaliz(&players);

    for (int i = 0; i < nb_fold; ++i)
    {
        std::vector<Player> training;
        std::vector<Player> test;
        split_data(&players, nb_fold, i, &training, &test);


        NoiseRemoval(&training);

        std::cout << std::endl << "KNN Algorithm Fold: " << i << std::endl;
        KNNAlgorithm(&training, &test, 16);

        train(&training, &test);

        std::cout << std::endl;
    }

    return 0;
}
