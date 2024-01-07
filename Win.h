#ifndef WIN_H
#define WIN_H
#include <string>
#include "vector"
#include<iostream>
#include "game.h"
#include <fstream>
#include <sstream>
#include <chrono>
#include <SFML/Graphics.hpp>
using namespace std;
struct windowSize
{
    unsigned int col;
    unsigned int row;
    unsigned int mines;
};

class welcomWindow
{
    string name = "Minesweeper";
    windowSize size;
    unsigned int timer = 0;
    unsigned int remain = 0;

public:
    void display(string filePath);
    welcomWindow()
    {

    }
    welcomWindow(windowSize sizeInput)
    {
        this->size.col = sizeInput.col;
        this->size.row = sizeInput.row;
        this->size.mines = sizeInput.mines;
    }
};

class TimerE
{
    chrono::time_point<chrono::high_resolution_clock> start = chrono::high_resolution_clock::now();
    int savedseconds = 0;
    bool pauseFlag = false;
public:
    void reset();
    int getSeconds();
    void pause();
    void continueTimer();
};

class gameWindow
{
    string name = "Minesweeper";
    string username;
    windowSize size;

public:
    void display(string filePath);

    gameWindow()
    {

    }
    gameWindow(windowSize sizeInput, string user)
    {
        username = user;
        this->size.col = sizeInput.col;
        this->size.row = sizeInput.row;
        this->size.mines = sizeInput.mines;
    }
};

class leaderWindow
{
    string name = "Minesweeper";
    string username;
    windowSize size;

public:
    void display(string filePath, bool winFlag, int timeE = 0);
    leaderWindow()
    {

    }
    leaderWindow(windowSize sizeInput, string user)
    {
        username = user;
        this->size.col = sizeInput.col;
        this->size.row = sizeInput.row;
        this->size.mines = sizeInput.mines;
    }
};
#endif