#pragma once

#include "5_SeaItem.h"
#include "5_BattleShipUtility.hpp"
#include <iostream>

//forward declaration
class Player;

namespace BattleShip{

    class Ship : public SeaItem {
        public:
            Ship(eShipStatus shipType, Player* playerPtrIn);
            
            virtual eShipStatus Hit() override;
                //could hoist into SeaItem but not ideal as it's specific to ships but makes polymorphism easier
            //later will get rid of these but for now ship keeps track of where it is.
            //this provides a good way to show dynamic cast? good way to intriduce pointers?
            //or test if shit and make a local reference! yay references.
            eOrientation orientation;
            //might be col or row depending on orientation 
            int index;
            //so we can search and count damage
            int fromPos;
            int toPos;

            BattleShip::GridRange gridRange;

        
            
        protected:

            Player* player;
            
            
    };
}
