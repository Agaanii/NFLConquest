#pragma once

#include "County.h"
#include "Map.h"

#include <set>

class Territory;
class Subterritory
{
public:
	bool ContainsCounty(const CountyId& countyId);
	void AddCounty(const CountyId& county);
	void SetOwner(Territory* owner) 
	{ 
		m_owner = owner;
	}
	Territory* GetOwner() const { return m_owner; }
	const std::set<CountyId>& GetCounties() const { return m_counties; }
	void ClaimCounties(CountyMap& countyMap)
	{
		for (auto&& county : m_counties)
		{
			countyMap.FindCounty(county).m_owner = this;
		}
	}

	static bool TransferCounties(Subterritory& from, Subterritory& to, const std::set<CountyId>& shiftingCounties, CountyMap& countyMap);
private:
	std::set<CountyId> m_counties;
	Territory* m_owner;
};
