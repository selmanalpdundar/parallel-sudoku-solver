#include "FastFlow.hpp"
std::atomic<bool> stop(false);      // Checking flag to stop other threads if one of thread find a correct way.
std::atomic<int> numberOfThread(0); // Following available threads.

int board[9][9];

bool FastFlow::isInRow(int board[9][9], int row, int number)
{
    for (int i = 0; i < 9; i++)
    {
        if (board[row][i] == number)
        {
            return true;
        }
    }
    return false;
}

bool FastFlow::isInColumn(int board[9][9], int column, int number)
{
    for (int i = 0; i < 9; i++)
    {
        if (board[i][column] == number)
        {
            return true;
        }
    }
    return false;
}

bool FastFlow::isInBox(int board[9][9], int row, int column, int number)
{
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            if (board[row + i][column + j] == number)
            {
                return true;
            }
        }
    }
    return false;
}

bool FastFlow::isSafe(int board[9][9], int row, int column, int number)
{

    int boxRowStart = 3 * (row / 3);
    int boxColumnStart = 3 * (column / 3);

    if (!isInBox(board, boxRowStart, boxColumnStart, number) &&
        !isInRow(board, row, number) && !isInColumn(board, column, number))
        return true;

    return false;
}

bool FastFlow::compare(tuple<Position, vector<int>> first, tuple<Position, vector<int>> second)
{
    return get<1>(first).size() < get<1>(second).size();
}

vector<int> FastFlow::getPossibleNumbers(int board[9][9], int row, int column)
{
    vector<int> numbers;

    for (int i = 1; i <= 9; i++)
    {
        if (this->isSafe(board, row, column, i))
        {
            numbers.push_back(i);
        }
    }
    return numbers;
}

vector<tuple<Position, vector<int>>> FastFlow::getAllPossibleValuesForEmptyCells(int board[9][9])
{
    vector<tuple<Position, vector<int>>> emptCellsPossibleValues;

    for (int row = 0; row < 9; row++)
    {
        for (int column = 0; column < 9; column++)
        {
            if (board[row][column] == 0)
            {
                Position position(row, column);
                emptCellsPossibleValues.push_back(make_tuple(position, getPossibleNumbers(board, row, column)));
            }
        }
    }

    sort(emptCellsPossibleValues.begin(), emptCellsPossibleValues.end(), &FastFlow::compare);

    return emptCellsPossibleValues;
}

vector<int> FastFlow::getPossibleNumbersForGivenCell(Game game, Position givenPosition)
{
    for (int i = 0; i < game.possibleValues.size(); i++)
    {
        Position position = get<0>(game.possibleValues.at(i));
        if (position.row == givenPosition.row && position.column == givenPosition.column)
        {
            return get<1>(game.possibleValues.at(i));
        }
    }
    throw ("Not");
}

void FastFlow::print()
{
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            if (board[i][j] == 0)
            {
                std::cout << ".";
            }
            else
            {
                std::cout << board[i][j];
            }
            std::cout << "|";
        }
        std::cout << endl;
    }
    std::cout << endl;
}

void FastFlow::solve(Game game)
{
    if (stop) return; // Check if someone alread find a solution or not.
    
    /** Check if all empty cell filled before or not, because the size of vector equals empty cell.  */
    if (game.currentIndex < game.possibleValues.size())
    {
        // getting all possible number for current empty cell by help of current index from game struct.
        Position position = std::get<0>(game.possibleValues.at(game.currentIndex));
        
        // Increase current index to next one.
        game.currentIndex++;
        
        // get all the possible numbers that is computed in advance
        vector<int> numbers = getPossibleNumbersForGivenCell(game, position);
        
        // Check if there is any resources
        if(numberOfThread  > 0 ){
            
            // get size of possible numbers
            const int size  = (int) numbers.size();
            
            // initilize degree
            int localParallelDegree = 0;
            
            /**
             check if there are enough resources for current celll to run all possible numbers within parallel degree that is equal to size of them.
             */
            if(numberOfThread >= numbers.size()){
                // subtract size from resources
                numberOfThread = numberOfThread - size;
                // assign size to local degree
                localParallelDegree = size;
            }
            else
            {
                /**
                 If there is  not enough resources get availalbe resources and assign it to local degree
                 */
                localParallelDegree = numberOfThread;
                numberOfThread = 0;
            }
            
            // declare parallel for with local parallel degree as maximum number of worker.
            ParallelFor pf(localParallelDegree);
            pf.parallel_for(0, size,    // start, stop indexes
                            1,          // step size
                            1,          // chunk size (0=static, >0=dynamic)
                            [&,this](const long i)
                            {
                                if (this->isSafe(game.board, position.row, position.column, numbers.at(i))) // Check if it is possible to use on current cell
                                {
                                    if (stop) return; // Check if someone already find a solution or not.
                                    game.board[position.row][position.column] = numbers.at(i); // assign a value to current cell
                                    this->solve(game); // call a solve in current thread which is already speared thread with current game state.
                                }
                            },
                            localParallelDegree); // parallel degreee
        }
        else
        {
            
            // If there is not any resources  iterate numbers on current thread
            for (int i = 0; i < numbers.size(); i++)
            {
                
                if (this->isSafe(game.board, position.row, position.column, numbers.at(i))) // Check if it is possible to use on current cell
                {
                    game.board[position.row][position.column] = numbers.at(i); // assign a value to current cell
                    if (stop) return; // Check if someone already find a solution or not.
                    this->solve(game); // call solve function in current thread
                
                }
            }
        }
    }
    else
    {
        // if there is no empty cell it means all the cell is filled therefore we can stop looking for a solution and save current state of game as a final solution
         
        stop = true;
        
        // Save current game to final board
        for (int i = 0; i < 9; i++)
        {
            for (int j = 0; j < 9; j++)
            {
                board[i][j] = game.board[i][j];
            }
        }
    }
}

FastFlow::FastFlow(int board[9][9], int degree)
{
    Game game;
    numberOfThread = degree;

    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            game.board[i][j] = board[i][j];
        }
    }

    game.possibleValues = getAllPossibleValuesForEmptyCells(game.board);
    game.currentIndex = 0;
        
    {
        Benchmark benchmark("FastFlow Implementation");
        solve(game);
        print();
    }
}
