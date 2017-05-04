#include "5_Player.h"  
#include "5_Ship.h"


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
    for (int j = widthHeight-1; j >= 0 ; --j){
        for (int i = 0; i < widthHeight; ++i){
            cout  << *grid[Convert2dTo1D(i,j)] << "  ";
        }
                
        cout << endl;  
    }
    cout << endl;  

    for (int j = widthHeight-1; j >= 0; --j){
        for (int i = 0; i < widthHeight; ++i){
            cout  << *enemyTrackingGrid[Convert2dTo1D(i,j)] << "  ";
        }
                
        cout << endl;
    }
}

void Player::placeShips(){
    

    for (auto& shipSize : BattleShip::shipSize){ //loop through map
        
        generator.seed(rd());
        

        int shipLength =  shipSize.second;
        int row = 0;
        int coloumn = 0;

        int offsetMax = widthHeight-shipLength;//find offset range
        
        uniform_int_distribution<> pickFromRangeBool(0,1);
        uniform_int_distribution<> pickFromRange(0,widthHeight-1);
        uniform_int_distribution<> pickFromRangeOffset(0,offsetMax-1);

        auto orientationToSet = [&](){
            if ((pickFromRangeBool(generator) > 0)) {
                return BattleShip::eOrientation::VERTICAL;
            }
            return BattleShip::eOrientation::HORIZONTAL;
        }();

        bool tryToPlace = true;     
        while(tryToPlace){

            row = pickFromRange(generator);
            coloumn = pickFromRangeOffset(generator);  

            auto gridRange = [&]() -> BattleShip::GridRange { 
                switch (orientationToSet) {
                    case BattleShip::eOrientation::VERTICAL:
                        return {
                            {row, coloumn}, 
                            {row+shipLength, coloumn}
                        };
                    case BattleShip::eOrientation::HORIZONTAL:
                        return  {
                            {coloumn, row}, 
                            {coloumn, row+shipLength}
                        };
                }
            }();

            // if(orientationToSet == BattleShip::eOrientation::VERTICAL){
            //     swap(row, coloumn);
            // }
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


    //alternative call
    //grid[Convert2dTo1D(posRow, posCol)].get()->Hit()
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

    enemyTrackingGrid[Convert2dTo1D(posRow, posCol)] = move(make_unique<BattleShip::Ship>(hitormiss, this));

}

//we can pass a ship because only ships will be calling this
bool Player::checkIfWholeShipSunk(BattleShip::eShipStatus type, int row, int coloumn, BattleShip::eOrientation orientation) {

    int shipLength = BattleShip::shipSize[type]; 
    std::cout << "shipSize is  " << shipLength<< endl;
    //here we fixed a bug. we were swapping the row and coloumn, forgetting that we had already swapped the, before.

    for (int i = 0; i < shipLength; ++i){
        //change if we add to row or col
        int r = 0;
        int c = 0;
        (orientation == BattleShip::eOrientation::VERTICAL) ? r = i : c = i;
        std::cout << "checking " << row+r << " and " << coloumn+c << endl;
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
