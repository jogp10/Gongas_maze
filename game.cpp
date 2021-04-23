//T02_G11

#include "game.h"
#include <iostream>
#include <fstream>
#include <string>
#include <thread>
#include <chrono>
#include <iomanip>
#include <cmath>
#include <vector>
#include <algorithm>

using namespace std;


int menu(){
    int menu_choice;
    // Main Menu
    cout << "1) Rules" << endl;
    cout << "2) Play" << endl;
    cout << "3) Leaderboard" << endl;
    cout << "0) Exit" << endl;

    // Ask for menu_choice
    while (true)
    {
        this_thread::sleep_for(chrono::milliseconds(250));
        cout << "Option: "; cin >> menu_choice;
        if(cin.eof()) return 0;
        else if(menu_choice == 1 || menu_choice == 2 || menu_choice == 3) return menu_choice;
        cin.clear();
        cin.ignore(10000, '\n');
        cerr << "Input a valid operation! (0, 1 or 2 to proceed)" << endl;
    }
}


void ReadRules(){
    string line;
    ifstream myfile ("rules.TXT");  // file where are the rules
    int goBack;

    // Open file and print all the lines
    if(myfile.is_open())
    {
        while (getline(myfile, line)) cout << line << '\n';
        myfile.close();
    }

    //Go back to main menu by pressing '0'
    cout << "Press '0' to go to the main menu" << endl;
    cin >> goBack;

    //Test input for an invalid one
    if(cin.eof()) return;
    while (goBack != 0 || cin.fail()) {
        cin.clear();
        cin.ignore(10000, '\n');
        cin >> goBack;
        if(cin.eof()) return;
    }
}


bool check_path(int level, string &path){
    //replacing 'XX' with the number of maze
    path[5]= (char)(level/10 + '0');
    path[6]= (char)(level%10 + '0');

    //open file and check if exists
    ifstream f(path);
    if(f.good()) return true;
    return false;
}


vector<string> ReadMaze(const string& path){
    string line; //variable where we store individual lines
    vector <string> vec; //place where we will be storing all the lines

    ifstream file(path); //open file for read

    //insert maze in a vector
    getline(file, line);
    while (getline(file, line)) vec.push_back(line);
    print(vec); //call function print
    return vec;
}


void print(const vector <string>& vec){
    //Displaying maze to user using elements stored in vec
    for (auto & i : vec) cout << i << endl;
}


void DisplayFile(const string& path, int n){

    string line; //variable where we store individual lines

    ifstream file(path);

    // display the maze on interface
    getline(file, line);
    if(path.size() == 11) {
        cout << setw(9) << "(" << n << ")" << endl;
    }
    while(getline(file, line)) cout << line << endl;
}


void play() {
    int MazeSelect;
    char start;
    bool exitGame = false;
    string path = "MAZE_xx.TXT";

    //display levels
    for(int l=1; l<=99; l++)
    {
        if(check_path(l, path)) DisplayFile(path, l);
    }

    // ask for a level to play
    cout << "What Maze do you wanna play?" << endl;

    // if he choose an invalid one, ask for another input while invalid!
    while (true)
    {
        cin >> MazeSelect;
        if(cin.fail());
        else if(cin.eof() || MazeSelect == 0) return;
        else if(check_path(MazeSelect, path)) break;
        cin.clear();
        cin.ignore(10000, '\n');
        cerr << "That's not a valid Maze! try another or '0' to return to main menu" << endl;
    }

    // Very start of the game
    cout << endl << "Good choice, let's start!" << endl << "Enter 'S' when you are READY..." << endl;
    cin >> start; cin.ignore(10000, '\n');
    if(cin.eof()) return;
    while ((start != 'S' && start != 's') || cin.eof()) {
        cin >> start; cin.ignore(10000, '\n');
        if(cin.eof()) return;
    }

    // display board, ready to start
    vector<string> vec = ReadMaze(path);

    // booleans for keep playing if both still alive
    bool player_live = true;
    bool robots_live = true;

    //get player's position & robot's position
    int y = 0, x;
    int y_player = 0, x_player = 0;
    vector<int> robot_x, robot_y;

    for (y; y < vec.size(); y++) {
        x = 0;
        for (x; x <= vec[y].size(); x++) {
            if (vec[y][x] == 'H')
            {
                y_player = y;
                x_player = x;
            }
            else if (vec[y][x] == 'R') {
                robot_x.push_back(x);
                robot_y.push_back(y);
            }
        }
    }

    // Start timer
    auto start_time = chrono::steady_clock::now();

    // move from player and automatic play from robots
    while (robots_live && player_live) {

        //move player
        player_live = player(vec, y_player, x_player, exitGame);
        if(exitGame) return;
        cin.ignore(10000, '\n');

        //robot's turn
        if (player_live) {
            vector<int> deadRobots; // for dead robots

            for (int i = 0; i <= robot_x.size() - 1; i++) {
                //see if robot did die because anther's robot move
                if (vec[robot_y[i]][robot_x[i]] == 'r') {
                    deadRobots.push_back(i);
                    continue;
                }

                    //move robot
                else if (!robots(vec, y_player, x_player, robot_y[i], robot_x[i])) {
                    deadRobots.push_back(i);
                }

                // if robot catches player
                if (y_player == robot_y[i] && x_player == robot_x[i]) player_live = false;
            }

            //remove positions of dead robots
            int count = 0;
            for (int j : deadRobots) {
                robot_x.erase(robot_x.begin() + j - count);
                robot_y.erase(robot_y.begin() + j - count);
                count++;
            }

            if (robot_x.empty()) robots_live = false; // if all robots died, game over
        }
        print(vec); // print level with update
    }

    auto end_time = chrono::steady_clock::now(); // finish timer

    // if player dead
    if (robots_live) {
        cout << "You lost!! Better luck next time." << endl << endl;
    }
    // if robots dead, register time
    else {
        char name[15];

        auto time_lapsed = static_cast<chrono::duration<double>>(end_time - start_time);
        cout << "What a fantastic show!!\nYou completed the level in "; cout << int(time_lapsed.count()); cout << " seconds!!!\n";
        cout << "Tell me your name so i can remember it!!\n";
        cin.getline(name, sizeof(name));
        winner(name, int(time_lapsed.count()), MazeSelect);

        if(cin.fail()) {
            cin.ignore(10000, '\n');
            cin.clear();
        }
    }
}


