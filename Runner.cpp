#include "Sequential.hpp"
#include "Native.hpp"
#include "FastFlow.hpp"

using namespace std;

int main(int argc, const char *argv[])
{

    int board[9][9] = {
            {8,0,0,0,0,0,0,0,0},
            {0,0,3,6,0,0,0,0,0},
            {0,7,0,0,9,0,2,0,0},
            {0,5,0,0,0,7,0,0,0},
            {0,0,0,0,4,5,7,0,0},
            {0,0,0,1,0,0,0,3,0},
            {0,0,1,0,0,0,0,6,8},
            {0,0,8,5,0,0,0,1,0},
            {0,9,0,0,0,0,4,0,0}
    };

        std::cout << "Algorithm Types" << std::endl;
        std::cout << "Sequential : 1 " << std::endl;
        std::cout << "Parallel   : 2 " << std::endl;
        std::cout << "FastFlow   : 3 " << std::endl;
        
        int type = 0;
        std::cin >> type;
        
        if(type == 1)
        {

            new Sequential(board);

        }
        else if (type == 2 )
        {
            std::cout << " Parallel degree: ";
            int n = 1;
            std::cin >>n;

            new Native(board, n);


        }
        else if (type == 3){
            std::cout << " Parallel degree : ";
            int n = 1;
            std::cin >>n;

            new FastFlow(board, n);

        }
   
    return 0;
}

