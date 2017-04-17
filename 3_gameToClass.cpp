#include <iostream>
#include <vector>
#include <array>
#include <memory>
#include <algorithm>

using namespace std;

//functions
//reference arguments



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
        fill( begin(grid), end(grid), 0);
    }
    
private:
    //we know the width so can just take an x and y
    int Convert2dTo1D(int x, int y){
        return x + y*widthHeight;
    }
    int widthHeight = 8;
    vector<int> grid;
    
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


    
