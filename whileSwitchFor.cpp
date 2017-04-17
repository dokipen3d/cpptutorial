#include <iostream>

using namespace std;

//http://en.cppreference.com/w/cpp/language/switch


int main()
{
    bool isPlaying = true;
    char command;
    
    while (isPlaying){
        
        cout << "What would you like to do? Type 'p' to play the game or 'e' to exit" << endl;
        cin >> command;
        
        switch(command) {
            case 'P' :
            case 'p' :  cout << "playing game" << endl;
                        for (int j = 0; j < 8; ++j){
                            for (int i = 0; i < 8; ++i){
                                cout << 0 << "  ";
                            }
                            
                            cout << endl;
                        }
                        
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


    
