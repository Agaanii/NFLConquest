#pragma once

#include "Subterritory.h"

#include <optional>

class Territory
{
public:
	Territory() {}
	Territory(const CountyId& homeCounty)
		: m_homeCounty(homeCounty)
	{
	}

	Territory(const Territory& other)
		: m_homeCounty(other.m_homeCounty)
		, m_defaultHomeSubterritory(other.m_defaultHomeSubterritory)
		, m_currentHomeSubterritory(other.m_currentHomeSubterritory)
		, m_earnedTerritories(other.m_earnedTerritories)
	{
		m_defaultHomeSubterritory.SetOwner(this);
		if (m_currentHomeSubterritory) m_currentHomeSubterritory->SetOwner(this);
		for (auto&& sub : m_earnedTerritories)
		{
			sub.SetOwner(this);
		}
	}

	void ClaimHome()
	{
		m_defaultHomeSubterritory.SetOwner(this);
		if (m_currentHomeSubterritory) m_currentHomeSubterritory->SetOwner(this);
	}

	void ClaimTerritories(CountyMap& countyMap)
	{
		if (m_currentHomeSubterritory)
		{
			m_currentHomeSubterritory->SetOwner(this);
			m_currentHomeSubterritory->ClaimCounties(countyMap);
		}
		for (auto&& subterritory : m_earnedTerritories)
		{
			subterritory.SetOwner(this);
			subterritory.ClaimCounties(countyMap);
		}
	}

	auto GetTerritoryCount() const { return m_earnedTerritories.size() + (m_currentHomeSubterritory.operator bool() ? 1 : 0); }

	CountyId m_homeCounty{ 0 };
	Subterritory m_defaultHomeSubterritory;
	std::optional<Subterritory> m_currentHomeSubterritory;
	std::vector<Subterritory> m_earnedTerritories;
};
