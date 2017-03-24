/*************************************************************************************
 * CS 445 Machine Learning
 * HW 6: Reinforcement Learning "Robby the Robot"
 * Author: Philip Troy Routley
 * Date: Winter 2017
 ************************************************************************************/
#include "robot.h"
#include "grid.h"
#include "robby.h"
#include <cstdlib>
#include <ncurses.h>
#include <iostream>
#include <string>

//contructor: place robot in random grid location
Robot::Robot(Grid *home)
{
    x = rand()%10;
    y = rand()%10;
#ifdef mk5
    sensor.facing = rand()%4;
#endif
    oldx = x;
    oldy = y;
    grid = home;
}
//ping queries the robot's surroundings to populate his sensor
int Robot::ping()
{
    sensor.n = grid->getContents(x,y-1);
    sensor.e = grid->getContents(x+1,y);
    sensor.w = grid->getContents(x-1,y);
    sensor.s = grid->getContents(x,y+1);
    sensor.h = grid->getContents(x,y);
#ifdef mk5
    state = sensor.facing * 243 + sensor.n*81 + sensor.e*27 + sensor.w*9
            + sensor.s*3 + sensor.h;
#else
    state = sensor.n*81 + sensor.e*27 + sensor.w*9
            + sensor.s*3 + sensor.h;
#endif
    
    return state;
}
void Robot::printwin(WINDOW *win)
{   
    mvwprintw(win,oldy+1, oldx+oldx+1, " ");
#ifdef mk5
    mvwprintw(win,y+1,x+x+1,fString[sensor.facing].c_str());
#else
    mvwprintw(win,y+1,x+x+1,"X");
#endif
    oldx = x;
    oldy = y;
}
double Robot::north()
{
    if (y==0) return WALLCRASH;
    else 
    {
        y--;
#ifdef mk5
        sensor.facing = int(N);
#endif
    }
    return 0.0;
}
double Robot::east()
{
    if (x==9) return WALLCRASH;
    else 
    {
        x++;
#ifdef mk5
        sensor.facing = int(E);
#endif
    }
    return 0.0;
}
double Robot::west()
{
    if (x==0) return WALLCRASH;
    else 
    {
        x--;
#ifdef mk5
        sensor.facing = int(W);
#endif
    }
    return 0.0;
}
double Robot::south()
{
    if (y==9) return WALLCRASH;
    else 
    {
        y++;
#ifdef mk5
        sensor.facing = int(S);
#endif
    }
    return 0.0;
}
double Robot::pickup()
{
    if (grid->squares[y][x] == NOTHING) return FAILCAN;
    else if (grid->squares[y][x] == CAN)
    {
        grid->squares[y][x] = NOTHING;
        return PICKUPCAN;
    }
    else
    {
        std::cout << "Error: unknown type " << grid->squares[y][x]
             << " at grid " << x << ", " << y << "\n";
    }
    return 0.0;
}

#ifdef mk5
const std::string Robot::fString[] = { "^", ">", "<", "v" };
#endif
