#include "Player.h"
#include "Ship.h"

using namespace std;
using namespace BattleShip;

// constructor
// now have to initialize vector with a new SeaItem and not 0
Player::Player(int inWidthHeight)
    : widthHeight(inWidthHeight)
    , grid(inWidthHeight * inWidthHeight)
    ,

    enemyTrackingGrid(inWidthHeight * inWidthHeight)
{
    for (auto& ptr : grid) {
        ptr = move(make_unique<BattleShip::SeaItem>());
    }
    for (auto& ptr : enemyTrackingGrid) {
        ptr = move(make_unique<BattleShip::SeaItem>());
    }

    placeShips();
}

// wrapping up into a class means we dont need to provide args. it can call
// itself
void Player::displayGrid()
{
    for (int j = widthHeight - 1; j >= 0; --j) {
        for (int i = 0; i < widthHeight; ++i) {
            cout << *grid[Convert2dTo1D(i, j)] << "  ";
        }

        cout << endl;
    }
    cout << endl;

    for (int j = widthHeight - 1; j >= 0; --j) {
        for (int i = 0; i < widthHeight; ++i) {
            cout << *enemyTrackingGrid[Convert2dTo1D(i, j)] << "  ";
        }

        cout << endl;
    }
}

void Player::placeShips()
{
    for (auto& currentShipSize : BattleShip::shipSize) { // loop through map

        generator.seed(rd());

        int shipLength = currentShipSize.second;
        int initialOffset = 0;
        int secondaryShift = 0;

        int offsetMax = widthHeight - shipLength; // find offset range

        bernoulli_distribution pickFromRangeBool(0.5);
        uniform_int_distribution<> pickFromRange(0, widthHeight - 1);
        uniform_int_distribution<> pickFromRangeOffset(0, offsetMax - 1);

        auto orientationToSet = pickFromRangeBool(generator) ? eOrientation::VERTICAL
                                                             : eOrientation::HORIZONTAL;

        bool tryToPlace = true;
        while (tryToPlace) {
            initialOffset = pickFromRange(generator);
            secondaryShift = pickFromRangeOffset(generator);

            GridRange gridRange{ initialOffset, secondaryShift, shipLength,
                orientationToSet };

            bool bStartAgain = false;

            ////HERE WE WILL ADD RANGE LOOPING
            for (gridRange.Begin();
                 (gridRange.Current() != gridRange.End()) && !bStartAgain; gridRange.Bump()) {

                eShipStatus testShip = grid[Convert2dTo1D(gridRange.Current().x, gridRange.Current().y)]
                                           ->Type();

                if (testShip != eShipStatus::Sea) {
                    bStartAgain = true;
                    break;
                }
            }

            // if we dont have to start again loop through ship and place!
            if (!bStartAgain) {

                for (gridRange.Begin();
                     gridRange.Current() != gridRange.End(); gridRange.Bump()) {

                    auto newShipPtr = make_unique<Ship>(currentShipSize.first, this);

                    newShipPtr->orientation = orientationToSet;
                    newShipPtr->index = initialOffset;
                    newShipPtr->fromPos = secondaryShift;
                    newShipPtr->gridRange = gridRange;

                    grid[Convert2dTo1D(gridRange.Current().x, gridRange.Current().y)] = move(newShipPtr);
                }
                tryToPlace = false;
            }
        } // end while
    }
}

// called from enemy
bool Player::AttemptHit(int posRow, int posCol)
{
    BattleShip::SeaItem* item = grid[Convert2dTo1D(posRow, posCol)].get();
    BattleShip::eShipStatus attempt = item->Hit();

    // alternative call
    // grid[Convert2dTo1D(posRow, posCol)].get()->Hit()
    if (attempt == BattleShip::eShipStatus::HIT) {
        cout << "HIT" << endl;
        return true;
    } else {
        cout << "MISS" << endl;
        return false;
    }
}

// only pass a status as we create the pointer inside the function and move it
// in. hard pass around unique_ptrs
void Player::addHitPositionToTrackingGrid(int posRow, int posCol,
    BattleShip::eShipStatus hitormiss)
{
    enemyTrackingGrid[Convert2dTo1D(posRow, posCol)] = move(make_unique<BattleShip::Ship>(hitormiss, this));
}

// we can pass a ship because only ships will be calling this
bool Player::checkIfWholeShipSunk(GridRange gridRange)


{

        
    for (gridRange.Begin();
         gridRange.Current() != gridRange.End(); gridRange.Bump()) {
        
        std::cout << "checking " << gridRange.Current().x << " and " << gridRange.Current().y << endl;
        if (grid[Convert2dTo1D(gridRange.Current().x, gridRange.Current().y)]->Type() != BattleShip::eShipStatus::HIT) {
            return false;
        }
    }

    return true;
}

// we know the width so can just take an x and y
int Player::Convert2dTo1D(int x, int y) { return x + y * widthHeight; }
