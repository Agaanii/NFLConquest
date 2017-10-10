#pragma once

#include "County.h"

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

	static bool TransferCounties(Subterritory& from, Subterritory& to, const std::vector<CountyId>& shiftingCounties, std::map<CountyId, County>& countyMap);
private:
	std::set<CountyId> m_counties;
	Territory* m_owner;
};
