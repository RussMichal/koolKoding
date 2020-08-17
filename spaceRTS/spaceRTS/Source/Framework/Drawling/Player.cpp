#include "MEpch.h"
#include "Player.h"

namespace MYENGINE
{
    Player::Player(const PlayerNumber & i_playerNum, const Team & i_team, const glm::vec4 & i_color)
        : playerNum(i_playerNum)
        , team(i_team)
        , color(i_color)
    {

    }
}