//T02_G11
#ifndef T02_G11_MAZE_H
#define T02_G11_MAZE_H

#include "structs.h"
#include "Post.h"
#include <iostream>
#include <string>

using namespace std;

class Maze
{
public:
    Maze(unsigned int numRows, unsigned int numCols);
    
    bool addPost(const Post& post, const Position& position);

    unsigned int getnumRows() const;
    unsigned int getnumCols() const;

    void show() const;

private:
    int numRows, numCols;
};


#endif //T02_G11_MAZE_H

/*
Maze is made of Post. In Maze you have to call Post class.

This class is made to create and load the map (not sure of this). 
*/