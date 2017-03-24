/*************************************************************************************
 * CS 445 Machine Learning
 * HW 6: Reinforcement Learning "Robby the Robot"
 * Author: Philip Troy Routley
 * Date: Winter 2017
 ************************************************************************************/

#include "robby.h"
#include "grid.h"
#include "robot.h"

#include <iostream>
#include <fstream>
#include <ncurses.h>
#include <string>
#include <cstring>
#include <cstdlib>
#include <time.h>
#include <cmath>

using namespace std;

int g_episodes = 5000;
int g_actions = 200;
double g_eta = 0.2;
double g_epsilon = -1.0;
double g_gamma = 0.9;
double g_cans = 0.5;
int g_display = 0;
double g_actiontax = 0.0;
double g_pause = 0.1;
string g_ofile ("robby_output.csv");

//function prototypes
Action bestActionForState(double Q[STATES][5], int state);
double max(double QS[5]);
void print_usage(char * programName);
void print_globals();

//main #####################################################################
int main(int argc, char *argv[])
{
    //get inputs
    if (argc > 1)
    {
        try
        {
            for (int arg_index = 1; arg_index < argc; arg_index++)
            {
                //cout << "arg_index: " << arg_index << "/" << argc - 1 << endl;
                //cout << "last arg: " << argv[argc-1] << endl;
                
                char flag = 'i'; //i = invalid flag
                if (*argv[arg_index] == '-')
                //argument is flag
                {
                    if (strlen(argv[arg_index]) == 2)
                    {
                        flag = argv[arg_index++][1];
                    }
                }
        
                //check to make sure last argument wasn't a flag without variable
                if (arg_index >= argc) throw 1;
        
                //-n episodes default 5000
                //-m actions per episode default 200
                //-l learning rate default 0.2 
                //-e constant epsilon -1.0
                //-g future discount default 0.9
                //-c can ratio default 0.5
                //-o output filename
                //-d display grid default 0 no, 1 train, 2 test, 3 both
                //-a action tax default 0.0
                //-p pause in seconds for slowing graphic display (default 0.1)
                
                //cout << "flag: " << flag << " arg: " << argv[arg_index] << endl;
                
                switch (flag) {
                case 'n':
                    g_episodes = atoi(argv[arg_index]);
                    if (g_episodes < 1) throw 3;
                    break;
                case 'm':
                    g_actions = atoi(argv[arg_index]);
                    if (g_actions < 1) throw 3;
                    break;
                case 'l':
                    g_eta = atof(argv[arg_index]);
                    if (g_eta < 0.0 || g_eta > 1.0) throw 3;
                    break;
                case 'e':
                    g_epsilon = atof(argv[arg_index]);
                    if (g_epsilon < -1.0 || g_eta > 1.0) throw 3;
                    break;
                case 'g':
                    g_gamma = atof(argv[arg_index]);
                    if (g_gamma < 0.0 || g_gamma > 1.0) throw 3;
                    break;
                case 'c':
                    g_cans = atof(argv[arg_index]);
                    if (g_cans < 0.0 || g_cans > 1.0) throw 3;
                    break;
                case 'o':
                    g_ofile = argv[arg_index];
                    break;
                case 'd':
                    g_display = atoi(argv[arg_index]);
                    if (g_display < 0 || g_display > 3) throw 3;
                    break;
                case 'a':
                    g_actiontax = atof(argv[arg_index]);
                    break;
                case 'p':
                    g_pause = atof(argv[arg_index]);
                    if (g_pause < 0.0) throw 3;
                    break;
                default:
                    throw 2;
                }
            }
        }
        catch(int e)
        {
            switch (e) {
            case 1:
                cout << "exception: to many arguments.\n";
                break;
            case 2:
                cout << "exception: invalid flag.\n";
                break;
            case 3:
                cout << "exception: out-of-range.\n";
                break;
            default:
                cout << "unknown exception.\n";
            }
            print_usage(argv[0]);
            return 1;
        }
    }
    print_globals();
    
//initialize simulation
    srand(time(NULL));
    double Q[STATES][5] = {0};
    char Qstring[33];
    Action action;
    
    struct timespec ts;             
    double intpart, fractpart;
    fractpart = modf(g_pause, &intpart);
    ts.tv_sec = int(intpart);
    ts.tv_nsec = int(fractpart*1000000000);
    
    
    ofstream ofs;
    ofs.open(g_ofile.c_str(), ofstream::out | ofstream::trunc);
    ofs << "Robby the Robot: Machine Learning HW 6\n"
        << "Episodes: " << g_episodes
        << "\nSteps per episode: " << g_actions
        << "\nLearning Rate: " << g_eta
        << "\nGamma: " << g_gamma
        << "\nAction Tax: " << g_actiontax << endl;

    Loc titleLoc = { .x = 0, .y = 0 };
    Loc trainLoc = { .x = 0, .y = 1 };
    Loc episodeLoc = { .x = 0, .y = 2 };
    Loc stepLoc = { .x = 15, .y = 2 };
    Loc gridLoc = { .x = 4, .y = 4 };
    Loc stateLoc = { .x = 0, .y = 17 };
    Loc actionLoc = { .x = 12, .y = 17 };
    Loc rewardLoc = { .x = 0, .y = 18 };
    Loc totalRewardLoc = { .x = 12, .y = 18 };
    Loc QLoc = { .x = 0, .y = 19 };
    
    initscr();
    WINDOW *grid_win;
    grid_win = newwin(12,23,gridLoc.y,gridLoc.x);
    curs_set(0);
            
    if (g_display != 0)
    {    
        wborder(grid_win,'#','#','#','#','#','#','#','#');
        mvprintw(titleLoc.y, titleLoc.x, "Robby the Robot: Machine Learning HW 6");
        mvprintw(episodeLoc.y, episodeLoc.x, "Episode:");
        episodeLoc.x += 9;
        mvprintw(stepLoc.y, stepLoc.x, "Step:");
        stepLoc.x += 6;
        mvprintw(stateLoc.y, stateLoc.x, "State:");
        stateLoc.x += 7;
        mvprintw(actionLoc.y, actionLoc.x, "Action:");
        actionLoc.x += 8;
        mvprintw(rewardLoc.y, rewardLoc.x, "Reward:");
        rewardLoc.x += 8;
        mvprintw(totalRewardLoc.y, totalRewardLoc.x, "Episode Reward:");
        totalRewardLoc.x += 16;
        refresh();
    }
    
//start simulation
//train robot
    double runningAvgReward = 0.0;
    double epsilon;
    ofs << "Training:\n0, 0, 0\n";
    if (g_display%2 == 1)
    {
        mvprintw(trainLoc.y, trainLoc.x, "Training... Epsilon:");
    }
    for (int ep = 0; ep < g_episodes; ep++)
    {
        double reward, episodeReward = 0.0;
        //new grid for episode
        Grid grid;
        //new robot on grid
        Robot robby(&grid);
        if (g_epsilon < 0.0)
        {
            //determine epsilon as 1 decaying by 0.01 per 50 episodes, floor of 0.1
            epsilon = 1.0 - 0.01 * (ep/50);
            if (epsilon < 0.1) epsilon = 0.1;
        }
        else
        {
            epsilon = g_epsilon;
        }
        
        for (int act = 0; act < g_actions; act++)
        {
            if (g_display%2 == 1)
            {
                grid.printwin(grid_win);
                robby.printwin(grid_win);
                wrefresh(grid_win);
                nanosleep(&ts, NULL);
            }
            //get state
            int state = robby.ping();
            //choose Action
            if ((double(rand())/RAND_MAX) <= epsilon)
            {
                //random action
                action = Action(rand()%5);
            }
            else
            {
                //best action
                action = bestActionForState(Q, state);
            }
            //perform Action
            switch (action) {
            case N:
                reward = robby.north();
                break;
            case E:
                reward = robby.east();
                break;
            case W:
                reward = robby.west();
                break;
            case S:
                reward = robby.south();
                break;
            case PICKUP:
                reward = robby.pickup();
            }
            reward -= g_actiontax;
            //receive reward
            episodeReward += reward;
            //observe new state
            int oldState = state;
            state = robby.ping();
            //update Q matrix
            Q[oldState][int(action)] = Q[oldState][int(action)]
                + g_eta * (reward + g_gamma * max(Q[state]) - Q[oldState][int(action)]);
        
            //update display
            if (g_display%2 == 1)
            {
                mvprintw(trainLoc.y, trainLoc.x+21, "%4.2f", epsilon);
                mvprintw(episodeLoc.y, episodeLoc.x, "%4d", ep);
                mvprintw(stepLoc.y, stepLoc.x, "%3d", act);
                mvprintw(stateLoc.y, stateLoc.x, "%3d", oldState);
                mvprintw(actionLoc.y, actionLoc.x, actionStr[int(action)]);
                mvprintw(rewardLoc.y, rewardLoc.x, "%2.0f ", reward);
                mvprintw(totalRewardLoc.y, totalRewardLoc.x, "%4.0f  ", episodeReward);
                sprintf(Qstring, "Q: %5.2f %5.2f %5.2f %5.2f %5.2f",
                    Q[oldState][0],
                    Q[oldState][1],
                    Q[oldState][2],
                    Q[oldState][3],
                    Q[oldState][4]);
                mvprintw(QLoc.y, QLoc.x, Qstring);
                refresh();
            }
        }
        
        runningAvgReward += episodeReward;
        if (ep%100==99)
        {
            runningAvgReward /= 100;
            ofs << ep+1 << ", " << episodeReward << ", " << runningAvgReward << "\n";
            runningAvgReward = 0.0;
        }
    }
    
    //test Robot
    ofs << "Testing:\n";
    if (g_display >= 2)
    {
        mvprintw(trainLoc.y, trainLoc.x, "Testing...  Epsilon:");
    }
    double totalReward = 0;
    double mean, var;
    double *epRewards = (double*)malloc(g_episodes * sizeof(double));
    
    for (int ep = 0; ep < g_episodes; ep++)
    {
        double reward, episodeReward = 0.0;
        //new grid for episode
        Grid grid;
        //new robot on grid
        Robot robby(&grid);
        //determine epsilon 0.1
        epsilon = 0.1;
        
        for (int act = 0; act < g_actions; act++)
        {
            if (g_display >=2)
            {
                grid.printwin(grid_win);
                robby.printwin(grid_win);
                wrefresh(grid_win);
                nanosleep(&ts, NULL);
            }
            //get state
            int state = robby.ping();
            //choose Action
            if ((double(rand())/RAND_MAX) <= epsilon)
            {
                //random action
                action = Action(rand()%5);
            }
            else
            {
                //best action
                action = bestActionForState(Q, state);
            }
            //perform Action
            switch (action) {
            case N:
                reward = robby.north();
                break;
            case E:
                reward = robby.east();
                break;
            case W:
                reward = robby.west();
                break;
            case S:
                reward = robby.south();
                break;
            case PICKUP:
                reward = robby.pickup();
            }
            reward -= g_actiontax;        
            //receive reward
            episodeReward += reward;
            //observe new state
            int oldState = state;
            state = robby.ping();
        
            //update display
            if (g_display >=2)
            {
                mvprintw(trainLoc.y, trainLoc.x+21, "%4.2f", epsilon);
                mvprintw(episodeLoc.y, episodeLoc.x, "%4d", ep);
                mvprintw(stepLoc.y, stepLoc.x, "%3d", act);
                mvprintw(stateLoc.y, stateLoc.x, "%3d", oldState);
                mvprintw(actionLoc.y, actionLoc.x, actionStr[int(action)]);
                mvprintw(rewardLoc.y, rewardLoc.x, "%2.0f ", reward);
                mvprintw(totalRewardLoc.y, totalRewardLoc.x, "%4.0f  ", episodeReward);
                sprintf(Qstring, "Q: %5.2f %5.2f %5.2f %5.2f %5.2f",
                    Q[oldState][0],
                    Q[oldState][1],
                    Q[oldState][2],
                    Q[oldState][3],
                    Q[oldState][4]);
                mvprintw(QLoc.y, QLoc.x, Qstring);
                refresh();
            }
        }
        totalReward += episodeReward;
        epRewards[ep] = episodeReward;
    }
    mean = totalReward / g_episodes;
    for (int ep = 0; ep < g_episodes; ep++)
    {
        var += (epRewards[ep]-mean) * (epRewards[ep]-mean) / g_episodes;
    }
    ofs << "Test Average: " << mean << " Test Std Dev: " << sqrt(var) << endl;
    
    ofs.close();
    if (g_display > 0)
    {
        getch();
    }
    endwin();
    return 0;
}

