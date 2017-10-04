#include "County.h"

#include <set>

class Subterritory
{
public:
	bool ContainsCounty(const CountyId& countyId);
	void AddCounty(const CountyId& county);

	static bool TransferCounties(Subterritory& from, Subterritory& to, const std::vector<CountyId>& counties);
private:
	std::set<CountyId> m_counties;
};
