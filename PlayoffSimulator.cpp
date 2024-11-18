// PlayoffSimulator.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file


#include <array>
#include <iostream>
#include <cmath>
#include <array>
#include <vector>
#include <string>
#include <utility>
#include <algorithm>

static std::string Bill = "Bill";
static std::string Matt = "Matt";
static std::string Caleb = "Caleb";
static std::string Austin = "Austin";
static std::string Drew = "Drew";
static std::string Jerry = "Jerry";
static  std::string Sam = "Sam";
static std::string John = "John";
static std::string Scott = "Scott";
static  std::string Mike = "Mike";

class Team
{
public:
	std::string name;
	int basewins = 0;
	int baseloses = 0;
	int baseties = 0;
	float pointsFor = 0.0f;
	int simwins = 0;
	int simloses = 0;
	int simties = 0;
	Team() = default;
	Team(std::string inname, int wins, int loses, int ties, float InpointsFor)
	{
		name = inname;
		basewins = wins;
		baseloses = loses;
		baseties = ties;
		pointsFor = InpointsFor;
	};
	Team(std::string inname, int wins, int loses, float InpointsFor)
	{
		name = inname;
		basewins = wins;
		baseloses = loses;
		baseties = 0;
		pointsFor = InpointsFor;
	};

	int GetWins() const
	{
		return basewins + simwins;
	};
	int GetLoses() const
	{
		return baseloses + simloses;
	};
	int GetTies() const
	{
		return baseties + simties;
	};

};

class League
{
public:
	std::vector<Team*> teams;
	std::vector<std::pair<Team*, Team*>> schedule;
	int teamsInPlayoff = 4;
	int minWins = -1;
	League()
	{

	}

	~League()
	{
		for (int i = teams.size() - 1; i >= 0; --i)
		{
			delete teams[i];
		}
	}

	void RecordOutcome(int index)
	{
		int game = index / 2;
		if (index % 2 == 0)
		{
			schedule[game].first->simwins++;
		}
		else
		{
			schedule[game].second->simwins++;
		}
	}

	void addMatchup(std::string one, std::string two)
	{
		Team* oneT;
		Team* twoT;
		for (int i = 0; i < teams.size(); ++i)
		{
			if (teams[i]->name == one)
			{
				oneT = teams[i];
			}
			if (teams[i]->name == two)
			{
				twoT = teams[i];
			}
		}
		schedule.push_back({ oneT, twoT });
	};

	void ResetSim()
	{
		for (int i = 0; i < teams.size(); ++i)
		{
			teams[i]->simwins = 0;
			teams[i]->simties = 0;
			teams[i]->simloses = 0;
		}
	};

	bool GetPlayoffTeams(std::string teamToCheck)
	{
		std::vector<Team*> playoffTeams;
		Team* badTeam = new Team("bad", 0, 15, 0);
		for (int i = 0; i < teamsInPlayoff; ++i)
		{
			playoffTeams.push_back(badTeam);
		}

		for (int i = 0; i < teams.size(); ++i)
		{
			if (minWins != -1 && teams[i]->GetWins() < minWins)
			{
				continue;
			}
			for (int j = 0; j < playoffTeams.size(); ++j)
			{

				if (teams[i]->GetWins() == playoffTeams[j]->GetWins())
				{
					if (teams[i]->GetTies() > playoffTeams[j]->GetTies())
					{
						playoffTeams.insert(playoffTeams.begin() + j, teams[i]);
						playoffTeams.pop_back();
						break;
					}
				}

				if (teams[i]->GetWins() > playoffTeams[j]->GetWins())
				{
					playoffTeams.insert(playoffTeams.begin() + j, teams[i]);
					playoffTeams.pop_back();
					break;
				}
			}

		}

		for (int i = 0; i < playoffTeams.size(); ++i)
		{
			if (playoffTeams[i]->name == teamToCheck)
			{
				delete badTeam;
				return true;
			}
		}

		delete badTeam;
		return false;
	};