Action bestActionForState(double Q[STATES][5], int state)
{
    //find best action for state
    double best = -10000.0; //initialize to low negative
    int bestAction = 0;
    for (int eachAction = 0; eachAction < 5; eachAction++)
    {
        if (Q[state][eachAction] > best)
        {
            best = Q[state][eachAction];
            bestAction = eachAction;
        }
    }
    //if more than one best action, break ties randomly
    int bestActions[5];
    int count;
    //check each action to see if it ties for best - if so, add
    //to bestActions array
    count = 0;
    for (int eachAction = 0; eachAction < 5; eachAction++)
    {
        if (Q[state][eachAction] == best)
        {
            bestActions[count++] = eachAction;
        }
    }
    if (count == 0 || count > 5) 
    {
        cout << "Error: invalid actions: " << count
             << "\nstate: " << state 
             << "\nQ-values: ";
        for (int eachAction = 0; eachAction < 5; eachAction++)
            cout << Q[state][eachAction] << " ";
        cout << endl;
        return N;
    }
    return Action(bestActions[rand()%count]);
}

double max(double QS[5])
{
    double best = -10000.0;
    for (int index = 0; index < 5; index++)
    {
        if (QS[index] > best) best = QS[index];
    }
    return best;
}

void print_usage(char * programName)
{
    cout << "Usage:\n"
         << programName
         << " [flag variable]*\n"
         << " -n integer # of episodes (default 5000)\n"
         << " -m integer # of actions per episode (default 200)\n"
         << " -l float learning rate 0.0-1.0 (default 0.2)\n"
         << " -e double constant epsilon -1.0-1.0 (default -1.0)\n"
         << "    negative means use variable epsilon starting at 1.0 and decaying\n"
         << "    0.01 every 50 episodes to a floor of 0.1\n"
         << " -c float probability square contains a can 0.0-1.0 (default 0.5)\n"
         << " -o output filename (default \"robby_output.csv\")\n"
         << " -d print grid (default 0 = no, 1 = training, 2 = testing, 3 = both)\n"
         << " -a action tax float reward penalty for each action (default 0)\n"
         << " -p pause in seconds for grid display (default 0.1)\n";
         
    return;
}

void print_globals()
{
    cout << "Episodes: " << g_episodes
         << "\nActions: " << g_actions
         << "\nLearning Rate: " << g_eta
         << "\nGamma: " << g_gamma
         << "\nCan ratio: " << g_cans
         << "\nOutput File: " << g_ofile
         << "\nDisplay flag: " << g_display
         << "\nDisplay pause: " << g_pause << endl;
    if (g_actiontax != 0.0) cout << "Action Tax: " << g_actiontax << endl;
    if (g_epsilon >= 0.0) cout << "Constant Epsilon: " << g_epsilon << endl;
         
    return;
}