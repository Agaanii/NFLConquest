#pragma once

#include "County.h"

#include <map>
#include <set>

class CountyMap
{
public:
	void AddCounty(County&& county, std::set<CountyId> adjacentCounties);
	County& FindCounty(const CountyId& county);
	std::set<CountyId> GetAdjacents(const CountyId& countyId) const;
	void Validate() const;
private:
	using Counties = std::map<CountyId, County>;
	using CountyAdjecencyMap = std::map<CountyId, std::set<CountyId>>;

	Counties m_counties;
	CountyAdjecencyMap m_edges;
};