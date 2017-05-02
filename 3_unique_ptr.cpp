#include <iostream>
#include <vector>
#include <array>
#include <memory>
#include <algorithm>
#include <map>
#include <random>
#include <chrono>
#include <functional>
#include <memory>

using namespace std;

//http://en.cppreference.com/w/cpp/language/operators
//https://msdn.microsoft.com/en-us/library/1z2f6c2k.aspx

//add unique pointer.
//use make unique everywhere
//have to use move to pass ownership
// cant copy so when initializing cant do it in class constructor


//everything we do in here is guarded by scope. this is used to put misc data about the game
namespace BattleShip{
    
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
    map<eShipStatus, int> shipSize{
        {eShipStatus::Carrier, 5},
        {eShipStatus::Battleship, 4},
        {eShipStatus::Cruiser,3},
        {eShipStatus::Submarine,3},
        {eShipStatus::Destroyer,2}
    };
    
   
    
    //display character
    map<eShipStatus, char> shipDisplayIcon{
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
    
    //base for sea items to go in grid
    class SeaItem{
    public:
        SeaItem(){};
        SeaItem(eShipStatus shipType)  : type(shipType){}
        
        //To get cout to accept a SeaItem object after the insertion operator, 
        //overload the insertion operator to recognize an ostream object on the
        //left and a SeaItem on the right. The overloaded << operator function must
        //then be declared as a friend of class SeaItem so it can access the 
        //private data within a SeaItem object.
        friend std::ostream& operator<<(std::ostream& os, const SeaItem& obj){
            // write obj to stream USE OBJ given!!
            return os << BattleShip::shipDisplayIcon[obj.type];
        }
        
        void Display(){
            //dont need to change as Hit changes type for us.
            cout << BattleShip::shipDisplayIcon[type];

        }
        
        //seaItem is the sea so it is always a miss
        virtual eShipStatus Hit(){
            cout << "im a seaItem!" << endl;
            type = eShipStatus::MISS;
            return type;
        }
        
        eShipStatus Type(){
            return type;
        }

       
    private:      
        

    //make these two protected so that we can use them in derived classes.
    protected:
        //realise that we need to keep track of if the sea has been hit before.take bDamaged and change to bHit
        bool bHit = false;
        eShipStatus type = eShipStatus::Sea;

    };
    //first we are filling each position with a ship. naive
    class Ship : public SeaItem {
    public:
        Ship(eShipStatus shipType)  :  SeaItem(shipType){}
        
        virtual eShipStatus Hit() override{
             if(!bHit){
                cout << "im a ship!" << endl;
                bHit = true;
                //store old type so that we can pass to checkSunk function because we are about to change it and that func will be checking me too
                eShipStatus oldType = type;
                type = eShipStatus::HIT;
                
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

     
         
    private:
        
        
    };
}


class Player{   

public:
    //constructor
    //now have to initialize vector with a new SeaItem and not 0
    Player(int inWidthHeight) :     widthHeight(inWidthHeight), 
                                    grid(   inWidthHeight*inWidthHeight
                                           ),

                                    enemyTrackingGrid(  inWidthHeight*inWidthHeight
                                                        )
                                    {
                                        generator.seed(42);
                                        

                                        for (auto& ptr : grid){
                                            ptr = move(make_unique<BattleShip::SeaItem>());
                                        }
                                        for (auto& ptr : enemyTrackingGrid){
                                            ptr = move(make_unique<BattleShip::SeaItem>());
                                        }

                                        placeShips(); 
                                    }
    
    //wrapping up into a class means we dont need to provide args. it can call itself
    void displayGrid(){
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
    
    void placeShips(){
        

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
                        
                        auto newShipPtr = make_unique<BattleShip::Ship>(shipSize.first);
                        
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
    
    //clear ourself
    void clear(){
        //fill( begin(grid), end(grid), move(make_unique<BattleShip::SeaItem>(BattleShip::eShipStatus::Sea)));
    }

    //called from enemy
    bool AttemptHit(int posRow, int posCol) {

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
    void addHitPositionToTrackingGrid(int posRow, int posCol, BattleShip::eShipStatus hitormiss ) {

        enemyTrackingGrid[Convert2dTo1D(posRow, posCol)] = move(make_unique<BattleShip::Ship>(BattleShip::eShipStatus::HIT));

    }

    //we can pass a ship because only ships will be calling this
    bool checkIfWholeShipSunk(BattleShip::eShipStatus type, int row, int coloumn, BattleShip::eOrientation orientation) {

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
 



    
private:
    //we know the width so can just take an x and y
    int Convert2dTo1D(int x, int y){
        return x + y*widthHeight;
    }
    
    int widthHeight = 10;

    //important to specify namespace
    vector<unique_ptr<BattleShip::SeaItem>> grid;
    vector<unique_ptr<BattleShip::SeaItem>> enemyTrackingGrid;

    
    //for random number generation
    random_device rd;
    mt19937 generator; //seed with 0. can seed with  std::random_device rd;
    
    
};

class Game{
public:
    Game(int gridSize) : me(gridSize), AI(gridSize) {
        cout << "game is created with gridSize: " << gridSize << "!" << endl;
        //myGrid.clear();
        startGameLoop();
    }


    void startGameLoop() {
        while ((eStatus == GameStatus::MENU) || (eStatus == GameStatus::PLAYING))
        {
        
            cout << "What would you like to do? Type 'p' to play the game or 'e' to exit" << endl;
            cin >> command;
            
            switch(command) {
                case 'P' :
                case 'p' :  cout << "playing game" << endl;
                            eStatus = GameStatus::PLAYING;
                            break;

                
                            
                case 'E' :
                case 'e' :  cout << "exiting game" << endl;
                            eStatus = GameStatus::EXITING;
                            break;
                            
                default :   cout << "you typed the wrong thing. Please try again" << endl;
            }

            while(eStatus == GameStatus::PLAYING) {
            me.displayGrid();
            cout << endl;
            AI.displayGrid();
            cout << endl;


            cout << "please enter a coordinate to drop a bomb!" << endl;
            int a{0};
            int b{0};

            cin >> a >> b;

           

            if(AI.AttemptHit(a,b)) {
                me.addHitPositionToTrackingGrid(a, b,
                 BattleShip::eShipStatus::HIT);
            }
            else {
                me.addHitPositionToTrackingGrid(a, b, 
                BattleShip::eShipStatus::MISS);
            }

        }

        



            


        }
    }

    
    
    enum class GameStatus {
        MENU,
        PLAYING,
        EXITING
    };
    
private:
    GameStatus eStatus = GameStatus::MENU;
    char command;
    Player me;
    Player AI;
};

int main()
{
    Game newGame(10);
   
    
    cout << "exiting" << endl;

    return 0;
}