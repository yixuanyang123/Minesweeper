#ifndef GAME_H
#define GAME_H
#include "Win.h"
#include <string>
#include "vector"
#include<iostream>
#include<algorithm>
#include <SFML/Graphics.hpp>
using namespace std;

class stateTile
{
public:
    sf::Sprite tile;
    unsigned char state;
    int adjacent = 0;
    bool isMines = false;
    int x;
    int y;
    bool reveal( vector<stateTile>& mainBoard, vector<int>minesPosition, int col, int row);
    bool flag = false;
};

vector<int> randomMines(unsigned int tilesNum, unsigned int minesNum, vector<stateTile>& mainBoard, int col, int row);
int buttonDetection(sf::Vector2i position, int col, int row);
#endif