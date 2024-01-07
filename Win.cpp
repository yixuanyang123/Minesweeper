#include "Win.h"
void setText(sf::Text& text, float x, float y) {
    sf::FloatRect textRect = text.getLocalBounds();
    text.setOrigin(textRect.left + textRect.width / 2.0f,
                   textRect.top + textRect.height / 2.0f);
    text.setPosition(sf::Vector2f(x, y));
}


void TimerE::reset()
{
    start = chrono::high_resolution_clock::now();
    savedseconds = 0;
}
void TimerE::pause()
{
    savedseconds = getSeconds();
    pauseFlag = true;
}
void TimerE::continueTimer()
{
    start = chrono::high_resolution_clock::now();
    pauseFlag = false;
}
int TimerE::getSeconds()
{
    if (pauseFlag)
    {
        return savedseconds;
    }
    else
        return savedseconds + chrono::duration_cast<chrono::seconds>(chrono::high_resolution_clock::now() - start).count();
}

void welcomWindow::display(string filePath)
{
    sf::RenderWindow window(sf::VideoMode(size.col * 32, (size.row * 32) + 100), this->name);
    sf::Font font;
    if (!font.loadFromFile(filePath + "font.ttf"))
    {
        cout << "no font file" << endl;
    }
    char input;
    string showtext = "|";
    while (window.isOpen())
    {
        sf::Text welText;
        welText.setFont(font);

        // set the string to display
        welText.setString("WELCOME TO MINESWEEPER!");

        // set the character size
        welText.setCharacterSize(24); // in pixels, not points!

        // set the color
        welText.setFillColor(sf::Color::White);

        // set the text style
        welText.setStyle(sf::Text::Bold | sf::Text::Underlined);

        float width = size.col * 32;
        float height = size.row * 32 + 100;
        setText(welText, width / 2.0f, height / 2.0f - 150);


        sf::Text promotText;
        promotText.setFont(font);

        // set the string to display
        promotText.setString("Enter your name:");

        // set the character size
        promotText.setCharacterSize(20); // in pixels, not points!

        // set the color
        promotText.setFillColor(sf::Color::White);

        // set the text style
        promotText.setStyle(sf::Text::Bold);

        setText(promotText, width / 2.0f, height / 2.0f - 75);

        sf::Text inputText;
        inputText.setFont(font);


        // set the character size
        inputText.setCharacterSize(18); // in pixels, not points!

        // set the color
        inputText.setFillColor(sf::Color::Yellow);

        inputText.setString(showtext);
        setText(inputText, width / 2.0f, height / 2.0f - 45);

        sf::Event event;

        while (window.pollEvent(event))
        {

            if (event.type == sf::Event::Closed)
                window.close();
            else if (event.type == sf::Event::TextEntered)
            {

                if (sf::Keyboard::isKeyPressed(sf::Keyboard::BackSpace))
                {
                    showtext = showtext.substr(0, showtext.length() - 1);
                    showtext = showtext.substr(0, showtext.length() - 1);
                    showtext += "|";
                    inputText.setString(showtext);
                    cout << showtext << endl;
                }
                else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
                {
                    showtext = showtext.substr(0, showtext.length() - 1);
                    window.close();
                    gameWindow game(size, showtext);
                    game.display(filePath);
                }
                else
                {
                    showtext = showtext.substr(0, showtext.length() - 1);
                    input = static_cast<char>(event.text.unicode);
                    if (isalpha(input) && showtext.size() < 10)
                    {
                        if (showtext.size() == 0)
                        {
                            input = toupper(input);
                        }
                        else
                        {
                            input = tolower(input);
                        }
                        showtext += input;
                        showtext += "|";
                        inputText.setString(showtext);
                        cout << showtext << endl;
                    }
                    else
                    {
                        showtext += "|";
                        inputText.setString(showtext);
                    }
                }

            }
        }
        window.clear(sf::Color::Blue);
        window.draw(welText);
        window.draw(promotText);
        window.draw(inputText);
        window.display();

    }
}

