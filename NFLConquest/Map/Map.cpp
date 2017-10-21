#include "Map.h"

#include "Subterritory.h"

void CountyMap::AddCounty(County&& county, std::set<CountyId> adjacentCounties)
{
	m_counties.insert({ county.m_countyId, std::move(county) });
	m_edges[county.m_countyId] = adjacentCounties;
}

County& CountyMap::FindCounty(const CountyId& county)
{
	return m_counties[county];
}

std::set<CountyId> CountyMap::GetAdjacents(const CountyId& countyId) const
{
	auto iter = m_edges.find(countyId);
	if (iter == m_edges.end()) return {};
	return iter->second;
}

void CountyMap::Validate() const
{
	for (auto&& county : m_counties)
	{
		if(!county.second.m_owner->ContainsCounty(county.first))
		{
			exit(-1);
		}
	}
}