	void SetMinWinsIn()
	{
		std::vector<int> wins;
		for (int i = 0; i < teamsInPlayoff; ++i)
		{
			wins.push_back(0);
		}

		for (auto team : teams)
		{
			for (int i = 0; i < wins.size(); ++i)
			{
				if (team->basewins > wins[i])
				{
					wins[i] = team->basewins;
					break;
				}
			}
		}

		if (wins.empty())
		{
			return;
		}
		minWins = wins[0];
		for (int i = 1; i < wins.size(); ++i)
		{
			if (wins[i] < minWins)
			{
				minWins = wins[i];
			}
		}
	}

	Team* GetTeam(std::string name) const
	{
		for (auto team : teams)
		{
			if (team->name == name)
			{
				return team;
			}
		}
		return nullptr;
	}

	void SwampMatchups(std::string team)
	{
		for (auto& matchup : schedule)
		{
			if (matchup.second->name == team)
			{
				Team* temp = matchup.first;
				matchup.first = matchup.second;
				matchup.second = temp;
			}
		}
	}

	void ReorderTeams(std::string teamName, bool considerPointsFor)
	{
		if (!considerPointsFor)
		{
			if (teams.empty() || teams[0]->name == teamName)
			{
				return;
			}

			for (int i = 0; i < teams.size(); ++i)
			{
				if (teams[i]->name == teamName)
				{
					std::iter_swap(teams.begin(), teams.begin() + i);
					return;
				}
			}
			return;
		}

		for (int j = 0; j < teams.size(); ++j)
		{
			for (int i = 0; i < teams.size() - 1; ++i)
			{
				if (teams[i]->pointsFor < teams[i + 1]->pointsFor)
				{
					std::iter_swap(teams.begin() + i, teams.begin() + i + 1);
				}
			}
		}


	}

	void Simulate(std::string teamToCheck, bool considerPointsFor = false)
	{
		int numMakes = 0;
		std::vector<int> seeds;
		int numGames = schedule.size();
		int numPosiblities = std::pow(2, numGames); //33554431

		SwampMatchups(teamToCheck);
		ReorderTeams(teamToCheck, considerPointsFor);
		SetMinWinsIn();

		for (int scen = 0; scen < numPosiblities; ++scen)
		{
			for (int bit = 0; bit < numGames; ++bit)
			{
				int bitI = scen >> bit;
				int bitO = bitI & 1;
				if (bitO == 1)
				{
					schedule[bit].first->simwins++;
					schedule[bit].second->simloses++;

				}
				else
				{
					schedule[bit].second->simwins++;
					schedule[bit].first->simloses++;
				}
			}
			if (GetPlayoffTeams(teamToCheck))
			{
				std::cout << teamToCheck << " made it with seed: " << std::to_string(scen) << std::endl;
				numMakes++;
				seeds.push_back(scen);

				//for (int i = 0; i < teams.size(); ++i)
				//{
				//	std::cout << teams[i]->name << " went " << std::to_string(teams[i]->GetWins()) << "-" << std::to_string(teams[i]->GetLoses()) << "-" << std::to_string(teams[i]->GetTies()) << std::endl;
				//}
			}
			ResetSim();
		}

		std::vector<std::pair<int, int>> winFrequencyCount;
		std::vector<std::pair<int, int>> lossFrequencyCount;

		for (int j = 0; j < schedule.size(); ++j)
		{
			winFrequencyCount.push_back({ j, 0 });
			lossFrequencyCount.push_back({ j, 0 });
		}

		std::cout << "Number of possibilities (hex) " << std::hex << numPosiblities - 1 << std::endl;
		std::cout << "Number of possibilities " << std::hex << std::to_string(numPosiblities) << std::endl;

		if (seeds.size() > 0)
		{
			int mandatoryWin = numPosiblities - 1;
			int mandatoryLoss = 0;
			for (int i = 0; i < seeds.size(); ++i)
			{
				mandatoryWin = mandatoryWin & seeds[i];
				mandatoryLoss = mandatoryLoss | seeds[i];
				//mandatoryWin = mandatoryWin ^ seeds[i];
				for (int j = 0; j < schedule.size(); ++j)
				{
					int gameResult = (seeds[i] >> j) & 1;
					if (gameResult == 1)
					{
						winFrequencyCount[j].second++;
					}
					else
					{
						lossFrequencyCount[j].second++;
					}
				}
			}

			for (int i = 0; i < numGames; ++i)
			{
				int wingame = (mandatoryWin >> i) & 1;
				if (wingame == 1)
				{
					std::cout << schedule[i].first->name << " must beat " << schedule[i].second->name << std::endl;
				}

				int lossgame = ~(mandatoryLoss >> i) & 1;
				if (lossgame == 1)
				{
					std::cout << schedule[i].second->name << " must beat " << schedule[i].first->name << std::endl;
				}
			}
		}

		std::cout << teamToCheck << " makes it in " << std::to_string(numMakes) << " scenarios" << std::endl;
		float chance = 100.0f * (float)numMakes / (float)numPosiblities;
		std::cout << teamToCheck << " makes it with a chance of " << std::to_string(chance) << "%" << std::endl;

		std::sort(lossFrequencyCount.begin(), lossFrequencyCount.end(), [](std::pair<int, int> a, std::pair<int, int> b)
			{
				return a.second > b.second;
			});
		std::sort(winFrequencyCount.begin(), winFrequencyCount.end(), [](std::pair<int, int> a, std::pair<int, int> b)
			{
				return a.second > b.second;
			});

		std::cout << "Most frequent games to win to go forward" << std::endl;
		for (int i = 0; i < winFrequencyCount.size(); ++i)
		{
			std::pair<Team*, Team*> matchup = schedule[winFrequencyCount[i].first];
			std::cout << matchup.first->name << " beats " << matchup.second->name << " in " << std::to_string(winFrequencyCount[i].second) << " outcomes" << std::endl;
		}

		std::cout << "Most frequent games to lose to go forward" << std::endl;
		for (int i = 0; i < lossFrequencyCount.size(); ++i)
		{
			std::pair<Team*, Team*> matchup = schedule[lossFrequencyCount[i].first];
			std::cout << matchup.second->name << " beats " << matchup.first->name << " in " << std::to_string(lossFrequencyCount[i].second) << " outcomes" << std::endl;
		}
	}
};




