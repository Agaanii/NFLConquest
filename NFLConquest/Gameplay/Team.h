#pragma once

#include "..\Map\Territory.h"

using TeamId = long;
class Team
{
public:
	Team(){}
	Team(TeamId teamId, const Territory& territory)
		: m_teamId(teamId)
		, m_territory(territory)
	{ }

	TeamId m_teamId{ 0 };
	Territory m_territory;
	std::unique_ptr<sf::Sprite> m_teamLogo;
	sf::Color m_teamColor;
};
