#include <iostream>
#include <vector>
#include <array>
#include <memory>
#include <algorithm>

using namespace std;

//functions
//reference arguments

enum class GameStatus{
        MENU,
        PLAYING,
        EXITING
    };

class gridClass{
    
public:
    //constructor
    gridClass(int inWidthHeight) :  widthHeight(inWidthHeight), 
                                    grid(inWidthHeight*inWidthHeight, 0){}
    
    //we know the width so can just take an x and y
    int Convert2dTo1D(int x, int y){
        return x + y*widthHeight;
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
    
    //clear ourself
    void clear(){
        fill( begin(grid), end(grid), 0);
    }
    
private:
    int widthHeight = 8;
    vector<int> grid;
};

int main()
{
    GameStatus eStatus = GameStatus::MENU;
    char command;
    
    gridClass myGrid(8);
    
    myGrid.clear();
    
    cout << endl;

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
    
    cout << "exiting" << endl;

    return 0;
}


    
