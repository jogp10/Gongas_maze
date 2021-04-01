//T02_G11

#include <iostream>
#include <fstream>
#include <string>
#include <thread>
#include <chrono>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <C:\Users\zebar\Source\Repos\zearaujo11\T02_G11\play.cpp>

using namespace std;

/**
Main menu
 */
int menu();

/**
Rules of the game
 */
void ReadRules();

/**
Start of the game
Let's play
 */
void play();

/**
 * Write winner's name and time in record
 * @param name of the player
 * @param time of the play
 * @param maze in which level
 */
void winner(string name, int time, int maze);
bool order(string a, string b);

int main() {
    int menu_choice;

Menu:
    menu_choice = menu();

    switch(menu_choice)
    {
        case 1:     // Rules of the game
            ReadRules();
            goto Menu;

        case 2:    // Start the game
            play();
            break;

        default:
            this_thread::sleep_for(chrono::seconds(2));
            break;
    }

    return 0;
}
