#include "4_SeaItem.h"
#include "4_BattleShipUtility.h"
#include "4_Player.h"
#include <iostream>



namespace BattleShip{

    class Ship : public SeaItem {
        public:
            Ship(eShipStatus shipType, Player* playerPtrIn)  :  SeaItem(shipType), player(playerPtrIn){}
            
            virtual eShipStatus Hit() override{
                if(!bHit){
                    std::cout << "im a ship!" << std::endl;
                    bHit = true;
                    //store old type so that we can pass to checkSunk function because we are about to change it and that func will be checking me too
                    eShipStatus oldType = type;
                    type = eShipStatus::HIT;
                    if (player->checkIfWholeShipSunk(Type(), index, fromPos, orientation)){
                       std::cout << "ship is Dead!" << std::endl;
                    }
                    
                }

                return type;
            }
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

        
            
        protected:

            Player* player;
            
            
    };
}
