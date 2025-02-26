using namespace std;
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <conio.h>
#include <string>
#include <iostream>
#include <chrono>
#include <thread>
#include <iomanip> 
#define RED "\033[31m"
#define BRIGHT_RED "\033[91m"   
#define MAGENTA "\033[35m"
#define RESET "\033[0m"
#define BLUE "\033[34m" 
#define BRIGHT_YELLOW "\033[93m"  
// Function prototypes
void frontmenu();
void leaderboard();
void printBorder();
void loadData();
void sortLeaderboard();
void saveInfo();
void menu();
void getGameDifficulty();
void MainGame(char min[20][20], char khane[20][20], bool gameover);
void Zerocell(char min[20][20], char khane[20][20], int i, int j);
void game(char min[20][20], char khane[20][20], int i, int j);
void savedata(long long int time,bool win);
void saveNames(string name);
struct Player
{
    string name;
    long long int time=0;
    int score=0;
};

Player players[100];
// Global variables
int flags = 20;
int playerCounter = 0;
bool gameover = false;
int NumOfPlayers = 0;
int mines;
int click1;
int main()
{
    // Clear the console and display the menu
    system("cls");
    menu();
}
// Menu function to navigate between New Game, Leaderboard, and Exit
void menu()
{  
 char khane[20][20];// Player-visible board
    char min[20][20];//Minesweeper board with mines and numbers
    for (int i = 0; i < 20; i++)
    {
        for (int j = 0; j < 20; j++)
        {
            khane[i][j] = '?';
            min[i][j] = '0';
        }
    }
    srand(time(0));
    const int mines = 50;
    int realmin = 0;
    
    while (realmin < mines)
    {   
        int x = rand() % 20;//put random mines
        int y = rand() % 20;
        if (min[x][y] == '0')
        {
            min[x][y] = '9';
            realmin++;
        }
    }
    // Calculate numbers for cells adjacent to mines
    for (int i = 0; i < 20; i++){
        for (int j = 0; j < 20; j++)
        {
            int rows = 20;
            int cols = 20;
            int mincounter = 0;
            if (min[i][j] == '0')
            {
                if (i >= 1 && j >= 1 && min[i - 1][j - 1] == '9')
                    mincounter++;
                if (i >= 1 && min[i - 1][j] == '9')
                    mincounter++;
                if (i >= 1 && j < cols - 1 && min[i - 1][j + 1] == '9')
                    mincounter++;
                if (j >= 1 && min[i][j - 1] == '9')
                    mincounter++;
                if (j < cols - 1 && min[i][j + 1] == '9')
                    mincounter++;
                if (i < rows - 1 && j >= 1 && min[i + 1][j - 1] == '9')
                    mincounter++;
                if (i < rows - 1 && min[i + 1][j] == '9')
                    mincounter++;
                if (i < rows - 1 && j < cols - 1 && min[i + 1][j + 1] == '9')
                    mincounter++;
                min[i][j] = '0' + mincounter;
            }
        }
    }

    Player player;
    string name;
    system("cls");
    frontmenu();
start:
    char decision = getch();
    if (decision == '1')
    {
        system("cls");
        cout <<"𝑷𝒍𝒆𝒂𝒔𝒆 𝑬𝒏𝒕𝒆𝒓 𝒀𝒐𝒖𝒓 𝑵𝒂𝒎𝒆:";
        cin >>name;
        saveNames(name);
        system("cls");
        getGameDifficulty();
        MainGame(min, khane, false);
    }
    switch (decision)
    {
    case '2':
        system("cls");
         NumOfPlayers = 0;
         loadData();
        break;
    case '3':
        exit(0);
    default:
        cout << "𝐏𝐥𝐞𝐚𝐬𝐞 𝐄𝐧𝐭𝐞𝐫 𝐂𝐨𝐫𝐫𝐞𝐜𝐭 𝐈𝐧𝐩𝐮𝐭!";
        system("cls");
        goto start;
    }
}
// Main game loop
std::chrono::time_point<std::chrono::high_resolution_clock> startTime = std::chrono::high_resolution_clock::now();
void MainGame(char min[20][20], char khane[20][20], bool gameover)
{

    system("cls");
    int x = 0, y = 0;// Player cursor position
    do
    {


        int counter = 0;// Count opened cells
        system("cls");
        for (int i = 0; i < 20; i++)
        {
            for (int j = 0; j < 20; j++)
            {
                if (khane[i][j] != '?' && khane[i][j] != 'f')
                {
                    counter++;
                }
            }
        }
        if (counter == 400-mines)// Check for win condition
        {
            std::chrono::time_point<std::chrono::high_resolution_clock> endTime = std::chrono::high_resolution_clock::now();
            long long timer = chrono::duration_cast<chrono::seconds>(endTime - startTime).count();
            cout << "Congratulations! You Win!" << endl;
            savedata(timer,true);
            this_thread::sleep_for(chrono::seconds(3));
            system("cls");
            exit(0);
            
            gameover = true;
        }
        // Display game instructions    
        cout << "𝐏𝐫𝐞𝐬𝐬 𝐄𝐧𝐭𝐞𝐫 𝐭𝐨 𝐬𝐞𝐥𝐞𝐜𝐭" << endl;
        cout <<  "𝐏𝐫𝐞𝐬𝐬 𝐄𝐬𝐜 𝐭𝐨 𝐄𝐱𝐢𝐭"<< endl;
        cout << "𝐏𝐫𝐞𝐬𝐬 𝐟 𝐭𝐨 𝐮𝐬𝐞 𝐲𝐨𝐮𝐫 𝐅𝐥𝐚𝐠:"<< endl;
        printBorder();

        for (int i = 0; i < 20; i++)
        {
            cout << "\u2551";
            for (int j = 0; j < 20; j++)
            {
                if (i == x && j == y)
                {
                    cout << MAGENTA << khane[i][j] << " " << RESET;
                }
                else if (gameover && min[i][j] == '9')
                {
                    cout << RED << "✦" << " " << RESET;
                }
                else
                {
                    cout << khane[i][j] << " ";
                }
            }
            cout << "\u2551\n";
        }

        cout << "\u255A";
        for (int i = 0; i < 40; i++)
        {
            cout << "\u2550";
        }
        cout << "\u255D\n";
        if (flags >= 0)
        {
            cout << "𝐅𝐥𝐚𝐠𝐬 𝐑𝐞𝐦𝐚𝐢𝐧𝐢𝐧𝐠: " << flags << endl;
        }
        else
        {
            cout << "𝐘𝐨𝐮 𝐔𝐬𝐞 𝐀𝐥𝐥 𝐨𝐟 𝐘𝐨𝐮𝐫 𝐅𝐥𝐚𝐠𝐬!" << endl;
        }
        if (!gameover)// Capture player input
        {
            int input = getch();
            switch (input)
            {
            case 77: // Right
                y++;
                break;
            case 75: // Left
                y--;
                break;
            case 72: // Up
                x--;
                break;
            case 80: // Down
                x++;
                break;
            case 13:
                game(min, khane, x, y);
                break;
            case 27: // Escape
                system("cls");
                exit(0);
                break;
            case 'f': // Place a flag
                if (khane[x][y] == '?' && flags > 0)
                {
                    flags--;
                    khane[x][y] = 'f';
                    break;
                }
                else if (khane[x][y] == 'f') {
                    flags++;
                    khane[x][y] == '?';
                }
            }
        }
     // Ensure the player does not move out of the board boundaries!
        if (x < 0)
            x = 0;
        if (x > 19)
            x = 19;
        if (y < 0)
            y = 0;
        if (y > 19)
            y = 19;

    } while (!gameover);
}

