/*************************************************************************************
 * CS 445 Machine Learning
 * HW 6: Reinforcement Learning "Robby the Robot"
 * Author: Philip Troy Routley
 * Date: Winter 2017
 ************************************************************************************/

#ifndef ROBOT_H
#define ROBOT_H

#include "grid.h"
#include <ncurses.h>
#include <string>

struct _sensor
{
#ifdef mk5
    int facing;
#endif
    int n, e, w, s, h;
};

class Grid;

class Robot
{
    private:
        _sensor sensor;
        int x,y, oldx, oldy;
        Grid *grid;
#ifdef mk5
        static const std::string fString[];
#endif
    public:
        int state;
        Robot(Grid *home);
        int ping();
        void printwin(WINDOW *win);
        double north();
        double east();
        double west();
        double south();
        double pickup();
};

#endif