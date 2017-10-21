#include "SFML\Graphics.hpp"

#include "Gameplay\Game.h"
#include "Gameplay\Team.h"
#include "Map\Map.h"
#include "main.h"

#include <algorithm>
#include <iostream>
#include <random>

sf::RenderWindow s_window(sf::VideoMode(1600, 900), "NFL Conquest");

CountyMap counties;
std::map<CountyId, TeamId> homeCounties;
int main()
{
	for (CountyId c = 1; c < 321; ++c)
	{
		counties.AddCounty(County(nullptr, { 0,0 }, c), { (c < 321) ? c + 1 : c - 1, (c > 1) ? c - 1 : c + 1 });
	}

	std::vector<sf::Color> teamColors = 
	{
		{ 0,   0,   0, 255 },
		{ 64,  0,   0, 255 },
		{ 128, 0,   0, 255 },
		{ 192, 0,   0, 255 },
		{ 0,   64,  0, 255 },
		{ 64,  64,  0, 255 },
		{ 128, 64,  0, 255 },
		{ 192, 64,  0, 255 },

		{ 0,   128, 0, 255 },
		{ 64,  128, 0, 255 },
		{ 128, 128, 0, 255 },
		{ 192, 128, 0, 255 },
		{ 0,   192, 0, 255 },
		{ 64,  192, 0, 255 },
		{ 128, 192, 0, 255 },
		{ 192, 192, 0, 255 },

		{ 0,   0,   128, 255 },
		{ 64,  0,   128, 255 },
		{ 128, 0,   128, 255 },
		{ 192, 0,   128, 255 },
		{ 0,   64,  128, 255 },
		{ 64,  64,  128, 255 },
		{ 128, 64,  128, 255 },
		{ 192, 64,  128, 255 },

		{ 0,   128, 255, 255 },
		{ 64,  128, 255, 255 },
		{ 128, 128, 255, 255 },
		{ 192, 128, 255, 255 },
		{ 0,   192, 255, 255 },
		{ 64,  192, 255, 255 },
		{ 128, 192, 255, 255 },
		{ 192, 192, 255, 255 },
	};
	std::random_device rd;
	std::mt19937 g(rd());
	std::shuffle(teamColors.begin(), teamColors.end(), g);

	std::map<TeamId, Team> teams;
	std::vector<TeamId> teamIds;
	for (TeamId t = 1; t < 33; ++t)
	{
		CountyId homeCounty = 10 * t - 5;
		homeCounties[homeCounty] = t;
		teams.emplace(t, Team(t, Territory(homeCounty)));
		teamIds.push_back(t);
		auto&& team = teams[t];
		team.m_territory.m_currentHomeSubterritory = Subterritory{};
		team.m_territory.ClaimHome();
		team.m_teamColor = teamColors[t - 1];
		for (CountyId c = 1; c < 11; ++c)
		{
			auto countyId = c + (10 * (t - 1));
			team.m_territory.m_defaultHomeSubterritory.AddCounty(countyId);
			team.m_territory.m_currentHomeSubterritory->AddCounty(countyId);
		}
		team.m_territory.m_currentHomeSubterritory->ClaimCounties(counties);
	}

	std::map<TeamId, Territory> startPositions = CacheTerritories(teams);
	struct GameWeek
	{
		std::vector<Game> m_games;
		std::map<TeamId, Territory> m_cachedTerritories;
	};

	using Season = std::vector<GameWeek>;

	Season season;

	for (auto weekI = 0; weekI < 16; ++weekI)
	{
		season.push_back({});
		auto& week = season.back();
		std::shuffle(teamIds.begin(), teamIds.end(), g);

		for (auto gameI = 0; gameI < 16; ++gameI)
		{
			week.m_games.push_back({ teamIds[gameI * 2], teamIds[(gameI * 2) + 1], Game::Result::UNDECIDED });
		}
	}

	std::cout << "-----------------------------------------" << std::endl;
	auto PIXEL_SIZE = 5;
	auto yPos = 1;
	for (auto& territory : startPositions)
	{
		auto&& team = teams[territory.first];
		if (territory.second.m_currentHomeSubterritory)
		{
			for (auto&& county : territory.second.m_currentHomeSubterritory->GetCounties())
			{
				drawCounty(PIXEL_SIZE, team, county, 0);
			}
		}
		for (auto&& sub : territory.second.m_earnedTerritories)
		{
			for (auto&& county : sub.GetCounties())
			{
				drawCounty(PIXEL_SIZE, team, county, 0);
			}
		}
	}
	for (auto&& week : season)
	{
		for (auto& game : week.m_games)
		{
			auto random = rd() % 60;
			if (random < 30) { 
				game.m_result = Game::Result::HOME_WIN; 
			}
			else if (random < 59) {
				game.m_result = Game::Result::AWAY_WIN; 
			}
			else {
				game.m_result = Game::Result::TIE;
			}
			std::cout << game.m_awayTeam << " @ " << game.m_homeTeam << ": " << static_cast<int>(game.m_result) << std::endl;
			ProcessGameResult(game, teams, counties);
		}
		week.m_cachedTerritories = CacheTerritories(teams);
		for (auto& territory : week.m_cachedTerritories)
		{
			auto&& team = teams[territory.first];
			if (territory.second.m_currentHomeSubterritory)
			{
				for (auto&& county : territory.second.m_currentHomeSubterritory->GetCounties())
				{
					drawCounty(PIXEL_SIZE, team, county, yPos);
				}
			}
			for (auto&& sub : territory.second.m_earnedTerritories)
			{
				for (auto&& county : sub.GetCounties())
				{
					drawCounty(PIXEL_SIZE, team, county, yPos);
				}
			}
		}
		++yPos;
		std::cout << "-----------------------------------------" << std::endl;
	}

	s_window.display();
	sf::Event event;
	while (true)
	{
		while (s_window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				return 0;
			}
		}
	}

	return 0; 
}

