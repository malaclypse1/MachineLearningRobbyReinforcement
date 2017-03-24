/*************************************************************************************
 * CS 445 Machine Learning
 * HW 6: Reinforcement Learning "Robby the Robot"
 * Author: Philip Troy Routley
 * Date: Winter 2017
 ************************************************************************************/
#include "robot.h"
#include "robby.h"
#include "grid.h"
#include <cstdlib>
#include <iostream>
#include <ncurses.h>


Grid::Grid()
{
    for (int y = 0; y < 10; y++)
    {
        for (int x = 0; x < 10; x++)
        {
            if ((double(rand())/RAND_MAX) <= g_cans)
            {
                squares[y][x] = CAN;
            }
            else squares[y][x] = NOTHING;
        }
    }
}
//return contents of given grid square
Contents Grid::getContents(int x, int y)
{
    if (x<0 || x>9 || y<0 || y>9)
        return WALL;
    else
        return squares[y][x];
}

void Grid::print()
{
    std::cout << "#######################\n";
    for (int y = 0; y < 10; y++)
    {
        std::cout << "#";
        for (int x = 0; x < 10; x++)
        {
            std::cout << ' ' << (squares[y][x] == CAN ? 'o' : '.');
        }
        std::cout << " #\n";
    }
    std::cout << "#######################\n";
}

void Grid::printwin(WINDOW *grid_win)
{
    for (int y = 0; y < 10; y++)
    {
        char line[22];
        line[0] = ' ';
        for (int x = 0; x < 10; x++)
        {
            line[x+x+1] = (squares[y][x] == CAN ? 'o' : '.');
            line[x+x+2] = ' ';
        }
        line[21]=0;
        mvwprintw(grid_win, y+1, 1, line);
    }
}