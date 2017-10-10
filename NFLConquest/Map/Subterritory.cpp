#include "Subterritory.h"

bool Subterritory::ContainsCounty(const CountyId& countyId)
{
	return m_counties.find(countyId) != m_counties.end();
}

bool Subterritory::TransferCounties(Subterritory& from, Subterritory& to, const std::vector<CountyId>& shiftingCounties, std::map<CountyId, County>& countyMap)
{
	for (auto&& county : shiftingCounties)
	{
		if (!from.ContainsCounty(county))
		{
			return false;
		}
	}

	for (auto& county : shiftingCounties)
	{
		to.m_counties.emplace(county);
		from.m_counties.erase(county);
		countyMap[county].m_owner = &to;
	}
	return true;
}

void Subterritory::AddCounty(const CountyId& county)
{
	m_counties.insert(county);
}
