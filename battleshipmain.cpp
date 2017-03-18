#include <iostream>
#include <vector>
#include <chrono>
#include <thread>
#include <cstdlib> // for system clear
#include <string>
#include <tuple>
#include <utility> //pair

//access namespaces
using namespace std;
using namespace std::this_thread;     // sleep_for, sleep_until
using namespace std::chrono_literals; // ns, us, ms, s, h, etc.

//################################################################
constexpr int rows = 10;
constexpr int coloumns = 10;
//vector of tuples. tuples cant use use default initialization so use make tuple then turn to auto //vector<tuple<int, string, int>>
//countm name and size of ships
using tupleShip = tuple<int, string, int> ;
const  vector<tupleShip> myVecTuple { 
                                                make_tuple(1, "Carrier", 5),
                                                make_tuple(2, "Battleship", 4),
                                                make_tuple(3, "Cruiser", 3),
                                                make_tuple(4, "Submarine", 3),
                                                make_tuple(5, "Destroyer", 2)
                                            };

const  vector<pair<string, int>> myVecPair { 
                                                { "Carrier", 5},
                                               {"Battleship", 4},
                                                {"Cruiser", 3},
                                                { "Submarine", 3},
                                                {"Destroyer", 2}
                                            };

//________________________________________________________________

//#########################################################
vector<int> createGrid(int width, int height){
    return vector<int>(rows*coloumns);
}//________________________________________________________________

//################################################################
int coord2dto1d(int x, int y){
    if ((x > rows) || (y > coloumns)){
        cout << "Bounds exceeded, please enter correct coordinates, exiting\n";
        return 0; 
    }
    
    return x + y*coloumns;
}//________________________________________________________________


//################################################################
void printGrid(const vector<int>& inGrid){
    for(int i = 0; i < rows; i++){
        for (int j = 0; j < coloumns; j++){
            //cout << "element value is: " << grid[coord2dto1d(i,j)];
            cout << inGrid[coord2dto1d(i,j)] << " "; 

        }
        cout << "\n";
    }
}//________________________________________________________________

//ANSI character to clear screen
void clearScreen(){
    cout << "\033[2J";
}//________________________________________________________________


//################################################################
//using platform commands to clear
void clear_screen()
{
#ifdef WINDOWS
    std::system("cls");
#else
    // Assume POSIX
    std::system ("clear");
#endif
}//________________________________________________________________


//################################################################
void clear_screen2(){

std::cout << 1 << "\E[H\E[2J";
}//________________________________________________________________

//fill inship positions
void fillGridWithShips(const vector<int>& inGrid){
    //debug gets
    for(const auto& elem: myVecPair){
        cout << get<0>(elem) << ": " << get<1>(elem) << "\n";
    }
}

//Example of templates to help with loops
template<typename Func>
inline void nested_loop_template(Func f, vector<int>& inGrid, int gridWidth)
{
    for (int i=0; i<gridWidth; ++i) 
      for (int j=0; j<gridWidth; ++j) 
        { 
           f(inGrid, i ,j); //call function with this signature;
        } 
}

void setToTwo(vector<int>& inGrid, int xCoord, int yCoord){
    inGrid[coord2dto1d(xCoord, yCoord)] = 2;

}

//################################################################
int main(){

    clear_screen();

    auto grid = createGrid(rows, coloumns);
    fill(grid.begin(), grid.end(), 0);
    printGrid(grid);
    sleep_for(1s);
    clear_screen();
    fillGridWithShips(grid);

    return 0;
}
//________________________________________________________________


/*example of calling template loop
nested_loop_template(setToTwo, grid, rows);
*/