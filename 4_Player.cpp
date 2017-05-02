#include "4_Player.h"  
#include "4_BattleShipUtility.h"
#include "4_Ship.h"

using namespace std;
//constructor
//now have to initialize vector with a new SeaItem and not 0
Player::Player(int inWidthHeight) :     widthHeight(inWidthHeight), 
                                grid(   inWidthHeight*inWidthHeight
                                        ),

                                enemyTrackingGrid(  inWidthHeight*inWidthHeight
                                                    )
                                {
                                
                                    

                                    for (auto& ptr : grid){
                                        ptr = move(make_unique<BattleShip::SeaItem>());
                                    }
                                    for (auto& ptr : enemyTrackingGrid){
                                        ptr = move(make_unique<BattleShip::SeaItem>());
                                    }

                                    placeShips(); 
                                }

//wrapping up into a class means we dont need to provide args. it can call itself
void Player::displayGrid(){
    for (int j = 0; j < widthHeight; ++j){
        for (int i = 0; i < widthHeight; ++i){
            cout  << *grid[Convert2dTo1D(i,j)] << "  ";
        }
                
        cout << endl;  
    }
    cout << endl;  

    for (int j = 0; j < widthHeight; ++j){
        for (int i = 0; i < widthHeight; ++i){
            cout  << *enemyTrackingGrid[Convert2dTo1D(i,j)] << "  ";
        }
                
        cout << endl;
    }
}

void Player::placeShips(){
    

    for (auto& shipSize : BattleShip::shipSize){ //loop through map
        //random bool for orientation
        //random int for col/row
        //find offset range
        //rand int for offset in range.
        //loop through positions and check it they are free (if they are all sea) use algo?
        //If free, loop through positions and place ship with map.first
        //auto timeSeed = chrono::high_resolution_clock::now().time_since_epoch().count();
        generator.seed(rd());
        
        bool tryToPlace = true;
        int shipLength =  shipSize.second;
        //random bool for orientation
        BattleShip::eOrientation orientationToSet;
        //random int for col/row
        int row = 0;
        int coloumn = 0;

        int offsetMax = widthHeight-shipLength;//find offset range
        
        uniform_int_distribution<> pickFromRangeBool(0,1);
        uniform_int_distribution<> pickFromRange(0,widthHeight-1);
        uniform_int_distribution<> pickFromRangeOffset(0,offsetMax-1);

        (pickFromRangeBool(generator) > 0) ? 
                orientationToSet = BattleShip::eOrientation::VERTICAL : 
                orientationToSet = BattleShip::eOrientation::HORIZONTAL;

        while(tryToPlace){
            

            row = pickFromRange(generator);
            coloumn = pickFromRangeOffset(generator);  

            if(orientationToSet == BattleShip::eOrientation::VERTICAL){
                swap(row, coloumn);
            }
            //so we can break inner for loop (used in condition)
            bool bStartAgain = false;
            
            for (int i = 0; (i < shipLength) && !bStartAgain; ++i){
                
                //change if we add to row or col
                int r = 0;
                int c = 0;
                (orientationToSet == BattleShip::eOrientation::VERTICAL) ? r = i : c = i;
                BattleShip::eShipStatus testShip = grid[Convert2dTo1D(row+r,coloumn+c)]->Type();
                if (testShip != BattleShip::eShipStatus::Sea){
                    bStartAgain = true;
                    break;
                }
            }

            //if we dont have to start again loop through ship and place!
            if (!bStartAgain){
                    for (int i = 0; i < shipLength; ++i){
                    //change if we add to row or col
                    int r = 0;
                    int c = 0;
                    (orientationToSet == BattleShip::eOrientation::VERTICAL) ? r = i : c = i;
                    
                    auto newShipPtr = make_unique<BattleShip::Ship>(shipSize.first, this);
                    
                    newShipPtr->orientation = orientationToSet;
                    newShipPtr->index = row;
                    newShipPtr->fromPos = coloumn;

                    grid[Convert2dTo1D(row+r,coloumn+c)] = move(newShipPtr);

                    
                    //cout << " " << row << " " << coloumn << " " << endl;
                    

                        tryToPlace = false;
                    }
            }
        }//end while
    }
}



//called from enemy
bool Player::AttemptHit(int posRow, int posCol) {

    BattleShip::SeaItem* item = grid[Convert2dTo1D(posRow, posCol)].get();
    BattleShip::eShipStatus attempt = item->Hit();

    if (attempt == BattleShip::eShipStatus::HIT) {
        
        cout << "HIT" << endl;
        return true;
    }
    else
    {
        cout << "MISS" << endl;
        return false;
    }
}

//only pass a status as we create the pointer inside the function and move it in. hard pass around unique_ptrs
void Player::addHitPositionToTrackingGrid(int posRow, int posCol, BattleShip::eShipStatus hitormiss ) {

    enemyTrackingGrid[Convert2dTo1D(posRow, posCol)] = move(make_unique<BattleShip::Ship>(BattleShip::eShipStatus::HIT, this));

}

//we can pass a ship because only ships will be calling this
bool Player::checkIfWholeShipSunk(BattleShip::eShipStatus type, int row, int coloumn, BattleShip::eOrientation orientation) {

    int shipLength = BattleShip::shipSize[type]; 

    if(orientation == BattleShip::eOrientation::VERTICAL){
                swap(row, coloumn);
            }

    for (int i = 0; i < shipLength; ++i){
        //change if we add to row or col
        int r = 0;
        int c = 0;
        (orientation == BattleShip::eOrientation::VERTICAL) ? r = i : c = i;
        
        if (grid[Convert2dTo1D(row+r,coloumn+c)]->Type() != BattleShip::eShipStatus::HIT){
            return false;
        }
    }

    return true;

}


//we know the width so can just take an x and y
int Player::Convert2dTo1D(int x, int y){
    return x + y*widthHeight;
}

