/*************************************************************************************
 * CS 445 Machine Learning
 * HW 6: Reinforcement Learning "Robby the Robot"
 * Author: Philip Troy Routley
 * Date: Winter 2017
 ************************************************************************************/

#ifndef ROBBY_H
#define ROBBY_H

#include <string>

extern int g_episodes;
extern int g_actions;
extern double g_eta;
extern double g_epsilon;
extern double g_gamma;
extern double g_cans;
extern int g_display;
extern double g_actiontax;
extern double g_pause;
extern std::string g_ofile;

#define WALLCRASH -5.0
#define FAILCAN -1.0
#define PICKUPCAN 10.0

#ifdef mk5
#define STATES 972
#else
#define STATES 243
#endif

enum Action { N, E, W, S, PICKUP };
const char actionStr[5][7] = 
    {
        "North ",
        "East  ",
        "West  ",
        "South ",
        "Pickup"
    };

struct Loc
{
    int x, y;
};

#endif