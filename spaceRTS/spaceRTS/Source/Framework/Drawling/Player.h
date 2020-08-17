#pragma once
#include "Vender/glm/glm.hpp"
#include "Core.h"

namespace MYENGINE
{
    enum PlayerNumber
    {
        Player1=1,
        Player2=2,
        Player3=4,
        Player4=8,
        PlayerAI=16,
        PlayerNutrual=32,
        numPlayers=6
    };
    enum Team
    {
        Team1=1,
        Team2=2,
        Team3=4,
        Team4=8,
        TeamAI=16,
        TeamNutrual=32,
        numTeams=6
    };
    class MYENGINE_API Player
    {
    public:
        Player(const PlayerNumber& i_playerNum,
            const Team& i_team,
            const glm::vec4& i_color );
        PlayerNumber playerNum;
        Team team;
        glm::vec4 color;
    };

    typedef struct PlayerList
    {
        std::vector<Player> currentPlayers;
        unsigned short playerIndex;
        inline Player& GetMainPlayer() { return currentPlayers[playerIndex]; }
    };
}