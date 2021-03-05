#include "Native.hpp"
bool Native::isInRow(int board[9][9], int row, int number)
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

bool Native::isInColumn(int board[9][9], int column, int number)
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

bool Native::isInBox(int board[9][9], int row, int column, int number)
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

bool Native::isSafe(int board[9][9], int row, int column, int number)
{

    int boxRowStart = 3 * (row / 3);
    int boxColumnStart = 3 * (column / 3);

    if (!isInBox(board, boxRowStart, boxColumnStart, number) &&
        !isInRow(board, row, number) && !isInColumn(board, column, number))
        return true;

    return false;
}

bool Native::compare(tuple<Position, vector<int>> first, tuple<Position, vector<int>> second)
{
    return get<1>(first).size() < get<1>(second).size();
}

vector<int> Native::getPossibleNumbers(int board[9][9], int row, int column)
{
    vector<int> numbers;

    for (int i = 1; i <= 9; i++)
    {
        if (isSafe(board, row, column, i))
        {
            numbers.push_back(i);
        }
    }
    return numbers;
}

vector<tuple<Position, vector<int>>> Native::getAllPossibleValuesForEmptyCells(int board[9][9])
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

    sort(emptCellsPossibleValues.begin(), emptCellsPossibleValues.end(), this->compare);

    return emptCellsPossibleValues;
}

vector<int> Native::getPossibleNumbersForGivenCell(Game game, Position givenPosition)
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

void Native::print()
{
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            if (this->board[i][j] == 0)
            {
                std::cout << ".";
            }
            else
            {
                std::cout << this->board[i][j];
            }
            std::cout << "|";
        }
        std::cout << endl;
    }
    std::cout << endl;
}

bool Native::solve(Game game)
{
    if (this->stop) return true; // Check if someone already find a solution or not.

    /** Check if all empty cell filled before or not, because the size of vector equals empty cell.  */
    if (game.currentIndex < game.possibleValues.size())
    {
        // getting all possible number for current empty cell by help of current index from game struct.
        Position position = std::get<0>(game.possibleValues.at(game.currentIndex));

        // Increase current index to next one.
        game.currentIndex++;

        // get all the possible numbers that is computed in advance
        std::vector<int> numbers = this->getPossibleNumbersForGivenCell(game, position);

        /* for debugging
        std::cout<<"Possible number count for position ["<< position.row <<"," <<position.column << "]" <<  "=" << numbers.size() << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
        */

        /*
        // for debugging
        for(int i= 0; i<numbers.size(); i++){
            std::cout<<"Possible number for position ["<< position.row <<"," <<position.column << "]" <<  "=" << numbers.at(i) << std::endl;
        }

         */
        std::vector<std::thread> *threadVector = new std::vector<std::thread>();

        // Iterate on all possible numbers for this position.
        for (int i = 0; i < numbers.size(); i++)
        {
            /*
             *@Brief
             * In the beginning we are evaluated possible numbers for positions but each time we put a number a empty cell
             * We change possibility of use of a number on a cell before using it we need to check if it is safe or not.
             */
            if (this->isSafe(game.board, position.row, position.column, numbers.at(i)))
            {
                // Updating current position with a number
                game.board[position.row][position.column] = numbers.at(i);
                // Check if there is any thread
                if (this->numberOfThread > 0)
                {
                    if (stop) return true; // before creating new thread check one more time if someone already find a solution or not
                    this->numberOfThread -= 1; // reduce thread resources

                    threadVector->push_back(std::thread(&Native::solve,this,game)); // create a new thread with new state of game.

                }
                else
                {
                    if (stop) return true; // check one more time if someone already find a solution or not

                    // Join the threads to current thread.
                    for(int i=0; i<threadVector->size(); i++){
                        if(threadVector->at(i).joinable()){
                            threadVector->at(i).join();
                        }
                    }

                    if(solve(game)) return true; // If there is no available resources, we need to call on current thread with back-tracking approach.
                    game.board[position.row][position.column] = 0;
                }

            }
        }

        // Join the threads to current thread.
        for(int i=0; i<threadVector->size(); i++){
            if(threadVector->at(i).joinable()){
                threadVector->at(i).join();
            }
        }

        return false;
    }
    else
    {
        /**
          if there is no empty cell it means all the cell is filled therefore we can stop looking for a solution and save current state of game as a final solution
         */
        this->stop = true; // make stop  flag true therefore other Native can stop looking for a solution

        /** Save current game to final board */
        for (int i = 0; i < 9; i++)
        {
            for (int j = 0; j < 9; j++)
            {
                this->board[i][j] = game.board[i][j];
            }
        }
        return true;
    }
}

Native::Native(int board[9][9], int degree)
{
    Game game;
    this->numberOfThread = degree;
    this->stop  = false;

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
        Benchmark benchmark("Native Thread Implementation");
        this->solve(game);
        this->print();
    }

}
