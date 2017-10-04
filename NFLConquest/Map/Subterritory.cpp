#include "Subterritory.h"

bool Subterritory::ContainsCounty(const CountyId& countyId)
{
	return m_counties.find(countyId) != m_counties.end();
}

bool Subterritory::TransferCounties(Subterritory& from, Subterritory& to, const std::vector<CountyId>& counties)
{
	for (auto&& county : counties)
	{
		if (!from.ContainsCounty(county))
		{
			return false;
		}
	}

	for (auto&& county : counties)
	{
		to.m_counties.emplace(county);
		from.m_counties.erase(county);
	}
	return true;
}

void Subterritory::AddCounty(const CountyId& county)
{
	m_counties.insert(county);
}