void Babes()
{
	std::string Bill = "Bill";
	std::string Matt = "Matt";
	std::string Caleb = "Caleb";
	std::string Austin = "Austin";
	std::string Drew = "Drew";
	std::string Jerry = "Jerry";
	std::string Sam = "Sam";
	std::string John = "John";
	std::string Scott = "Scott";
	std::string Mike = "Mike";

	League league;

	league.teamsInPlayoff = 4;

	league.teams.push_back(new Team{ Bill, 2, 8, 0.0f });
	league.teams.push_back(new Team{ Matt, 6, 4, 0.0f });
	league.teams.push_back(new Team{ Caleb, 9, 1, 0.0f });
	league.teams.push_back(new Team{ Austin, 8, 2, 0.0f });
	league.teams.push_back(new Team{ Drew, 6, 4, 0.0f });
	league.teams.push_back(new Team{ Jerry, 5,5, 0.0f });
	league.teams.push_back(new Team{ Sam, 3, 6, 1, 0.0f });
	league.teams.push_back(new Team{ John, 4, 6 , 0.0f });
	league.teams.push_back(new Team{ Scott, 3,7, 0.0f });
	league.teams.push_back(new Team{ Mike, 3, 6, 1, 0.0f });

	//11
	league.addMatchup(Matt, John);
	league.addMatchup(Scott, Drew);
	league.addMatchup(Mike, Austin);
	league.addMatchup(Caleb, Sam);
	league.addMatchup(Bill, Jerry);
	//12
	league.addMatchup(Matt, Scott);
	league.addMatchup(John, Mike);
	league.addMatchup(Bill, Drew);
	league.addMatchup(Austin, Caleb);
	league.addMatchup(Sam, Jerry);
	//13
	league.addMatchup(Matt, Mike);
	league.addMatchup(Bill, Scott);
	league.addMatchup(Caleb, John);
	league.addMatchup(Drew, Sam);
	league.addMatchup(Austin, Jerry);
	//14
	league.addMatchup(Bill, Matt);
	league.addMatchup(Mike, Caleb);
	league.addMatchup(Scott, Sam);
	league.addMatchup(John, Jerry);
	league.addMatchup(Drew, Austin);
	//15
	league.addMatchup(Matt, Caleb);
	league.addMatchup(Bill, Sam);
	league.addMatchup(Jerry, Mike);
	league.addMatchup(Austin, Scott);
	league.addMatchup(Drew, John);

	league.Simulate(Bill);
}

