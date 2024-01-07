#include "game.h"
vector<int> randomMines(unsigned int tilesNum, unsigned int minesNum, vector<stateTile>& mainBoard, int col, int row)
{
    vector<int> res;
    vector<int> randomVec;
    for (auto& a : mainBoard)
    {
        a.isMines = false;
    }
    for (unsigned int i = 0; i < tilesNum; i++)
    {
        randomVec.push_back(i);
    }
    random_shuffle(randomVec.begin(), randomVec.end());

    for (unsigned int i = 0; i < minesNum; i++)
    {
        res.push_back(randomVec[i]);
        mainBoard.at(randomVec[i]).isMines = true;
    }
    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < col; j++)
        {
            int adjacentNum = 0;
            for(int n = i-1;n<i+2;n++)
                for (int m = j - 1; m < j+2; m++)
                {
                    if ((n > 0 || n == 0) && (m > 0 || m == 0) && (n < row) && (m < col))
                    {
                        if (mainBoard[m + n * col].isMines)
                            adjacentNum++;
                    }
                }
            mainBoard[j + i * col].adjacent = adjacentNum;
        }
    }
    return res;
}

//0. Reset
//1. Debug
//2. Pause
//3. Leader
//4. Nothing
int buttonDetection(sf::Vector2i position, int col, int row)
{
    if (position.x > (((col) / 2.0) * 32) - 32 && position.x < (((col) / 2.0) * 32) - 32 + 64 && position.y>32 * ((row)+0.5f) && position.y < 32 * ((row)+0.5f) + 64)
        return 0;
    else if (position.x > ((col) * 32) - 304 && position.x < ((col) * 32) - 304 + 64 && position.y > 32 * ((row)+0.5f) && position.y < 32 * ((row)+0.5f) + 64)
        return 1;
    else if (position.x > ((col) * 32) - 240 && position.x < ((col) * 32) - 240 + 64 && position.y > 32 * ((row)+0.5f) && position.y < 32 * ((row)+0.5f) + 64)
        return 2;
    else if (position.x > ((col) * 32) - 176 && position.x < ((col) * 32) - 176 + 64 && position.y > 32 * ((row)+0.5f) && position.y < 32 * ((row)+0.5f) + 64)
        return 3;
    else if (position.y < 32 * ((row)))
        return 5;
    else
        return 4;
}

bool stateTile::reveal( vector<stateTile>& mainBoard, vector<int>minesPosition,int col,int row)
{
    this->flag = true;
    if (this->isMines)
    {
        return true;
    }
    if (this->adjacent != 0)
    {
        this->state = this->adjacent;
        return false;
    }
    else
        this->state = 0;
    for(int i=this->y-1;i<this->y+2;i++)
        for (int j = this->x - 1; j < this->x + 2; j++)
        {
            if ((i > 0 || i == 0) && (j > 0 || j == 0) && (j < col) && (i < row))
            {
                if(!mainBoard[i * col + j].flag)
                    mainBoard[i * col + j].reveal(mainBoard, minesPosition, col, row);
            }
        }
    return false;
}