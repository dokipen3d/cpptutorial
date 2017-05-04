#pragma once

#include "4_SeaItem.h"
#include "4_BattleShipUtility.hpp"


#include <vector>
#include <memory>
#include <random>


class Player{   



public:
    //constructor
    //now have to initialize vector with a new SeaItem and not 0
    Player(int inWidthHeight);
    
    //wrapping up into a class means we dont need to provide args. it can call itself
    void displayGrid();
    
    void placeShips();
    
    //called from enemy
    bool AttemptHit(int posRow, int posCol);

    //only pass a status as we create the pointer inside the function and move it in. hard pass around unique_ptrs
    void addHitPositionToTrackingGrid(int posRow, int posCol, BattleShip::eShipStatus hitormiss );

    //we can pass a ship because only ships will be calling this
    bool checkIfWholeShipSunk(BattleShip::eShipStatus type, int row, int coloumn, BattleShip::eOrientation orientation);

private:
    //we know the width so can just take an x and y
    int Convert2dTo1D(int x, int y);
    
    int widthHeight = 10;

    //important to specify namespace
    std::vector<std::unique_ptr<BattleShip::SeaItem>> grid;
    std::vector<std::unique_ptr<BattleShip::SeaItem>> enemyTrackingGrid;

    
    //for random number generation
    std::random_device rd;
    std::mt19937 generator; //seed with 0. can seed with  std::random_device rd;
    
};