void gameWindow::display(string filePath)
{
    sf::RenderWindow window(sf::VideoMode(size.col * 32, (size.row * 32) + 100), this->name);
    sf::Font font;
    int flagsNum = 0;
    int secondsNow;
    TimerE timeElapse;
    if (!font.loadFromFile(filePath + "font.ttf"))
    {
        cout << "no font file" << endl;
    }
    char input;
    string showtext = "|";
    sf::Texture hiddenTiles;

    hiddenTiles.loadFromFile(filePath + "images/tile_hidden.png");
    vector<stateTile> mainBoard;
    for (int i = 0; i < this->size.col * this->size.row; i++)
    {
        stateTile tile;
        tile.tile.setTexture(hiddenTiles);
        tile.tile.move(sf::Vector2f(i % this->size.col * 32, int(i / this->size.col) * 32));
        tile.state = 9;
        tile.x = i % size.col;
        tile.y = int(i / size.col);
        mainBoard.push_back(tile);
    }


    sf::Texture revealTiles;
    revealTiles.loadFromFile(filePath + "images/tile_revealed.png");

    sf::Texture mine;
    mine.loadFromFile(filePath + "images/mine.png");

    vector<sf::Texture> number;
    for (int i = 1; i < 9; i++)
    {
        sf::Texture number_i;
        number_i.loadFromFile(filePath + "images/number_" + to_string(i) + ".png");
        number.push_back(number_i);
    }

    sf::Texture face_win;
    face_win.loadFromFile(filePath + "images/face_win.png");
    sf::Texture face_lose;
    face_lose.loadFromFile(filePath + "images/face_lose.png");
    sf::Texture face_happy;
    face_happy.loadFromFile(filePath + "images/face_happy.png");

    sf::Texture flag;
    flag.loadFromFile(filePath + "images/flag.png");

    sf::Texture debug;
    debug.loadFromFile(filePath + "images/debug.png");
    sf::Texture leaderboard;
    leaderboard.loadFromFile(filePath + "images/leaderboard.png");
    sf::Texture pause;
    pause.loadFromFile(filePath + "images/pause.png");
    sf::Texture play;
    play.loadFromFile(filePath + "images/play.png");
    sf::Texture digits;
    digits.loadFromFile(filePath + "images/digits.png");

    sf::RectangleShape rect(sf::Vector2f( this->size.col*32, 100));
    rect.setPosition(sf::Vector2f(0, this->size.row * 32));
    rect.setFillColor(sf::Color::White);

    sf::Sprite debugButton(debug);
    sf::Sprite leaderboardButton(leaderboard);
    sf::Sprite pauseButton(pause);
    sf::Sprite playButton(play);
    sf::Sprite happyFaceButton(face_happy);

    happyFaceButton.move(sf::Vector2f((((size.col) / 2.0) * 32) - 32, 32 * ((size.row) + 0.5f)));
    debugButton.move(sf::Vector2f((size.col * 32) - 304, 32 * (size.row + 0.5f)));
    pauseButton.move(sf::Vector2f(((size.col) * 32) - 240, 32 * ((size.row) + 0.5f)));
    playButton.move(sf::Vector2f(((size.col) * 32) - 240, 32 * ((size.row) + 0.5f)));
    leaderboardButton.move(sf::Vector2f(((size.col) * 32) - 176, 32 * ((size.row) + 0.5f)));

    vector<sf::Sprite> counts;
    for (int i = 0; i < 3; i++)
    {
        sf::Sprite sp(digits, sf::IntRect(0, 0, 21, 32));
        counts.push_back(sp);
    }
    counts[0].move(sf::Vector2f(33, 32 * ((size.row) + 0.5f) + 16));
    counts[1].move(sf::Vector2f(33 + 21, 32 * ((size.row) + 0.5f) + 16));
    counts[2].move(sf::Vector2f(33 + 21 + 21, 32 * ((size.row) + 0.5f) + 16));

    vector<int> adjacent;
    vector<int> minesposition = randomMines(this->size.col * this->size.row, this->size.mines,mainBoard,this->size.col,this->size.row);
    vector<stateTile> states;

    vector<sf::Sprite> timer;
    for (int i = 0; i < 4; i++)
    {
        sf::Sprite sp(digits, sf::IntRect(0, 0, 21, 32));
        timer.push_back(sp);
    }
    timer[0].move(sf::Vector2f(((size.col) * 32) - 97, 32 * ((size.row) + 0.5f) + 16));
    timer[1].move(sf::Vector2f(((size.col) * 32) - 97 + 21, 32 * ((size.row) + 0.5f) + 16));
    timer[2].move(sf::Vector2f(((size.col) * 32) - 54, 32 * ((size.row) + 0.5f) + 16));
    timer[3].move(sf::Vector2f(((size.col) * 32) - 54 + 21, 32 * ((size.row) + 0.5f) + 16));

    int remain;
    remain = this->size.mines;
    sf::Sprite negative(digits, sf::IntRect(0, 0, 21, 32));
    negative.setTextureRect(sf::IntRect(10 * 21, 0, 21, 32));
    negative.move(sf::Vector2f(12, 32 * ((this->size.row) + 0.5f) + 16));

    vector<bool> flags;
    for (int i = 0; i < 6; i++)
        flags.push_back(false);
    vector<int> savedStates;
    for (int i = 0; i < mainBoard.size(); i++)
        savedStates.push_back(mainBoard.at(i).state);
    while (window.isOpen())
    {
        sf::Event event;

        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            else if (event.type == sf::Event::MouseButtonPressed)
            {
                if (event.key.code == sf::Mouse::Left)
                {
                    sf::FloatRect buttonRect = happyFaceButton.getLocalBounds();
                    sf::Vector2i position = sf::Mouse::getPosition(window);
                    int button;
                    button = buttonDetection(position, size.col, size.row);
                    switch (button)
                    {
                        case 0:
                            flags[button] = !flags[button];
                            flagsNum = 0;
                            remain = size.mines;
                            for (int i = 0; i < mainBoard.size(); i++)
                            {
                                mainBoard.at(i).state = 9;
                            }
                            for (int i = 0; i < 6; i++)
                                flags[i] = false;
                            timeElapse.reset();
                            timeElapse.continueTimer();
                            minesposition = randomMines(this->size.col * this->size.row, this->size.mines,mainBoard,this->size.col,this->size.row);
                            break;
                        case 1:
                            if(!flags[4]&&!flags[5])
                                flags[button] = !flags[button];
                            if (flags[1] && !flags[4]&&!flags[5])
                            {
                                for (auto a : minesposition)
                                {
                                    savedStates[a] = mainBoard.at(a).state;
                                    mainBoard.at(a).state = 12;
                                }
                                timeElapse.pause();

                            }

                            else if (!flags[1] && !flags[4] && !flags[5])
                            {
                                timeElapse.continueTimer();
                                for (auto a : minesposition)
                                {
                                    mainBoard.at(a).state = savedStates[a];
                                }
                            }
                            break;
                        case 2:
                            if (!flags[4] && !flags[5])
                                flags[button] = !flags[button];
                            if (flags[button]&& !flags[4] && !flags[5])
                            {
                                timeElapse.pause();
                                for (unsigned int i = 0; i < mainBoard.size(); i++)
                                    savedStates[i] = mainBoard[i].state;
                                for (unsigned int i = 0; i < mainBoard.size(); i++)
                                {
                                    mainBoard[i].state = 11;
                                }
                            }
                            else if (!flags[button] && !flags[4] && !flags[5])
                                timeElapse.continueTimer();
                            for (int i = 0; i < mainBoard.size(); i++)
                            {
                                mainBoard.at(i).state = savedStates[i];
                            }
                            break;
                        case 3:
                            flags[button] = !flags[button];
                            if (flags[button])
                            {
                                leaderWindow leads(this->size,username);
                                for (auto& a : mainBoard)
                                {
                                    a.tile.setTexture(revealTiles);
                                    window.draw(a.tile);
                                }

                                window.display();
                                timeElapse.pause();
                                leads.display(filePath,false,0);
                                flags[button] = !flags[button];
                                timeElapse.continueTimer();
                            }
                            break;
                        case 4:
                            break;
                        case 5:
                            if (!flags[2]&&!flags[4]&&!flags[5])
                            {
                                sf::Vector2i position = sf::Mouse::getPosition(window);
                                if (position.y < 32 * ((size.row)))
                                {

                                    int numPosition = int(position.y / 32) * size.col + int(position.x / 32);
                                    if (mainBoard[numPosition].reveal(mainBoard, minesposition, this->size.col, this->size.row))
                                    {
                                        flags[5] = true;
                                        timeElapse.pause();
                                    }
                                }
                                for (auto& a : mainBoard)
                                {
                                    a.flag = false;
                                }
                                break;
                            }

                    }

                }
                else if (event.key.code == sf::Mouse::Right)
                {

                    if (!flags[2]&&!flags[4]&&!flags[5])
                    {
                        sf::FloatRect buttonRect = happyFaceButton.getLocalBounds();
                        sf::Vector2i position = sf::Mouse::getPosition(window);
                        if (position.y < 32 * ((size.row)))
                        {

                            int numPosition = int(position.y / 32) * size.col + int(position.x / 32);
                            if (mainBoard.at(numPosition).state != 10)
                            {
                                flagsNum++;
                                mainBoard.at(numPosition).state = 10;
                                mainBoard.at(numPosition).tile.setTexture(flag);
                                window.draw(mainBoard.at(numPosition).tile);
                                remain--;
                            }
                            else
                            {
                                flagsNum--;
                                mainBoard.at(numPosition).state = 9;
                                mainBoard.at(numPosition).tile.setTexture(hiddenTiles);
                                window.draw(mainBoard.at(numPosition).tile);
                                remain++;
                            }
                        }
                        if (remain == 0)
                        {
                            for (int i = 0; i < minesposition.size(); i++)
                            {
                                flags[4] = true;
                                if (mainBoard[minesposition[i]].state != 10)
                                {
                                    flags[4] = false;
                                    break;
                                }
                            }
                        }

                        if (flags[4])
                        {
                            leaderWindow leads(this->size, username);
                            int remain1 = abs(remain);
                            vector<int> outputCount;
                            outputCount.push_back((remain1 / 100) % 10);
                            outputCount.push_back((remain1 / 10) % 10);
                            outputCount.push_back((remain1 / 1) % 10);
                            for (int i = 0; i < 3; i++)
                            {
                                counts[i].setTextureRect(sf::IntRect(outputCount[i] * 21, 0, 21, 32));
                                window.draw(counts[i]);
                            }
                            happyFaceButton.setTexture(face_win);
                            window.draw(happyFaceButton);
                            window.display();
                            timeElapse.pause();
                            leads.display(filePath, true,timeElapse.getSeconds());

                        }
                    }

                }

            }
        }
        window.clear(sf::Color::White);
        for (auto a : mainBoard)
        {
            a.tile.setTexture(revealTiles);
            window.draw(a.tile);
        }
        for (auto& a : mainBoard)
        {
            if (!flags[1]&&!flags[4]&&!flags[5])
            {
                for (auto& a : mainBoard)
                {
                    if (a.state == 12)
                    {
                        a.state = 9;
                        a.tile.setTexture(hiddenTiles);
                    }
                }
            }
            else if (flags[5])
            {
                for (auto a : minesposition)
                {
                    mainBoard.at(a).state = 12;
                }
            }
            switch (a.state)
            {
                case 0:
                    a.tile.setTexture(revealTiles);
                    break;
                case 1:
                    a.tile.setTexture(number[0]);
                    break;
                case 2:
                    a.tile.setTexture(number[1]);
                    break;
                case 3:
                    a.tile.setTexture(number[2]);
                    break;
                case 4:
                    a.tile.setTexture(number[3]);
                    break;
                case 5:
                    a.tile.setTexture(number[4]);
                    break;
                case 6:
                    a.tile.setTexture(number[5]);
                    break;
                case 7:
                    a.tile.setTexture(number[6]);
                    break;
                case 8:
                    a.tile.setTexture(number[7]);
                    break;
                case 9:
                    a.tile.setTexture(hiddenTiles);
                    break;
                case 10:
                    a.tile.setTexture(flag);
                    break;
                case 11:
                    a.tile.setTexture(revealTiles);
                    break;
                case 12:
                    a.tile.setTexture(mine);
                    break;
                default:
                    break;
            }
            if (flags[2])
            {
                a.tile.setTexture(revealTiles);
                window.draw(a.tile);
            }

            else if(!flags[3])
                window.draw(a.tile);
        }
        happyFaceButton.setTexture(face_happy);
        window.draw(happyFaceButton);
        window.draw(debugButton);
        if (flags[2])
        {
            window.draw(playButton);
        }
        else
            window.draw(pauseButton);
        window.draw(leaderboardButton);
        vector<int> outputCount;


        int remain1 = abs(remain);
        outputCount.push_back((remain1 / 100) % 10);
        outputCount.push_back((remain1 / 10) % 10);
        outputCount.push_back((remain1 / 1) % 10);
        if (flags[4])
        {
            happyFaceButton.setTexture(face_win);
            window.draw(happyFaceButton);
        }
        else if (flags[5])
        {
            happyFaceButton.setTexture(face_lose);
            window.draw(happyFaceButton);
        }
        secondsNow = timeElapse.getSeconds();
        vector<int>outputtimer;
        outputtimer.push_back(int(int(secondsNow / 60) / 10));
        outputtimer.push_back(int(int(secondsNow / 60) % 10));
        outputtimer.push_back(int(int(secondsNow % 60) / 10));
        outputtimer.push_back(int(int(secondsNow % 60) % 10));
        for (int i = 0; i < 4; i++)
        {
            timer[i].setTextureRect(sf::IntRect(outputtimer[i] * 21, 0, 21, 32));
            window.draw(timer[i]);
        }
        for (int i = 0; i < 3; i++)
        {
            counts[i].setTextureRect(sf::IntRect(outputCount[i] * 21, 0, 21, 32));
            window.draw(counts[i]);
        }
        if (remain < 0)
        {
            window.draw(negative);
        }
        for (auto a : timer)
        {
            window.draw(a);
        }
        window.display();
    }
}


