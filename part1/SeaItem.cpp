#include "SeaItem.h"
#include <iostream>

namespace BattleShip{
    
        using namespace std;

        SeaItem::SeaItem(){};
        SeaItem::SeaItem(eShipStatus shipType)  : type(shipType){}
        
        
        void SeaItem::Display(){
            //dont need to change as Hit changes type for us.
            cout << BattleShip::shipDisplayIcon[type];

        }
        
        //seaItem is the sea so it is always a miss
        eShipStatus SeaItem::Hit(){
            cout << "im a seaItem!" << endl;
            type = eShipStatus::MISS;
            return type;
        }
        
        eShipStatus SeaItem::Type(){
            return type;
        }

        
}


