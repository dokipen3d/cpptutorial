#pragma once


#include <map>

//everything we do in here is guarded by scope. this is used to put misc data about the game
namespace BattleShip{

    using std::map;

    //enum for different kinds of ships allowed
    enum class eShipStatus{
        Carrier,
        Battleship,
        Cruiser,
        Submarine,
        Destroyer,
        Sea,
        HIT,
        MISS
    };
    
    //map to get ship size based on type
    static map<eShipStatus, int> shipSize{
        {eShipStatus::Carrier, 5},
        {eShipStatus::Battleship, 4},
        {eShipStatus::Cruiser,3},
        {eShipStatus::Submarine,3},
        {eShipStatus::Destroyer,2}
    };
    
   
    
    //display character
    static map<eShipStatus, char> shipDisplayIcon{
        {eShipStatus::Carrier, 'C'},
        {eShipStatus::Battleship, 'B'},
        {eShipStatus::Cruiser, 'c'},
        {eShipStatus::Submarine, 'S'},
        {eShipStatus::Destroyer, 'D'},
        {eShipStatus::Sea, '~'},
        {eShipStatus::HIT, 'X'},
        {eShipStatus::MISS, '*'}


    };
    
    //could be bool but how to interpret true or false? call it is up?
    enum class eOrientation{
        VERTICAL,
        HORIZONTAL
    };
    
    //ideas for adding 'range' item for looping along column rows
    struct position {
        int row;
        int coloumn;
    }

    struct GridRange {
        position start;
        position end;
    }
}