#include "SFML\Graphics.hpp"

#include "Gameplay\Game.h"
#include "Gameplay\Team.h"
#include "Map\Map.h"

sf::RenderWindow s_window(sf::VideoMode(1600, 900), "NFL Conquest");

int main()
{
	CountyMap counties;
	for (CountyId c = 1; c < 101; ++c)
	{
		counties.AddCounty(County(nullptr, { 0,0 }, c), { (c < 100) ? c + 1 : c - 1, (c > 1) ? c - 1 : c + 1 });
	}
	std::set<CountyId> homeCounties;

	std::map<TeamId, Team> teams;
	for (TeamId t = 1; t < 11; ++t)
	{
		Subterritory homeTerritory;

		for (CountyId c = 1; c < 11; ++c)
		{
			homeTerritory.AddCounty(c + (10 * (t - 1)));
		}
		CountyId homeCounty = 10 * t - 5;
		homeCounties.insert(homeCounty);
		teams.emplace(t, Team(t, Territory(homeCounty, homeTerritory)));
	}

	auto game = Game{ 1, 2, Game::Result::AWAY_WIN };

	auto&& homeTeam = teams[game.m_homeTeam];
	auto&& awayTeam = teams[game.m_awayTeam];
	switch (game.m_result)
	{
	case Game::Result::TIE:
	case Game::Result::UNDECIDED:
		// No territory change
		break;
	case Game::Result::HOME_WIN:
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
					if (!allowHomeExpansion && !homeTeam.m_territory.m_defaultHomeSubterritory.ContainsCounty(adjacent))
					{
						// Not part of the home territory, and we're not allowing expansion beyond home
						continue;
					}
					if (homeCounties.count(adjacent))
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
				if (homeCounties.count(adjacent))
				{
					// This is a home county of another team. Don't subsume it
					continue;
				}
				countiesToClaim.insert(adjacent);
			}
		}

		break;
	}
	case Game::Result::AWAY_WIN:
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
				awayTeam.m_territory.m_earnedTerritories.push(homeTeam.m_territory.m_earnedTerritories.top());
				homeTeam.m_territory.m_earnedTerritories.pop();
			}
			else
			{
				awayTeam.m_territory.m_earnedTerritories.push(*homeTeam.m_territory.m_currentHomeSubterritory);
				homeTeam.m_territory.m_currentHomeSubterritory.reset();
			}
		}
	}
		break;
	}

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
