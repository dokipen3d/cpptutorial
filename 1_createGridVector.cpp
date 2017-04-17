#include <iostream>
#include <vector>
#include <array>
#include <memory>
#include <algorithm>

using namespace std;

//http://en.cppreference.com/w/cpp/algorithm

//introduce vector
//constexpr
//range based for
//fill algorithm
//2d access to grid with [] operator

int main()
{
    bool isPlaying = true;
    char command;
    
    constexpr int widthHeight = 8;
    
    vector<int> grid(widthHeight*widthHeight);

    //init using range-for loop
    for (auto& pos : grid){
        pos = 0;
    }
    
    //using algorithm
    fill( begin(grid), end(grid), 0);
    
    cout << endl;

    while (isPlaying){
        
        cout << "What would you like to do? Type 'p' to play the game or 'e' to exit" << endl;
        cin >> command;
        
        switch(command) {
            case 'P' :
            case 'p' :  cout << "playing game" << endl;
                        for (int j = 0; j < 8; ++j){
                            for (int i = 0; i < 8; ++i){
                                cout  << grid[i + j*widthHeight] << "  ";
                            }
                            
                            cout << endl;
                        }
                        isPlaying = false;
                        break;
                        
            case 'E' :
            case 'e' :  cout << "exiting game" << endl;
                        isPlaying = false;
                        break;
                        
            default :   cout << "you typed the wrong thing. Please try again" << endl;
        }
        
    }
    
    cout << "exiting" << endl;

    return 0;
}


    