void game(char min[20][20], char khane[20][20], int i, int j)
{
    if (min[i][j] == '9')//If player click on mine :[
    {
        MainGame(min, khane, true);
        std::chrono::time_point<std::chrono::high_resolution_clock> endTime = std::chrono::high_resolution_clock::now();
        long long timer = chrono::duration_cast<chrono::seconds>(endTime - startTime).count();//count the time the player played the game
        cout << "𝐆𝐚𝐦𝐞𝐎𝐯𝐞𝐫! 𝐒𝐞𝐞 𝐘𝐨𝐮 𝐍𝐞𝐱𝐭 𝐓𝐢𝐦𝐞! (︶︹︺)" << endl;
        this_thread::sleep_for(chrono::seconds(3));//show the massage for 3 seconds
        savedata(timer,false);
        system("cls");
        exit(0);
        
    }
    if (khane[i][j] == 'f')
    {
        khane[i][j] = min[i][j];
        MainGame(min, khane, false);
    }
    if (min[i][j] == '0')
    {
        Zerocell(min, khane, i, j);
        MainGame(min, khane, false);
    }
    else
    {
        khane[i][j] = min[i][j];
        MainGame(min, khane, false);
    }
}

void Zerocell(char min[20][20], char khane[20][20], int i, int j)
{
    if (khane[i][j] != '?')
        return;
    khane[i][j] = min[i][j];
    if (min[i][j] == '0')
    {
        for (int mx = -1; mx <= 1; mx++)//check around the cells
        {
            for (int my = -1; my <= 1; my++)
            {
                int ax = i + mx;
                int ay = j + my;
                if (ax >= 0 && ax < 20 && ay >= 0 && ay < 20 && min[ax][ay] != '9' && khane[ax][ay] == '?')
                {
                    Zerocell(min, khane, ax, ay);
                }
            }
        }
    }
}

