#pragma once

#include "Subterritory.h"

#include <optional>
#include <stack>

class Territory
{
public:
	Territory() {}
	Territory(const CountyId& homeCounty, const Subterritory& defaultHomeSubterritory)
		: m_homeCounty(homeCounty)
		, m_defaultHomeSubterritory(defaultHomeSubterritory)
		, m_currentHomeSubterritory(defaultHomeSubterritory)
	{
		m_defaultHomeSubterritory.SetOwner(this);
	}

	auto GetTerritoryCount() const { return m_earnedTerritories.size() + (m_currentHomeSubterritory.operator bool() ? 1 : 0); }

	CountyId m_homeCounty{ 0 };
	Subterritory m_defaultHomeSubterritory;
	std::optional<Subterritory> m_currentHomeSubterritory;
	std::stack<Subterritory> m_earnedTerritories;
};
