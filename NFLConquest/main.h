#pragma once

#include "Map/Map.h"
#include "Gameplay/Team.h"

void ProcessGameResult(Game &game, std::map<TeamId, Team>& teams, CountyMap &counties, std::set<CountyId> &homeCounties);

std::map<TeamId, Territory> CacheTerritories(std::map<TeamId, Team> &teams);

void drawCounty(int PIXEL_SIZE, Team & team, const CountyId & county, int yPos);

void ProcessAwayWin(Team & homeTeam, Team & awayTeam);

void ProcessHomeWin(Team & homeTeam, CountyMap & counties, const std::set<CountyId>& homeCounties);