void printBorder()
{
    cout << "\u2554";
    for (int i = 0; i < 40; i++)
    {
        cout << "\u2550";
    }
    cout << "\u2557\n";
}
// This function displays the menu with the following options:
// 1. New Game
// 2. Leaderboard
// 3. Exit
void frontmenu()
{
    cout << RED<< " █▄ ▄█ █ █▄ █ ██▀ ▄▀▀ "<<RESET<<BRIGHT_YELLOW<<"█   █ ██▀ ██▀ █▀▄ ██▀ █▀▄\n"<<RESET<<BRIGHT_YELLOW<<" █ ▀ █ █ █ ▀█ █▄▄ ▄██"<<RESET<<RED<<" ▀▄▀▄▀ █▄▄ █▄▄ █▀  █▄▄ █▀▄" <<RESET<< endl;

    cout << "             \u2554";
    for (int i = 0; i < 20; i++)
    {
        cout << "\u2550";
    }
    cout << "\u2557" << endl;
    cout << "             \u2551";
    for (int j = 0; j < 5; j++)
    {
        cout << " ";
    }
    cout << "1.𝙉𝙚𝙬 𝙂𝙖𝙢𝙚";
    for (int j = 0; j < 5; j++)
    {
        cout << " ";
    }
    cout << "\u2551" << endl;

    cout << "             " << "\u255A";
    for (int i = 0; i < 20; i++)
    {
        cout << "\u2550";
    }
    cout << "\u255D" << endl;
    //-----
    cout << "             \u2554";
    for (int i = 0; i < 20; i++)
    {
        cout << "\u2550";
    }
    cout << "\u2557" << endl;

    cout << "             \u2551";
    for (int j = 0; j < 3; j++)
    {
        cout << " ";
    }
    cout << "2.𝙇𝙚𝙖𝙙𝙚𝙧𝘽𝙤𝙖𝙧𝙙";
    for (int j = 0; j < 4; j++)
    {
        cout << " ";
    }
    cout << "\u2551" << endl;
    cout << "             \u255A";
    for (int i = 0; i < 20; i++)
    {
        cout << "\u2550";
    }
    cout << "\u255D" << endl;
    //------
    cout << "             \u2554";
    for (int i = 0; i < 20; i++)
    {
        cout << "\u2550";
    }
    cout << "\u2557" << endl;
    cout << "             \u2551";
    for (int j = 0; j < 7; j++)
    {
        cout << " ";
    }
    cout << "3.𝙀𝙭𝙞𝙩";
    for (int j = 0; j < 7; j++)
    {
        cout << " ";
    }
    cout << "\u2551" << endl;
    cout << "             \u255A";
    for (int i = 0; i < 20; i++)
    {
        cout << "\u2550";
    }
    cout << "\u255D" << endl;
}

