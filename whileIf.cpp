#include <iostream>

using namespace std;

int main()
{
    bool isPlaying = true;
    string command;
    
    while (isPlaying){
        
        cout << "What would you like to do? Type \"play\" to play the game or \"exit\" to exit" << endl;
        cin >> command;
        
        if (command == "play"){
            cout << "playing game" << endl;
        }
        else if(command == "exit"){
            cout << "exiting game" << endl;
            isPlaying = false;
        }
        else{
            cout << "you typed the wrong thing. Please try again" << endl;
        }
        
    }
    
    cout << "exiting" << endl;

    return 0;
}


    