void leaderWindow::display(string filePath, bool winFlag, int timeE)
{
    sf::RenderWindow window(sf::VideoMode(size.col * 16, (size.row * 16) + 50), this->name);
    sf::Font font;
    int flagsNum = 0;
    int secondsNow;
    TimerE timeElapse;
    if (!font.loadFromFile(filePath + "font.ttf"))
    {
        cout << "no font file" << endl;
    }
    ifstream infile;
    infile.open(filePath+ "leaderboard.txt", ios::in);
    char buf[1024];

    stringstream ss;
    string gamer = "";
    vector<int> times;
    int rank = 0;
    for (int i = 0; i < 5; i++)
    {

        infile.getline(buf, 1024);
        string message;
        message = buf;
        gamer = gamer + to_string(i)+"."+"\t"+message.substr(0,5)+"\t"+message.substr(6,message.size()-6)+"\n\n";
        times.push_back(atoi(message.substr(0, 2).c_str()) * 60 + atoi(message.substr(3, 2).c_str()) * 60);
    }
    infile.close();
    if (winFlag)
    {
        for (int i = 0; i < times.size(); i++)
        {
            if (times[i] > timeE)
            {
                break;
            }
            else
                rank++;
        }
        if (rank < 5)
        {
            string newgamer="";
            string saveFile = "";
            ss << gamer;
            for (int i = 0; i < rank; i++)
            {

                ss.getline(buf, 1024);
                string message = buf;
                saveFile += message.substr(3, 5) + "," + message.substr(8, message.size() - 8)+"\n";
                newgamer += message + "\n\n";
            }
            saveFile += to_string(int(timeE / 60 / 10)) + to_string(int(timeE / 60 % 10)) + ":" + to_string(int(timeE % 60 / 10)) + to_string(int(timeE % 60 % 10)) + "," + username + "*\n";
            newgamer += to_string(rank+1) + ".\t" + to_string(int(timeE / 60 / 10)) + to_string(int(timeE / 60 % 10)) + ":" + to_string(int(timeE % 60 / 10)) + to_string(int(timeE % 60 % 10)) + "\t" + username + "*" + "\n\n";
            for (int i = rank+1; i < 5; i++)
            {
                ss.getline(buf, 1024);
                string message = buf;
                ss.getline(buf, 1024);
                saveFile += message.substr(3, 5) + "," + message.substr(8, message.size() - 8) + "\n";
                newgamer += to_string(i + 1) + "." + message.substr(2, message.size() - 2) + "\n\n";
            }
            gamer = newgamer;
            ofstream writeFile;
            writeFile.open(filePath + "leaderboard.txt", ios::out);
            writeFile.write(saveFile.c_str(),saveFile.size());

        }
    }

    sf::Text leaderText;
    leaderText.setFont(font);

    // set the string to display
    leaderText.setString("LEADERBOARD");

    // set the character size
    leaderText.setCharacterSize(20); // in pixels, not points!

    // set the color
    leaderText.setFillColor(sf::Color::White);

    // set the text style
    leaderText.setStyle(sf::Text::Bold | sf::Text::Underlined);
    setText(leaderText, 16 * size.col / 2.0f, ((size.row * 16) + 50) / 2.0f - 120);


    sf::Text nameText;
    nameText.setFont(font);

    // set the string to display
    nameText.setString(gamer);

    // set the character size
    nameText.setCharacterSize(18); // in pixels, not points!

    // set the color
    nameText.setFillColor(sf::Color::White);

    // set the text style
    nameText.setStyle(sf::Text::Bold);
    setText(nameText, 16 * size.col / 2.0f, ((size.row * 16) + 50) / 2.0f +20);
    sf::Event event;
    while (window.isOpen())
    {
        while (window.pollEvent(event))
        {

            if (event.type == sf::Event::Closed)
                window.close();
        }
        window.clear(sf::Color::Blue);
        window.draw(leaderText);
        window.draw(nameText);
        window.display();
    }


}
