#include "Ship.h"
#include "Player.h"
#include <iostream>



namespace BattleShip{

    
    Ship::Ship(eShipStatus shipType, Player* playerPtrIn)  :  SeaItem(shipType), player(playerPtrIn){}
    
    eShipStatus Ship::Hit() {
        if(!bHit){
            std::cout << "im a ship!" << std::endl;
            bHit = true;
            //store old type so that we can pass to checkSunk function because we are about to change it and that func will be checking me too
            eShipStatus oldType = type;
            type = eShipStatus::HIT;

           
            //remember to pass in the oldtype otherwise we wont check thr ight length
            if (player->checkIfWholeShipSunk(this->gridRange)){
                std::cout << "ship is Dead!" << std::endl;
            }
            
        }

        return type;
    }
            
}
