#include "Team.h"

class Game
{
public:
	enum class Result
	{
		UNDECIDED,
		TIE,
		HOME_WIN,
		AWAY_WIN
	};

	TeamId m_homeTeam;
	TeamId m_awayTeam;
	Result m_result;
};

using Week = std::vector<Game>;
using Season = std::vector<Week>;