#include <iostream>
#include <vector>
#include <array>
#include <memory>
#include <algorithm>
#include <map>

using namespace std;

//change to vector of chars to display different types.
//show how stl allows you to change type and behaviour adapt (ie <<)
//int to char
//fill to 'X' -> fill works regardless of type!
//type up list of ships and how many
//namespaces to have enums that are not in a class but are not global either
//dont use unordered_map because enums need a hash.also might want the order for placing. 
//then change to vector of ships

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
        
        void Display(){
            cout << shipDisplayIcon[type];
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
    Grid(int inWidthHeight) :  widthHeight(inWidthHeight), 
                                    grid(inWidthHeight*inWidthHeight, 0){}
    
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
        fill( begin(grid), end(grid), 'X');
    }
    
private:
    //we know the width so can just take an x and y
    int Convert2dTo1D(int x, int y){
        return x + y*widthHeight;
    }
    
    int widthHeight = 8;
    vector<char> grid;
    
};

class Game{

public:
    Game(int gridSize) : myGrid(gridSize){
        cout << "game is created with gridSize: " << gridSize << "!" << endl;
        myGrid.clear();
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


    
