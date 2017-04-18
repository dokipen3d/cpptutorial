#include <iostream>
#include <vector>
#include <array>
#include <memory>
#include <algorithm>
#include <map>

using namespace std;

//http://en.cppreference.com/w/cpp/language/operators
//https://msdn.microsoft.com/en-us/library/1z2f6c2k.aspx

//actually change to vec of ships/
//place ships in grid with random placement
//init grid constructor with Ship type of Sea
//provide << operator for seaItems. show how display just works


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
    
    //map to get ship count for placing 
    map<eShip, int> shipCount{
        {eShip::Carrier, 1},
        {eShip::Battleship, 2},
        {eShip::Cruiser,3},
        {eShip::Submarine,4},
        {eShip::Destroyer,5}
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
            // write obj to stream USE OBJ!
            return os << BattleShip::shipDisplayIcon[obj.type];
        }
        
        void Display(){
            cout << BattleShip::shipDisplayIcon[type];
        }
        
        virtual void Hit(){
            
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
                                {}
    
    //wrapping up into a class means we dont need to provide args. it can call itself
    void displayGrid(){
        for (int j = 0; j < widthHeight; ++j){
            for (int i = 0; i < widthHeight; ++i){
                cout  << grid[Convert2dTo1D(i,j)] << "  ";
            }
                 
            cout << endl;
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
    
    int widthHeight = 8;
    //important to specify namespace
    vector<BattleShip::SeaItem> grid;
    
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
    Game newGame(8);
   
    
    cout << "exiting" << endl;

    return 0;
}


    