bool player(vector<string> &vec, int &y, int &x, bool& exitGame){
    while(true) {

        char play;
        string check = "QWEASDZXC"; //possible plays

        // ask for a play
        cout << "What's your play" << endl;
        cin >> play;

        if(cin.eof()){
            exitGame = true; return true;
        }

        while (check.find(toupper(play)) == string::npos) {
            cin.clear(); cin.ignore(10000, '\n');
            cin >> play;
            if(cin.eof()){
                exitGame = true;
                return true;
            }
        }


        // Move, if collision -> he dies. Game over
        switch (toupper(play)) {
            case 'W': // move up
                if (!validMove(vec, y, x, -1)) break;  // test valid move
                return movePlayer(vec, y, x, -1);  // update player status ( alive or dead)

            case 'E': // move diagonal right up
                if (!validMove(vec, y, x, -1, +1)) break;
                return movePlayer(vec, y, x, -1, +1);

            case 'D': // move right
                if (!validMove(vec, y, x, 0, +1)) break;
                return movePlayer(vec, y, x, 0, +1);

            case 'C': // move diagonal right down
                if (!validMove(vec, y, x, +1, +1)) break;
                return movePlayer(vec, y, x, +1, +1);

            case 'X': // move down
                if (!validMove(vec, y, x, +1)) break;
                return movePlayer(vec, y, x, +1);

            case 'Z': // move diagonal left down
                if (!validMove(vec, y, x, +1, -1)) break;
                return movePlayer(vec, y, x, +1, -1);

            case 'A': // move left
                if (!validMove(vec, y, x, 0, -1)) break;
                return movePlayer(vec, y, x, 0, -1);

            case 'Q': // move diagonal left up
                if (!validMove(vec, y, x, -1, -1)) break;
                return movePlayer(vec, y, x, -1, -1);

            default:   // stay place
                return true;
        }
    }
}


bool movePlayer(vector<string> &vec, int &y, int &x, int vertical, int horizontal)
{
    swap(vec[y + vertical][x + horizontal], vec[y][x]);  // move player
    if (vec[y][x] == '*' || vec[y][x] == 'R') {  // if player moves causes death
        vec[y][x] = ' ';
        vec[y + vertical][x + horizontal] = 'h';  // update player's status
        return false;
    }
    // update player's position
    y += vertical;
    x += horizontal;
    return true;
}


bool validMove(vector<string> &vec, int &y, int &x, int vertical, int horizontal)
{
    if (vec[y + vertical][x + horizontal] == 'r') {   // if player move is against a stuck robot
        cout << "Not the play you wanted to do, try another! ";
        return false; // Not a valid move
    }
    return true;
}