void saveNames(string name)// Save player name when starting a game
{
    Player player;
    player.name = name;
    ofstream file("leaderboard.txt", ios::app);
    if (file.is_open())
    {
        file << player.name << " " ;
    }
    file.close();
}

void savedata(long long int time, bool win)//
{
    Player player;
    player.time = time;
    player.score = win ? 1000 - time : 0;
    fstream file("leaderboard.txt", ios::app);
    if (file.is_open())
    {
        file << player.score << " " << player.time << endl;
    }
    file.close();
    
}

void loadData()
{
    ifstream file("leaderboard.txt");
    if (file.is_open())
    {
        NumOfPlayers ;
        Player player;
        while (file >> player.name >> player.score >> player.time)
        {
            players[NumOfPlayers] = player;
            NumOfPlayers++;
        }
    }
    file.close();
    sortLeaderboard();
}
void sortLeaderboard()
{

    for (int i = 0; i < NumOfPlayers - 1; i++)
    {
        for (int j = i + 1; j < NumOfPlayers; j++)
        {
            if (players[i].score < players[j].score)
            {
                Player temp = players[i];
                players[i] = players[j];
                players[j] = temp;
            }
        }
    }
 saveInfo();   
}
void saveInfo()
{
    ofstream file("leaderboard.txt", ios::trunc);

    if(file.is_open())
    {
        for(int i = 0; i <NumOfPlayers ; i++)
        {
        file << players[i].name << "   " << players[i].score<< "  " << players[i].time << endl;
        }
    }
    leaderboard();
}

void leaderboard()
{   
    system("cls"); 
    cout <<BRIGHT_YELLOW<<" █   ██▀ ▄▀▄ █▀▄ ██▀ █▀▄ ██▄ ▄▀▄ ▄▀▄ █▀▄ █▀▄\n"<<RESET<<MAGENTA<<" █▄▄ █▄▄ █▀█ █▄▀ █▄▄ █▀▄ █▄█ ▀▄▀ █▀█ █▀▄ █▄▀\n"<<RESET<<endl;
    cout << endl;

    cout << setw(15)  << "𝑵𝒂𝒎𝒆" 
         << "          " << "𝑺𝒄𝒐𝒓𝒆" 
         << "     " << "𝑻𝒊𝒎𝒆" 
         << endl;
    cout << string(35, '_') << endl;

    for (int i = 0; i < NumOfPlayers; i++)
    {
        cout << setw(15) << left << players[i].name
             << setw(10) << players[i].score
             << setw(10) << players[i].time 
             << endl;
    }  

    cout << "\nPress any key to get back to menu";
    char get = getch();
    menu();
}
void getGameDifficulty() {
    system("cls");

    // Difficulty menu UI
    cout << "\u2554";
    for (int i = 0; i < 16; i++) {
        cout << "\u2550";
    }
    cout << "\u2557\n";
    cout << "\u2551" << "SelectDifficulty" << "\u2551\n";
    cout << "\u2560";
    for (int i = 0; i < 16; i++) {
        cout << "\u2550";
    }
    cout << "\u2563\n";
    cout << "\u2551" << "1. Easy         " << "\u2551\n";
    cout << "\u2551" << "                " << "\u2551\n";
    cout << "\u2551" << "2. Medium       " << "\u2551\n";
    cout << "\u2551" << "                " << "\u2551\n";
    cout << "\u2551" << "3. Hard         " << "\u2551\n";
    cout << "\u255A"; 
    for (int i = 0; i < 16; i++) {
        cout << "\u2550";
    }
    cout << "\u255D\n";

    // Input handling for difficulty selection.
    click1 = getch();
    switch (click1) {
        case '1' :
            mines = 40; // easy
            break;
        case '2' :
            mines = 66; // medium
            break;
        case '3' :
            mines = 100; // hard
            break;
    }
    flags = mines;
}
