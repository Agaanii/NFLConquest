#include "Subterritory.h"

extern CountyMap counties;
bool Subterritory::ContainsCounty(const CountyId& countyId)
{
	return m_counties.find(countyId) != m_counties.end();
}

bool Subterritory::TransferCounties(Subterritory& from, Subterritory& to, const std::set<CountyId>& shiftingCounties, CountyMap& countyMap)
{
	if (&from == &to) return true;
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
		countyMap.FindCounty(county).m_owner = &to;
	}
	return true;
}

void Subterritory::AddCounty(const CountyId& county)
{
	m_counties.insert(county);
	counties.FindCounty(county).m_owner = this;
}