void drawCounty(int PIXEL_SIZE, Team & team, const CountyId & county, int yPos)
{
	sf::RectangleShape square({ static_cast<float>(PIXEL_SIZE), static_cast<float>(PIXEL_SIZE * 10) });
	square.setFillColor(team.m_teamColor);
	square.setPosition(static_cast<float>((county - 1) * PIXEL_SIZE), static_cast<float>(yPos * 10 * PIXEL_SIZE));
	s_window.draw(square);
}

std::map<TeamId, Territory> CacheTerritories(std::map<TeamId, Team> &teams)
{
	std::map<TeamId, Territory> positions;
	for (auto&& team : teams)
	{
		positions.insert({ team.first, team.second.m_territory });
	}
	return positions;
}

void ProcessGameResult(Game &game, std::map<TeamId, Team>& teams, CountyMap &counties)
{
	auto&& homeTeam = teams[game.m_homeTeam];
	auto&& awayTeam = teams[game.m_awayTeam];
	switch (game.m_result)
	{
	case Game::Result::TIE:
	case Game::Result::UNDECIDED:
		// No territory change
		break;
	case Game::Result::HOME_WIN:
		ProcessHomeWin(homeTeam, counties);
		break;
	case Game::Result::AWAY_WIN:
		ProcessAwayWin(homeTeam, awayTeam);
		break;
	}
}

