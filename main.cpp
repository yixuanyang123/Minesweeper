#include <iostream>
#include <SFML/Graphics.hpp>
#include "Win.h"
#include <fstream>
#include <unistd.h>
#include <sstream>
int main()
{
    char* cwd = getcwd(NULL, 0);
    stringstream ss;
    ss << cwd;
    string direct;
    ss >> direct;
    direct += "/files/";
    cout << direct << endl;

    char buf[1024];
    string message;
    ifstream infile;
    infile.open(direct+"board_config.cfg", ios::in);
    windowSize sizeFile;
    if (infile.is_open())
    {
        infile.getline(buf, 1024);
        sizeFile.col = strtol(buf,nullptr,10);
        infile.getline(buf, 1024);
        sizeFile.row = strtol(buf, nullptr, 10);
        infile.getline(buf, 1024);
        sizeFile.mines = strtol(buf, nullptr, 10);
    }
    else
    {
        cout << "no such cfg file" << endl;
    }
    welcomWindow wel(sizeFile);
    wel.display(direct);
    //cout << filePath << endl;
    return 0;
}