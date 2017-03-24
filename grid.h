/*************************************************************************************
 * CS 445 Machine Learning
 * HW 6: Reinforcement Learning "Robby the Robot"
 * Author: Philip Troy Routley
 * Date: Winter 2017
 ************************************************************************************/

#ifndef GRID_H
#define GRID_H

#include "robot.h"
#include <ncurses.h>

enum Contents { NOTHING, WALL, CAN };

class Robot;

class Grid
{
    public:
        Grid();
        Contents squares[10][10];
        Contents getContents(int x, int y);
        void print();
        void printwin(WINDOW *grid_win);
};

#endif