bool robots(vector<string> &vec, int &yp, int &xp, int &yr, int &xr)
{
    int indice;
    double q,w,e,a,d,z,x,c, minor = 999999;
    vector<double> minor_d;

    //case 0
    q = sqrt((pow(xp-(xr-1), 2) + pow(yp - (yr-1), 2))); minor_d.push_back(q);
    //case 1
    w = sqrt((pow(xp-xr, 2) + pow(yp - (yr-1), 2))); minor_d.push_back(w);
    //case 2
    e = sqrt((pow(xp-(xr+1), 2) + pow(yp - (yr-1), 2))); minor_d.push_back(e);
    //case 3
    a = sqrt((pow(xp-(xr-1), 2) + pow(yp - yr, 2))); minor_d.push_back(a);
    //case 4
    d = sqrt((pow(xp-(xr+1), 2) + pow(yp - yr, 2))); minor_d.push_back(d);
    //case 5
    z = sqrt((pow(xp-(xr-1), 2) + pow(yp - (yr+1), 2))); minor_d.push_back(z);
    //case 6
    x = sqrt((pow(xp-xr, 2) + pow(yp - (yr+1), 2))); minor_d.push_back(x);
    //case 7
    c = sqrt((pow(xp-(xr+1), 2) + pow(yp - (yr+1), 2))); minor_d.push_back(c);

    for(int i=0; i<=7; i++)
    {
        if (minor_d[i] <= minor) // see which distance is minor
        {
            minor = minor_d[i];
            indice = i;
        }
    }

    switch(indice)
    {
        case 0:  // Q
            return moveRobot(vec, yr, xr, -1, -1);
        case 1: // W
            return moveRobot(vec, yr, xr, -1);
        case 2: // E
            return moveRobot(vec, yr, xr, -1, +1);
        case 3:  // A
            return moveRobot(vec, yr, xr, 0, -1);
        case 4: // D
            return moveRobot(vec, yr, xr, 0, +1);
        case 5: // Z
            return moveRobot(vec, yr, xr, +1, -1);
        case 6:  // X
            return moveRobot(vec, yr, xr, +1);
        case 7: // C
            return moveRobot(vec, yr, xr, +1, +1);
        default:
            return true;
    }
}


bool moveRobot(vector<string> &vec, int &yr, int &xr, int vertical, int horizontal)
{
    swap(vec[yr + vertical][xr + horizontal], vec[yr][xr]);  // move robot
    if (vec[yr][xr] == '*' || vec[yr][xr] == 'r' || vec[yr][xr] == 'R')  // if move causes death to the robot
    {
        vec[yr][xr] = ' ';  // eliminate cause of death
        vec[yr + vertical][xr + horizontal] = 'r';  // new position of R is now a stuck robot
        return false;
    }
    else if(vec[yr][xr] == 'H') vec[yr][xr] = ' ';  // if robot catches player
    yr += vertical; xr += horizontal;
    return true;
}


void winner(char name[15],int time,int maze) {
    string path = "MAZE_XX_WINNERS.TXT";   // path of file to write winners

    //Which maze, change path
    path[5] = (char) (maze / 10 + '0');
    path[6] = (char) (maze % 10 + '0');

    // open file to write win
    fstream win(path);

    if (win.is_open())
    {
        win.seekp(0, ios::end);  // starting point at the end of file
        win << left << setw(15) << name;  // size of name component
        win << internal << setw(8) << time << '\n';
        win.close();
    }
    else 
    {
        ofstream make_file(path); 
        make_file << "T02_G11 \nPlayer          - Time \n-----------------------\n"; 
        make_file << left << setw(15) << name;  // size of name component
        make_file << internal << setw(8) << time << '\n';
        make_file.close(); 
    }

    // read all lines in a vector
    ifstream readf(path);
    vector<string> file;
    string line;

    if (readf.is_open())
    {
        while (getline(readf, line))
        {
            file.push_back(line);
        }
        readf.close();
    }

    //sort the vector
    if (file.size() > 5)
    {
        sort(file.begin() + 4, file.end(), order);
    }

    // write back into the file
    ofstream writef(path);  // open file to read and write

    if (writef.is_open())
    {
        for (auto & i : file)
        {
            writef << i << endl;
        }
        writef.close();
    }
}

bool order(const string& a, const string& b) {return (stoi (a.substr(16, 8), nullptr) < stoi (b.substr(16, 8), nullptr));}


void leaderboard(){
    int level;
    int ret;
    string path = "MAZE_xx_WINNERS.TXT";

    cout << "Which level do you want to beat the records?" << endl;
    cin >> level;

    if(cin.eof()) return;
    // if he choose an invalid one, ask for another input while invalid!
    while (cin.fail() || (!check_path(level, path) && level != 0)) {
        cin.clear();
        cin.ignore(10000, '\n');
        cerr << "That's not a valid Maze! try another or '0' to return to main menu" << endl;
        cin >> level;
        if(cin.eof()) return;
    }
    if (level == 0) return;

    //read_level
    DisplayFile(path);
    cout << "\n\n0 to go back to main menu\n";

    // if he choose an invalid one, ask for another input while invalid!
    while ((cin.fail() || (ret != 0)) && !cin.eof()) {
        cin.clear();
        cin.ignore(10000, '\n');
        cin >> ret;
    }
}