void Units()
{
	std::string Will = "Will";
	std::string Nick = "Nick";
	std::string Matt = "Matt";
	std::string Kara = "Kara";
	std::string Andrew = "Andrew";
	std::string Chris = "Chris";
	std::string Fart = "Fart";
	std::string Holland = "Holland";
	std::string Lorrin = "Lorrin";
	std::string Zack = "Zack";
	std::string Jimmy = "Jimmy";
	std::string Hank = "Hank";

	League league;

	league.teamsInPlayoff = 6;

	//put the team you want to calculate at the top of the list to give them the benefit of the doubt in ties
	league.teams.push_back(new Team{ Nick, 8, 2, 2215.74f });
	league.teams.push_back(new Team{ Matt, 7, 3, 2228.5f });
	league.teams.push_back(new Team{ Hank, 7, 3, 1982.44f });
	league.teams.push_back(new Team{ Kara, 7, 3, 1910.02f });
	league.teams.push_back(new Team{ Chris, 6, 4, 2058.7f });
	league.teams.push_back(new Team{ Jimmy, 5, 5, 2103.66f });
	league.teams.push_back(new Team{ Holland, 5, 5, 2033.5f });
	league.teams.push_back(new Team{ Fart, 5, 5, 1968.82f });
	league.teams.push_back(new Team{ Zack, 4, 6, 1944.74f });
	league.teams.push_back(new Team{ Will, 3, 7, 1993.3f });
	league.teams.push_back(new Team{ Andrew, 2, 8, 1897.44f });
	league.teams.push_back(new Team{ Lorrin, 1, 9, 1655.7f });

	auto WillTeam = league.GetTeam(Will);
	WillTeam->basewins++;

	auto HollandTeam = league.GetTeam(Holland);
	HollandTeam->basewins++;

	auto AndrewTeam = league.GetTeam(Andrew);
	AndrewTeam->baseloses++;

	auto MattTeam = league.GetTeam(Matt);
	MattTeam->basewins++;

	auto JimmyTeam = league.GetTeam(Jimmy);
	JimmyTeam->basewins++;

	auto ZackTeam = league.GetTeam(Zack);
	ZackTeam->baseloses++;

	auto HankTeam = league.GetTeam(Hank);
	HankTeam->baseloses++;

	auto LorrinTeam = league.GetTeam(Lorrin);
	LorrinTeam->baseloses++;

	//11
	//league.addMatchup(Matt, Hank);
	league.addMatchup(Nick, Kara);
	//league.addMatchup(Will, Andrew);
	league.addMatchup(Chris, Fart);
	//league.addMatchup(Holland, Lorrin);
	//league.addMatchup(Zack, Jimmy);
	//12
	league.addMatchup(Matt, Nick);
	league.addMatchup(Hank, Kara);
	league.addMatchup(Will, Chris);
	league.addMatchup(Holland, Jimmy);
	league.addMatchup(Andrew, Fart);
	league.addMatchup(Lorrin, Zack);
	//13
	league.addMatchup(Matt, Lorrin);
	league.addMatchup(Will, Hank);
	league.addMatchup(Holland, Kara);
	league.addMatchup(Chris, Jimmy);
	league.addMatchup(Zack, Fart);
	league.addMatchup(Andrew, Nick);
	//14
	league.addMatchup(Matt, Andrew);
	league.addMatchup(Chris, Hank);
	league.addMatchup(Kara, Jimmy);
	league.addMatchup(Will, Holland);
	league.addMatchup(Lorrin, Fart);
	league.addMatchup(Zack, Nick);


	league.Simulate(Matt, true);
}

void TestLeague()
{
	std::string Will = "Will";
	std::string Nick = "Nick";

	League league;

	league.teamsInPlayoff = 1;

	league.teams.push_back(new Team{ Nick, 8, 2, 2215.74f });
	league.teams.push_back(new Team{ Will, 8, 3, 2228.5f });

	league.addMatchup(Nick, Will);

	league.Simulate(Nick, true);
}

int main()
{
	Units();
}