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
  public :
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

};


std::vector<std::string> split(const std::string &s, char c) {
    std::string buff{""};
    std::vector<std::string> v;

        for(auto n:s)
        {
            if(n != c) buff+=n;
            else if(n == c && buff != "")
            {
                v.push_back(buff);
                buff = "";
            }
        }
        if(buff != "")
            v.push_back(buff);

        return v;
}

std::string remove_char( std::string str, char ch )
{
    // remove all occurrences of char ch from str
    str.erase( std::remove( str.begin(), str.end(), ch ), str.end() ) ;
    return str ;
}

std::vector<Player> read_csv(const std::string path)
{
  std::vector<Player> players;
  std::string text("");
  std::string line;
  std::ifstream myfile (path);
  if (myfile.is_open())
  {
    getline (myfile,line);
    while ( getline (myfile,line) )
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
      
      players.push_back(player);

      text += line +  '\n';
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


  if( i > 0)
  {
    std::vector<Player>::const_iterator first = players->begin();
    std::vector<Player>::const_iterator last = players->begin() + nb_element * i;
    //*training = std::vector<Player>(first, last);
    training->insert(training->end(), first, last);

  }

  std::vector<Player>::const_iterator first = players->begin() + nb_element * i;
  std::vector<Player>::const_iterator last = players->begin() + nb_element * (i + 1);
  *test = std::vector<Player>(first, last);

  if ( i < nb_fold - 1)
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

void train(std::vector<Player> *players)
{

}

void rate(std::vector<Player> *players)
{
  int score = 0;
  for (auto& player : *players)
    score += player.LeagueIndex == player.LeagueIndex;
  std::cout << "the score is " << score << std::endl;
}

int main() {
  std::cout << "Hello World!" << std::endl;


  std::string path("data.csv");
  //String path = chose_file()
  //std::string file = read_file(path );
  std::vector<Player> players  = read_csv(path);
  int nb_fold = 5;
  for(int i = 0; i < nb_fold; ++i)
  {
    std::vector<Player> training;
    std::vector<Player> test;
    split_data(&players, nb_fold, i, &training, &test);

    trim(&training);

    train(&training);
    //validate(&test);

    rate(&players);
  }



  return 0;
}
