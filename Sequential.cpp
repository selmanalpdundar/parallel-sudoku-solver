
#include "Sequential.hpp"

Sequential::Sequential(int board[9][9])
{
    /*
    *  Game is a struct I am using it to pass data that is need to run solve algorithm.
    *  Array pass reference by default therefore I am using struct to pass by value.
    */
    Game game;

    // Copying given board to game board
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            game.board[i][j] = board[i][j];
        }
    }

    // Getting all possible number for empty cells in board.
    game.possibleValues = this->getAllPossibleValuesForEmptyCells(game.board);

    // This is used to see which cell we filled before.
    game.currentIndex = 0;
    
    {
        Benchmark benchmark("Sequential Implementation");
        // Start solving game
        this->solve(game);
        this->print();
    }
    

}

bool Sequential::solve(Game game)
{
    /**
     * @brief
     *  The logic here is we are using sorted vector which is sorted increasingly by possible number that can be used for a empty cell.
     *  Check if all empty cell filled before or not, because the size of vector equals empty cell.
     */
    if (game.currentIndex < game.possibleValues.size())
    {
        // gets an empty cell's position
        Position position = std::get<0>(game.possibleValues.at(game.currentIndex));
        
        // increase current index to move next cell.
        game.currentIndex++;

        // gets all possible numbers for current position.
        vector<int> numbers = getPossibleNumbersForGivenCell(game.possibleValues, position);

        // iterates all possible numbers for given cell
        for (int i = 0; i < numbers.size(); i++)
        {
            // checks current number to see if it is safe to use in board.
            if (this->isSafe(game.board, position.row, position.column, numbers.at(i)))
            {
                // if it is safe, using it as a cell value.
                game.board[position.row][position.column] = numbers.at(i);

                /**
                 * @brief
                 *  After replacing empty cell with a possible number we create sub tree which will go and try to solve next cell position.
                 *  the logic is here repeat this process for all possible number until which eventually lead a valid path.
                 *  We are using back tracking algorithm therefore we will try next number if and only if the current choice of number is reach dead end.
                 */

                if(this->solve((game))){
                    return true;
                }

                game.board[position.row][position.column] = 0;
            }
        }
    }
    else
    {
        /**
         * @brief
         *  if there is no empty cell it means it is solved
         *  we can copy this game board to solved board.
         */
        for (int i = 0; i < 9; i++)
        {
            for (int j = 0; j < 9; j++)
            {
                this->solvedBoard[i][j] = game.board[i][j];
            }
        }

        return true;
    }

    return false;
}

bool compareSequential(tuple<Position, vector<int>> first, tuple<Position, vector<int>> second)
{
    // compare possible values with size.
    return get<1>(first).size() < get<1>(second).size();
}

vector<tuple<Position, vector<int>>> Sequential::getAllPossibleValuesForEmptyCells(int board[9][9])
{
    /*
        I created an empty vector becuase
        I do not know number of empty cell I need something dynamic.
     */
    vector<tuple<Position, vector<int>>> emptCellsPossibleValues;

    /*
        I visited all empty cell and get possible number for this empty cell.
        After that I create a tuple with this cell position.
     */
    for (int row = 0; row < 9; row++)
    {
        for (int column = 0; column < 9; column++)
        {
            if (board[row][column] == 0)
            {
                Position position(row, column);
                emptCellsPossibleValues.push_back(make_tuple(position, this->getPossibleNumbers(board, row, column)));
            }
        }
    }

    /*
        In the lesson professor suggested us to go for a cell which has less possible number than others,
        therefore, I sort them by number of possible number before return.
     */
    sort(emptCellsPossibleValues.begin(), emptCellsPossibleValues.end(), compareSequential);

    /*
        It is the vector of tuple which is sorted increasing order.
     */
    return emptCellsPossibleValues;
}

vector<int> Sequential::getPossibleNumbers(int board[9][9], int row, int column)
{

    vector<int> numbers;

    /*
        I iterate from 1 to 9 and check if it is safe to use in this cell.
     */
    for (int i = 1; i <= 9; i++)
    {
        if (isSafe(board, row, column, i))
        {
            numbers.push_back(i);
        }
    }
    return numbers;
}

vector<int> Sequential::getPossibleNumbersForGivenCell(vector<tuple<Position, vector<int>>> possibleValues, Position givenPosition)
{
    /*
        I get a cell position and iterate for this position inside possible values for empty cells
     when ever then position found return all numbers for this position.
     */
    for (int i = 0; i < possibleValues.size(); i++)
    {
        Position position = get<0>(possibleValues.at(i));
        if (position.row == givenPosition.row && position.column == givenPosition.column)
        {
            return get<1>(possibleValues.at(i));
        }
    }

    return vector<int>();
}

bool Sequential::isInRow(int board[9][9], int row, int number)
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

bool Sequential::isInColumn(int board[9][9], int column, int number)
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

bool Sequential::isInBox(int board[9][9], int row, int column, int number)
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

bool Sequential::isSafe(int board[9][9], int row, int column, int number)
{

    int boxRowStart = 3 * (row / 3);
    int boxColumnStart = 3 * (column / 3);

    if (!isInBox(board, boxRowStart, boxColumnStart, number) &&
        !isInRow(board, row, number) && !isInColumn(board, column, number))
        return true;

    return false;
}

void Sequential::print()
{
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            if (this->solvedBoard[i][j] == 0)
            {
                std::cout << ".";
            }
            else
            {
                std::cout << this->solvedBoard[i][j];
            }
            std::cout << "|";
        }
        std::cout << endl;
    }
    std::cout << endl;
}
