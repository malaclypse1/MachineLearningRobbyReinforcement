Robby the Robot
CS445 Machine Learning
Homework 6
By: Troy Routley

Quickstart: to compile and visually see a trained Robby picking up cans:

make
./robby -d 2

For experiments 1-4, compile robby with the command:

make

For experiment 5: compile Robby mk5:

make clean
make mk5=1

Usage:
./robby [flag variable]*
 -n integer # of episodes (default 5000)
 -m integer # of actions per episode (default 200)
 -l float learning rate 0.0-1.0 (default 0.2)
 -e double constant epsilon -1.0-1.0 (default -1.0)
    negative means use variable epsilon starting at 1.0 and decaying
    0.01 every 50 episodes to a floor of 0.1
 -c float probability square contains a can 0.0-1.0 (default 0.5)
 -o output filename (default "robby_output.csv")
 -d print grid (default 0 = no, 1 = training, 2 = testing, 3 = both)
 -a action tax float reward penalty for each action (default 0)
 -p pause in seconds for grid display (default 0.1)
 
