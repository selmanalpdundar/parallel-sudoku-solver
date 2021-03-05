#ifndef Position_hpp
#define Position_hpp

#include <stdio.h>

class Position
{
public:
    int row;
    int column;
    Position(int row, int column)
    {
        this->row = row;
        this->column = column;
    }
};
#endif
