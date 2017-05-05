#pragma once

#include "SeaItem.h"
#include "BattleShipUtility.hpp"
#include <iostream>

//forward declaration
class Player;

namespace BattleShip{

    class Ship : public SeaItem {
        public:
            Ship(eShipStatus shipType, Player* playerPtrIn);
            
            virtual eShipStatus Hit() override;
            BattleShip::GridRange gridRange;
            
        protected:

            Player* player;
            
            
    };
}
