#include "Maze.h"

using namespace std;

int main() {

    //Create a maze object
    Maze maze;
    //Read in a maze text file
    maze.ReadTextFile("test2.txt");
    //Solve the maze
    maze.Solve(maze);
    //Write out the solved maze to a text file
    maze.WriteTextFile("testSolved2.txt");

    return 0;
}