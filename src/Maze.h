#ifndef ASSIGNMENT_2_MAZE_H
#define ASSIGNMENT_2_MAZE_H

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

using namespace std;

//Maze Position class.
class MazePosition {
    //Keep track of row, col, and whether it's a junction
public:
    int row;
    int col;
    int junc;

    MazePosition() : row(-1), col(-1), junc(-1){} //Default constructor
    MazePosition(int row, int col, int junc) : row(row), col(col), junc(junc) {} // 2 position constructor
    MazePosition(const MazePosition& pos) : MazePosition(pos.row, pos.col, pos.junc) {} //Copy constructor
};

//Node class
class Node {
public:
    MazePosition pos;
    Node* next;
    Node* prev;

    Node() : next(nullptr), prev(nullptr) {}
};

// Maze class
class Maze {
private:
    Node* first;
    Node* last;

public:
    //Maze array to save the text file into.
    char maze_array [51][51]{};
    Maze() : first(nullptr), last(nullptr) {}

    //Read text file function
    void ReadTextFile(string filenameIn){
        string line;
        ifstream file { filenameIn };
        //Check if the file is open
        if (!file.is_open())
            return;

        int i = 0;
        //Loop through the entire file and get each line
        while (getline(file, line))
        {
            //Make sure the file isn't larger than 50 lines
            if (i <= 50)
            {
                //Enter each character into the 2D maze array.
                for (int j = 0; j <= 50; j++) {
                        maze_array[i][j] = line[j];
                }
            }
            i++;
        }
        file.close();
    };

    //Write a text file function
    void WriteTextFile(string filenameOut){
        string line = "";
        ofstream file { filenameOut };
        //Check if the file is open
        if (!file.is_open())
            return;

        //Loop through each maze row and column
        for (int i = 0; i <= 50; i++) {
            for (int j = 0; j <= 50; j++){
                //Replace all 'D' with blanks.
                if(maze_array[i][j] == 'D')
                {
                    maze_array[i][j] = ' ';
                }
                line += maze_array[i][j];
            }
            line += "\n";
        }
        //Write to file
        file << line;
        file.close();
    }

    //Push function for the stack
    void Push(MazePosition pos) {
        Node* node = new Node();
        node->pos = pos;
        // Points to the last node entered so we can traverse backwards
        node->prev = last;

        if(first == nullptr) {
            // add the first node
            first = node;
        } else {
            // add other node
            last->next = node;
        }
        last = node;
    }

    //Pop function for the stack
    MazePosition Pop() {
        // have a node to pop
        if (first != nullptr) {
            Node* node = last;
            //the previous pointer is the new last node
            last = last->prev;
            // if there is still a node in the stack
            if (last != nullptr) {
                // set the updated last node to point to null
                last->next = node->next;
            }

            // removing the only node in the stack
            if (node == first) {
                first = nullptr;
            }

            //Save the node position address
            MazePosition temp = node->pos;
            delete node;

            return temp;
        }
        //if this returns it means there was an error
        return MazePosition();
    }

    //Check Position function
    string CheckPosition(int curPosX, int curPosY, Maze &curMaze) {
        //Check north for wall
        string paths;
        // Make sure the direction checked isn't the last place that was moved from
        if (curPosX -1 != curMaze.last->pos.row && curPosY == curMaze.last->pos.col) {
            //Make sure move won't go out of bounds.
            if ((curPosX - 1) < 0)
                paths += "0";
            //If the position is empty, save to move to.
            else if (maze_array[curPosX - 1][curPosY] == ' ')
                paths += "1";
            else
                paths += "0";
        }

            //Check east for wall
        // Make sure the direction checked isn't the last place that was moved from
        if (curPosX == curMaze.last->pos.row && curPosY + 1 != curMaze.last->pos.col) {
            //Make sure move won't go out of bounds.
            if ((curPosY + 1) > 51)
                paths += "0";
                //If the position is empty, save to move to.
            else if (maze_array[curPosX][curPosY + 1] == ' ')
                paths += "1";
            else
                paths += "0";
        }

            //Check south for wall
        // Make sure the direction checked isn't the last place that was moved from
        if (curPosX + 1 != curMaze.last->pos.row && curPosY == curMaze.last->pos.col) {
            //Make sure move won't go out of bounds.
            if ((curPosX + 1) > 51)
                paths += "0";
                //If the position is empty, save to move to.
            else if (maze_array[curPosX + 1][curPosY] == ' ')
                paths += "1";
            else
                paths += "0";
        }
            //Check west for wall
        // Make sure the direction checked isn't the last place that was moved from
        if (curPosX == curMaze.last->pos.row && curPosY - 1 != curMaze.last->pos.col) {
            //Make sure move won't go out of bounds.
            if ((curPosY - 1) < 0)
                paths += "0";
                //If the position is empty, save to move to.
            else if (maze_array[curPosX][curPosY - 1] == ' ')
                paths += "1";
            else
                paths += "0";
        }
        //Return which directions are available.
        return paths;
    }

    //Solve function
    void Solve(Maze &curMaze){
        string direction;
        bool pathFound = false;
        int junc = 0, count = 0;
        int row = 1, col = 0, choices = 0;
        //Push the entrance position onto the stack
        curMaze.Push({row, col, junc});
        //Mark the current position to show where we've been.
        maze_array[row][col] = '#';

        //While curMaze row and columns don't match the exit
        // coordinates keep searching.
        while (curMaze.last->pos.row != 50 && curMaze.last->pos.col != 50)
        {
            //Check for all available locations to move to.
            direction = CheckPosition(row,col, curMaze);
            //If the direction is a dead end.
            if (direction == "0000")
            {
                //Go to the last known junction location
                while (curMaze.last->pos.junc != 1)
                {
                    //Mark each space going back from the dead end as a 'D'
                    maze_array[curMaze.last->pos.row][curMaze.last->pos.col] = 'D';
                    curMaze.Pop();
                }
                maze_array[curMaze.last->pos.row][curMaze.last->pos.col] = 'D';
                curMaze.Pop();
                //Set the current row and col to the last position on the stack.
                row = curMaze.last->pos.row;
                col = curMaze.last->pos.col;
            }

            //Loop through each direction character
            for (char c : direction)
            {
                count++;
                //Check if a path is found.
                if(c == '1'){
                    //Adjust the direction to move to
                    if(!pathFound) {
                        if (count == 1) {
                            row--;
                        } else if (count == 2) {
                            col++;
                        } else if (count == 3) {
                            row++;
                        } else if (count == 4) {
                            col--;
                        }
                        maze_array[row][col] = '#';
                        pathFound = true;
                    }
                    choices++;
                }
            }
            count = 0;
            pathFound = false;
            curMaze.Push({row, col, junc});
            //If multiple choices were found, set the node to have a junction.
            if (choices > 1){
                curMaze.last->pos.junc = 1;
            }
            choices = 0;
        }
    }
};
#endif //ASSIGNMENT_2_MAZE_H
