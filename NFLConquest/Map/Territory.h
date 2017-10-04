#include "Subterritory.h"

#include <optional>
#include <stack>

class Territory
{
protected:
	CountyId m_homeCounty{ 0 };
	Subterritory m_defaultHomeSubterritory;
	std::optional<Subterritory> m_currentHomeSubterritory;
	std::stack<Subterritory> m_earnedTerritories;

	std::unique_ptr<sf::Sprite> m_teamLogo;
	sf::Color m_teamColor;
};
