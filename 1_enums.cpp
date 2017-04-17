#include <iostream>
#include <vector>
#include <array>
#include <memory>
#include <algorithm>

using namespace std;

enum class GameStatus{
    MENU,
    PLAYING,
    EXITING
};

constexpr int widthHeight = 8;

int Convert2dTo1D(int x, int y, int inWidthHeight){
    return x + y*inWidthHeight;
}

//provide default value for width to allow calling displayGrid(grid);
void displayGrid(vector<int>& inVector, int inWidthHeight = widthHeight){
    for (int j = 0; j < inWidthHeight; ++j){
        for (int i = 0; i < inWidthHeight; ++i){
            cout  << inVector[Convert2dTo1D(i,j,inWidthHeight)] << "  ";
            }
                     
            cout << endl;
        }
}

//what happens if we dont put &?
void clearGrid(vector<int>& inVector){
    fill( begin(inVector), end(inVector), 0);
}

int main()
{
    GameStatus eStatus = GameStatus::MENU;
    char command;
    
    vector<int> grid(widthHeight*widthHeight);
    clearGrid(grid);
    
    cout << endl;

    while ((eStatus == GameStatus::MENU) || (eStatus == GameStatus::PLAYING)){
        
        cout << "What would you like to do? Type 'p' to play the game or 'e' to exit" << endl;
        cin >> command;
        
        switch(command) {
            case 'P' :
            case 'p' :  cout << "playing game" << endl;
                        displayGrid(grid);
                        //eStatus = GameStatus::EXITING;
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


    
