#pragma once


#include <iostream>
#include "4_BattleShipUtility.hpp"


namespace BattleShip{
    
    class SeaItem{

       
    public:
        SeaItem();
        SeaItem(eShipStatus shipType);
        
    
        friend std::ostream& operator<<(std::ostream& os, const SeaItem& obj){
            // write obj to stream USE OBJ given!!
            return os << BattleShip::shipDisplayIcon[obj.type];
        }
        
        void Display();
        
        //seaItem is the sea so it is always a miss
        virtual eShipStatus Hit();
        
        eShipStatus Type();
       
    private:      
        

    //make these two protected so that we can use them in derived classes.
    protected:
        //realise that we need to keep track of if the sea has been hit before.take bDamaged and change to bHit
        bool bHit = false;
        eShipStatus type = eShipStatus::Sea;

    };
}