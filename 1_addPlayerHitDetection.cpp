#include <iostream>
#include <vector>
#include <array>
#include <memory>
#include <algorithm>
#include <map>
#include <random>
#include <chrono>

using namespace std;

//http://en.cppreference.com/w/cpp/language/operators
//https://msdn.microsoft.com/en-us/library/1z2f6c2k.aspx

//add a hit and miss status
//add two players (so two grids). actuall make grid the base of player. then will inherit and override functions
//currently have vector of sea items (Which ships are also sea items)
//hit function is going to be the other players guess
//change its return type to return a HIT or a MISS
//make index/orient in ship public for easy setting (will change later)
//change placement to make randoness more consistent

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
                bHit = true;
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
    Player(int inWidthHeight) :   widthHeight(inWidthHeight), 
                                grid(inWidthHeight*inWidthHeight, 
                                BattleShip::SeaItem(BattleShip::eShipStatus::Sea))
                                {
                                    generator.seed(42);
                                    placeShips(); 
                                }
    
    //wrapping up into a class means we dont need to provide args. it can call itself
    void displayGrid(){
        for (int j = 0; j < widthHeight; ++j){
            for (int i = 0; i < widthHeight; ++i){
                cout  << grid[Convert2dTo1D(i,j)] << "  ";
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
                    
                    BattleShip::eShipStatus testShip = grid[Convert2dTo1D(row+r,coloumn+c)].Type();
                
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
                        
                        BattleShip::Ship newShip = BattleShip::Ship(shipSize.first);
                        
                        newShip.orientation = orientationToSet;
                        newShip.index = row;
                        newShip.fromPos = coloumn;

                        grid[Convert2dTo1D(row+r,coloumn+c)] = newShip;

                       
                        cout << " " << row << " " << coloumn << " " << endl;
                        

                         tryToPlace = false;
                     }
                }
            }//end while
        }
    }
    
    //clear ourself
    void clear(){
        fill( begin(grid), end(grid), BattleShip::SeaItem(BattleShip::eShipStatus::Sea));
    }


    
private:
    //we know the width so can just take an x and y
    int Convert2dTo1D(int x, int y){
        return x + y*widthHeight;
    }
    
    int widthHeight = 10;

    //important to specify namespace
    vector<BattleShip::SeaItem> grid;
    
    //for random number generation
    random_device rd;
    mt19937 generator; //seed with 0. can seed with  std::random_device rd;
    
    
};

class Game{
public:
    Game(int gridSize) : myGrid(gridSize){
        cout << "game is created with gridSize: " << gridSize << "!" << endl;
        //myGrid.clear();
        startGameLoop();
    }


    void startGameLoop(){
        while ((eStatus == GameStatus::MENU) || (eStatus == GameStatus::PLAYING)){
        
        cout << "What would you like to do? Type 'p' to play the game or 'e' to exit" << endl;
        cin >> command;
        
        switch(command) {
            case 'P' :
            case 'p' :  cout << "playing game" << endl;
                        myGrid.displayGrid();
                        eStatus = GameStatus::EXITING;
                        break;
                        
            case 'E' :
            case 'e' :  cout << "exiting game" << endl;
                        eStatus = GameStatus::EXITING;
                        break;
                        
            default :   cout << "you typed the wrong thing. Please try again" << endl;
        }
    }
    }
    
    enum class GameStatus{
        MENU,
        PLAYING,
        EXITING
    };
    
private:
    GameStatus eStatus = GameStatus::MENU;
    char command;
    Player myGrid;
};

int main()
{
    Game newGame(10);
   
    
    cout << "exiting" << endl;

    return 0;
}