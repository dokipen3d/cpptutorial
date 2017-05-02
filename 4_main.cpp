#include <iostream>
#include "4_Player.h"
#include "4_SeaItem.h"
#include "4_BattleShipUtility.hpp"


//http://en.cppreference.com/w/cpp/language/operators
//https://msdn.microsoft.com/en-us/library/1z2f6c2k.aspx

//add unique pointer.
//use make unique everywhere
//have to use move to pass ownership
// cant copy so when initializing cant do it in class constructor






class Game{
public:
    Game(int gridSize) : me(gridSize), AI(gridSize) {
        std::cout << "game is created with gridSize: " << gridSize << "!" << std::endl;
        //myGrid.clear();
        startGameLoop();
    }


    void startGameLoop() {
        while ((eStatus == GameStatus::MENU) || (eStatus == GameStatus::PLAYING))
        {
        
            std::cout << "What would you like to do? Type 'p' to play the game or 'e' to exit" << std::endl;
            std::cin >> command;
            
            switch(command) {
                case 'P' :
                case 'p' :  std::cout << "playing game" << std::endl;
                            eStatus = GameStatus::PLAYING;
                            break;

                
                            
                case 'E' :
                case 'e' :  std::cout << "exiting game" << std::endl;
                            eStatus = GameStatus::EXITING;
                            break;
                            
                default :   std::cout << "you typed the wrong thing. Please try again" << std::endl;
            }

            while(eStatus == GameStatus::PLAYING) {
            me.displayGrid();
            std::cout << std::endl;
            AI.displayGrid();
            std::cout << std::endl;


            std::cout << "please enter a coordinate to drop a bomb!" << std::endl;
            int a{0};
            int b{0};

            std::cin >> a >> b;

           

            if(AI.AttemptHit(a,b)) {
                std::cout << "attempted hit" << std::endl;
                me.addHitPositionToTrackingGrid(a, b,
                 BattleShip::eShipStatus::HIT);
            }
            else {
                me.addHitPositionToTrackingGrid(a, b, 
                BattleShip::eShipStatus::MISS);
            }

        }

        



            


        }
    }

    
    
    enum class GameStatus {
        MENU,
        PLAYING,
        EXITING
    };
    
private:
    GameStatus eStatus = GameStatus::MENU;
    char command;
    Player me;
    Player AI;
};

int main()
{


    Game newGame(10);
   
    
    std::cout << "exiting" << std::endl;

    return 0;
}