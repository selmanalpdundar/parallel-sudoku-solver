#include <stdio.h>
#include "Position.hpp"
#include <vector>
#include <tuple>

#ifndef Game_hpp
#define Game_hpp

typedef struct
{
    int board[9][9];
    std::vector<std::tuple<Position, std::vector<int>>> possibleValues;
    int currentIndex;
} Game;


#endif
