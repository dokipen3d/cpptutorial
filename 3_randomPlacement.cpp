#include <iostream>
#include <vector>
#include <array>
#include <memory>
#include <algorithm>
#include <map>
#include <random>
#include <ctime>

using namespace std;

//http://en.cppreference.com/w/cpp/language/operators
//https://msdn.microsoft.com/en-us/library/1z2f6c2k.aspx


//place ships!
//add two players (so two grids)
//both based on a base player object
//one AI, other human
//might want to display different aspects of the grid depending on if it is a
//human or AI (or might be over the network etc)


//everything we do in here is guarded by scope. this is used to put misc data about the game
namespace BattleShip{
    
    //enum for different kinds of ships allowed
    enum class eShip{
        Carrier,
        Battleship,
        Cruiser,
        Submarine,
        Destroyer,
        Sea
    };
    
    //map to get ship size based on type
    map<eShip, int> shipSize{
        {eShip::Carrier, 5},
        {eShip::Battleship, 4},
        {eShip::Cruiser,3},
        {eShip::Submarine,3},
        {eShip::Destroyer,2}
    };
    
   
    
    //display character
    map<eShip, char> shipDisplayIcon{
        {eShip::Carrier, 'C'},
        {eShip::Battleship, 'B'},
        {eShip::Cruiser, 'c'},
        {eShip::Submarine, 'S'},
        {eShip::Destroyer, 'D'},
        {eShip::Sea, '~'}
    };
    
    //could be bool but how to interpret true or false? call it is up?
    enum class eOrientation{
        VERTICAL,
        HORIZONTAL
    };
    
    //base for sea items to go in grid
    class SeaItem{
    public:
        SeaItem(eShip shipType)  : type(shipType){}
        
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
            cout << BattleShip::shipDisplayIcon[type];
        }
        
        virtual void Hit(){
            
        }
        
        eShip Type(){
            return type;
        }
    private:      
        //for default contructor
        eShip type = eShip::Sea;

    };
    
    //first we are filling each position with a ship. naive
    class Ship : SeaItem{
    public:
        Ship(eShip shipType)  : SeaItem(shipType){}
        
        virtual void Hit() override{
             cout << "I was hit!" << endl;
        }
         
    private:
        
        //later will get rid of these but for now ship keeps track of where it is.
        eOrientation orientation;
        //might be col or row depending on orientation 
        int index;
        //so we can search and count damage
        int fromPos;
        int toPos;
        bool Damaged;
    };
}



class Grid{
    
public:
    //constructor
    //now have to initialize vector with a new SeaItem and not 0
    Grid(int inWidthHeight) :   widthHeight(inWidthHeight), 
                                grid(inWidthHeight*inWidthHeight, 
                                BattleShip::SeaItem(BattleShip::eShip::Sea))
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
            generator.seed(time(nullptr));
            bool tryToPlace = true;
            int shipLength =  shipSize.second;
            //random bool for orientation
            BattleShip::eOrientation orientation;
            //random int for col/row
            int coloumn = 0;
            int row = 0;
            int offsetMax = widthHeight-shipLength;//find offset range
            
            uniform_int_distribution<> pickFromRangeBool(0,1);
            uniform_int_distribution<> pickFromRange(0,widthHeight-2);
            uniform_int_distribution<> pickFromRangeOffset(0,offsetMax-1);
            
            while(tryToPlace){
                (pickFromRangeBool(generator) > 0) ? 
                    orientation = BattleShip::eOrientation::VERTICAL : 
                    orientation = BattleShip::eOrientation::HORIZONTAL;
 
                //could just set once and swap if VERTICAL. might make clearer
                if (orientation == BattleShip::eOrientation::VERTICAL){
                    coloumn = pickFromRange(generator);
                    row = pickFromRangeOffset(generator);
                }
                else{
                    row = pickFromRange(generator);
                    coloumn = pickFromRangeOffset(generator);
                }
            
                //so we can break inner for loop (used in condition)
                bool bStartAgain = false;
                
                for (int i = 0; (i < shipLength) && !bStartAgain; ++i){
                    
                    //change if we add to row or col
                    int r = 0;
                    int c = 0;
                    (orientation == BattleShip::eOrientation::VERTICAL) ? r = i : c = i;
                    
                    BattleShip::eShip testShip = grid[Convert2dTo1D(row+r,coloumn+c)].Type();
                
                    if (testShip != BattleShip::eShip::Sea){
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
                        (orientation == BattleShip::eOrientation::VERTICAL) ? r = i : c = i;
                        
                         grid[Convert2dTo1D(row+r,coloumn+c)] = shipSize.first;
                         tryToPlace = false;
                     }
                }
            }//end while
        }
    }
    
    //clear ourself
    void clear(){
        fill( begin(grid), end(grid), BattleShip::SeaItem(BattleShip::eShip::Sea));
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
    Grid myGrid;
};

int main()
{
    Game newGame(10);
   
    
    cout << "exiting" << endl;

    return 0;
}


    