void ProcessAwayWin(Team & homeTeam, Team & awayTeam)
{
	// Away team takes subterritories from Home Team
	// # of territories = Min(Away team territory count, Home team territory count)
	// Priority of territory steal is opposite of order home team gained them
	// With territory containing home stadium always being last

	auto availableSubterritories = homeTeam.m_territory.GetTerritoryCount();
	auto ownedSubterritories = awayTeam.m_territory.GetTerritoryCount();
	auto numSubterritoriesToSteal = std::min(availableSubterritories, ownedSubterritories);

	for (auto i = 0; i < numSubterritoriesToSteal; ++i)
	{
		if (homeTeam.m_territory.m_earnedTerritories.size())
		{
			awayTeam.m_territory.m_earnedTerritories.push_back(homeTeam.m_territory.m_earnedTerritories.back());
			awayTeam.m_territory.ClaimTerritories(counties);
			homeTeam.m_territory.m_earnedTerritories.pop_back();
			counties.Validate();
			std::cout << "";
		}
		else
		{
			awayTeam.m_territory.m_earnedTerritories.push_back(*homeTeam.m_territory.m_currentHomeSubterritory);
			awayTeam.m_territory.ClaimTerritories(counties);
			homeTeam.m_territory.m_currentHomeSubterritory.reset();
			counties.Validate();
			std::cout << "";
		}
	}
}

void ProcessHomeWin(Team & homeTeam, CountyMap & counties)
{
	// Home team keeps all current territory
	// If home team has none of its original territory, reclaim stadium and one ring of counties around it
	// If home team has its home stadium as territory, expand out one ring, limited by initial home territory
	// Optional: Allow expansion past initial home territory, excluding any other team's home stadium
	bool allowHomeExpansion = false;
	std::set<CountyId> countiesToClaim;
	if (homeTeam.m_territory.m_currentHomeSubterritory)
	{
		// Own home stadium. Expand it one ring if possible
		for (auto&& county : homeTeam.m_territory.m_currentHomeSubterritory->GetCounties())
		{
			for (auto&& adjacent : counties.GetAdjacents(county))
			{
				if (!homeTeam.m_territory.m_defaultHomeSubterritory.ContainsCounty(adjacent))
				{
					// Not part of the home territory, and we're not allowing expansion beyond home
					continue;
				}
				if (homeTeam.m_territory.m_currentHomeSubterritory->ContainsCounty(adjacent))
				{
					continue;
				}
				auto homeCountyIter = homeCounties.find(adjacent);
				if (homeCountyIter != homeCounties.end() && homeCountyIter->second != homeTeam.m_teamId)
				{
					// This is a home county of another team. Don't subsume it
					continue;
				}
				countiesToClaim.insert(adjacent);
			}
		}
	}
	else
	{
		// Don't own home stadium
		countiesToClaim.insert(homeTeam.m_territory.m_homeCounty);
		// Go through adjacents, see if they're in default home subterritory
		for (auto&& adjacent : counties.GetAdjacents(homeTeam.m_territory.m_homeCounty))
		{
			if (!allowHomeExpansion && !homeTeam.m_territory.m_defaultHomeSubterritory.ContainsCounty(adjacent))
			{
				// Not part of the home territory, and we're not allowing expansion beyond home
				continue;
			}
			auto homeCountyIter = homeCounties.find(adjacent);
			if (homeCountyIter != homeCounties.end() && homeCountyIter->second != homeTeam.m_teamId)
			{
				// This is a home county of another team. Don't subsume it
				continue;
			}
			countiesToClaim.insert(adjacent);
		}
		homeTeam.m_territory.m_currentHomeSubterritory = Subterritory{};
		homeTeam.m_territory.m_currentHomeSubterritory->SetOwner(&homeTeam.m_territory);
	}

	// Find all subterritories that contain any county we're claiming

	using CountySet = std::set<CountyId>;
	std::map<Subterritory*, CountySet> sourceSubterritories;
	for (auto&& county : countiesToClaim)
	{
		auto& countyObj = counties.FindCounty(county);
		sourceSubterritories[countyObj.m_owner].insert(county);
	}
	for (auto&& subterritory : sourceSubterritories)
	{
		Subterritory::TransferCounties(*subterritory.first, *homeTeam.m_territory.m_currentHomeSubterritory, subterritory.second, counties);
	}
}
