#ifndef BOARD_H
#define BOARD_H

#include <iostream>
#include <cstdlib>
#include "functions.h"

using namespace std;

namespace Alex {

class board
{
public:
    board(int rowSize = 3, string start = "NULL");
    //  Pre-condition:
    //      rowSize is equal to the number of rows the board has
    //      start holds the starting position of the board (ie. 123456780)
    //
    //  Post-condition:
    //      _rowSize = rowSize
    //      _board now holds the string start

    board(const board& other);
    ~board();

    bool move(DIR dir);
    //  Post-condition:
    //      board has moved and returned true
    //          OR
    //      board has not moved and returned false

    bool canMove(DIR dir);
    //  Post-condition:
    //      checks if current board can move in direction dir
    //      returns true if possible
    //      false if not

    void display();
    //  Post-condition:
    //      outputs board to console

    bool isSolved();
    //  Post-condition:
    //      checks if board is a solved board
    //      returns true or false

    void randomize();
    //  Pre-condition:
    //      highly preferred to have srand called in main
    //  Post-condition:
    //      board has been randomized

    int boardSize();
    //  Post-condition:
    //      returns the size of the board (ie. 3x3 = board size of 9)

    void copy(const board& other);
    board& operator=(const board& other);

    int _size;
    int* _board;
    int _zeroPos;
    DIR _prevMove;
};

board::board(int rowSize, string start)
{
    //  initializes board

    _size = rowSize;
    size_t _boardSize = boardSize();
    _board = new int[_boardSize];
    _prevMove = NO_DIR;

    //  if start is initialized
    if (start != "NULL")
    {
        if (_size > 3)
        {
            cout << "Board cannot take in a starting board for sizes greater than 3" << endl;
            exit(0);
        }
        //  check if string size matches board size
        if (start.size() != _boardSize)
        {
            cout << "Starting board not consistent with board size" << endl;
            exit(0);
        }

        //  sets up board to be equal to string
        for (size_t i = 0; i < _boardSize; i++)
        {
            char c = start[i];
            int num = atoi(&c);
            _board[i] = num;

            if (num == 0)
                _zeroPos = i;
        }
    }
    else
    {
        //  sets up intial correct board
        _zeroPos = _boardSize - 1;
        for (size_t i = 0; i < _boardSize; i++)
        {
            if (i == _zeroPos)
                _board[i] = 0;
            else
                _board[i] = i + 1;
        }
    }
}

board::board(const board &other)
{
    copy(other);
}

board::~board()
{
    if (_board)
        delete[] _board;
}

bool board::move(DIR dir)
{
    //  will not move back the same way
    //  will only move if it can
    if (canMove(dir))
    {
        //  sets prevMove to current move
        _prevMove = dir;

        //  moves the 0 in the direction it needs to go
        //  updates the zeroPos
        if (dir == UP)
        {
            Swap(_board[_zeroPos], _board[_zeroPos - _size]);
            _zeroPos -= _size;
        }
        else if (dir == DOWN)
        {
            Swap(_board[_zeroPos], _board[_zeroPos + _size]);
            _zeroPos += _size;
        }
        else if (dir == LEFT)
        {
            Swap(_board[_zeroPos], _board[_zeroPos - 1]);
            _zeroPos--;
        }
        else if (dir == RIGHT)
        {
            Swap(_board[_zeroPos], _board[_zeroPos + 1]);
            _zeroPos++;
        }

        return true;
    }
    else
        return false;
}

bool board::canMove(DIR dir)
{
    if (dir == UP)
    {
        //  cannot move up if in topmost row
        if (_zeroPos >= 0 && _zeroPos < _size)
            return false;
    }
    else if (dir == DOWN)
    {
        //  starting position of last most row
        int lastRow = _size * (_size - 1);
        //  cannot move down if in lastmost row
        if (_zeroPos >= lastRow && _zeroPos < lastRow + _size)
            return false;
    }
    else if (dir == LEFT)
    {
        for (int i = 0; i < _size; i++)
        {
            //  position of leftmost row
            int leftRow = _size * i;
            //  cannot move left if in leftmost row
            if (_zeroPos == leftRow)
                return false;
        }
    }
    else if (dir == RIGHT)
    {
        for (int i = 0; i < _size; i++)
        {
            //  position of rightmost row
            int rightRow = _size * i + _size - 1;
            //  cannot move right if in rightmost row
            if (_zeroPos == rightRow)
                return false;
        }
    }
    //  default
    return true;
}

void board::display()
{
    int _boardSize = boardSize();
    for (int i = 0; i < _boardSize; i++)
    {
        //  new row
        if (i % _size == 0)
            cout << endl;

        cout << '\t' << _board[i];
    }
    cout << endl;
}

bool board::isSolved()
{
    int _boardSize = boardSize();
    //  makes sure zero is in right place
    if (_zeroPos != _boardSize - 1)
    {
        return false;
    }

    for (int i = 0; i < _boardSize; i++)
    {
        //  check to see if board matches up
        //  skips checking zeroPos
        if (i != _zeroPos)
        {
            if (_board[i] != i + 1)
            {
                return false;
            }
        }
    }

    cout << "The board is solved!" << endl;
    return true;
}

void board::randomize()
{
    //  200 random moves
    for (int i = 0; i < 200; i++)
        move(getDir(rand() % MAX_MOVES));
}

void board::copy(const board &other)
{
    //  copies other's variables
    _size = other._size;
    int _boardSize = boardSize();
    _zeroPos = other._zeroPos;
    _prevMove = other._prevMove;
    _board = new int[_boardSize];

    for (int i = 0; i < _boardSize; i++)
        _board[i] = other._board[i];
}

board& board::operator=(const board& other)
{
    copy(other);

    return *this;
}

int board::boardSize()
{
    return _size * _size;
}

}

#endif // BOARD_H
