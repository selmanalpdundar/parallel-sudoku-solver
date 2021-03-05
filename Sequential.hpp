/**
 * @file Sequential.cpp
 * @author Selman Alpdündar (selman.alp@hotmail.com.tr)
 * @brief
 * @version 0.1
 * @date 2020-08-21
 *
 * @copyright Copyright (c) 2020
 *
 */

#ifndef Sequential_hpp
#define Sequential_hpp

#include <stdio.h>
#include <vector>
#include <algorithm>
#include <iostream>
#include <tuple>
#include "Benchmark.hpp"
#include "Position.hpp"
#include <functional>
#include <cstdlib>
#include <thread> // std::this_thread::sleep_for
#include <chrono> // std::chrono::seconds
#include "Game.hpp"

using namespace std;

class Sequential
{

private:
    /**
     * @brief
     * keeps all possible number for each empty positions in the board.
     */
    vector<tuple<Position, vector<int>>> possibleValues;

    /**
     * @brief
     * takes a row position and a number then checks if it is in the given row or not.
     * @param row is the row number.
     * @param number is the number that will be checked in row.
     * @return true when it is in the given row.
     * @return false when it is not in the given row.
     */
    bool isInRow(int board[9][9], int row, int number);

    /**
     * @brief
     * takes a column position and a number then checks if it is in the column or not.
     * @param column is the column number.
     * @param number is the number that will be checked in column.
     * @return true when it is in the given column.
     * @return false when it is not in the given column.
     */
    bool isInColumn(int board[9][9], int column, int number);

    /**
     * @brief
     * takes a row position, a column position and a number then checks if it is in the box or not.
     * @param row is the row number.
     * @param column is the column number.
     * @param number is the number that will be checked in the box.
     * @return true when it is in the box.
     * @return false when it is not in the box.
     */
    bool isInBox(int board[9][9], int row, int column, int number);

    /**
     * @brief
     *  takes a row number position, a column number position and a number then check if it is safe to use given number in given position.
     * @param row is the row number.
     * @param column is the column number.
     * @param number is the number that will be checked if it is safe or not.
     * @return true when it is safe to use number.
     * @return false when it is not safe to use number.
     */
    bool isSafe(int board[9][9], int row, int column, int number);

    /**
     *
     * @param board is the game board.
     * @param row is the row number.
     * @param column is the column number.
     * @return return int vector which contains possible number that can be placed in given position.
     */
    vector<int> getPossibleNumbers(int board[9][9], int row, int column);

    /**
     * @param board is the game board.
     * @return vector of tuple which contain possible values that can be placed for empty cells.
     */
    vector<tuple<Position, vector<int>>> getAllPossibleValuesForEmptyCells(int board[9][9]);

    /**
     * @brief Get the Possible Numbers For Given Cell object
     *
     * @param possibleValues is the all possible values for empty cells.
     * @param givenPosition is the position of cell.
     * @return vector<int> is the possible numbers that can be replaced for given cell.
     */
    vector<int> getPossibleNumbersForGivenCell(vector<tuple<Position, vector<int>>> possibleValues, Position givenPosition);

public:
    /**
    * @brief
    * When ever the board solved, the result will be replaced on solved board.
    */
    int solvedBoard[9][9];

    /**
     * Constructor.
     * @param board is game board.
     */
    Sequential(int board[9][9]);

    /**
     * @brief
     * takes a game which contains all possible values for empty cells and  a counter.
     * @param game is game which has index, sudoku board, in all possible number within
     */
    bool solve(Game game);

    /**
    * @brief
    *  prints solved board.
    */
    void print();
};
#endif /* Sequential_hpp */
