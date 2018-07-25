#include<iostream>
#include<string>
#include<iomanip>
#include <algorithm>
#include <exception>
#include <fstream>
#include <ctime>
#include <sstream>

using namespace std;

struct QuitNow : public std::exception
{
    QuitNow() { }
    virtual ~QuitNow() throw() { }
    virtual const char* what() throw()
    {
        return 0;
    }
};

bool is_digits(const std::string &str)
{
    return std::all_of(str.begin(), str.end(), ::isdigit); // C++11
}

int win = 0;
bool player_turn = 0;
int block[19][19];
char menu = '0';
int board_size;
int blockOfGroup[19][19];
string input;
ostringstream oss;
int max_mark,player1_mark,player2_mark;

int write_trace(string content) {
    time_t now = time(0);
    tm *gmtm = gmtime(&now);
    char* dt = asctime(gmtm);
    ofstream  myfile;
    myfile.open("gameTrace.txt", ios::app);
    myfile << dt;
    myfile << "| " << content <<endl <<"_________________________" << endl;
    oss.str("");
    oss.clear();
    myfile.close();
    return 0;
}

int check_input() {
    getline(cin, input);
    if (input == "quit" || input == "Quit") {
        throw QuitNow();
    }
    else {
        input.erase(std::remove(input.begin(), input.end(), ' '), input.end());
    }
    return 0;
}

int rematch() {
    system("cls");
    cout << endl << "-----------------------------------" << endl <<
    "|      Congratulation player " << 2 - player_turn << "    |" << endl <<
    "|       You have won the game!    |" << endl <<
    "-----------------------------------" << endl << endl ;
    cout << "Do you want a rematch?" << endl;
    cout << "1. Yes" << endl;
    cout << "2. Back to main menu" << endl;
    cout << "3. Quit game" << endl;
    check_input();
    if (input[0] == '1') {
        write_trace("<---- Rematch ---->");
        return menu = '1';
    }
    if (input[0] == '2') {
        write_trace("<---- Game end ---->");
        return    menu = '0';
    }
    if (input[0] == '3') {
        throw QuitNow();
    }
    return 0;
}

void create_board() {
    cout << endl << "Score:" << endl << "Player 1: " << player1_mark << "  |  Player 2: " << player2_mark << endl << endl;
    for (int j = 0; j < board_size + 1; j++) {
        cout << " ";
        if (j == 0) {
            cout << " " << setw(3);
            for (int i = 0; i < board_size; i++)
            {
                cout << char(i + 65) << setw(2);
            }
            cout << endl;
            cout << "   --";
            for (int i = 0; i < board_size; i++)
            {
                cout << "--";
            }
            cout << endl;
        }
        else {
            cout << char(j + 64) << setw(2);
            cout << "|";
            for (int i = 0; i < board_size; i++)
            {
                if (block[i][j - 1] == 1) {
                    cout << "O" << setw(2);
                }
                if (block[i][j - 1] == 2) {
                    cout << "X" << setw(2);
                }
                if (block[i][j - 1] == 0) {
                    cout << "." << setw(2);
                }
            }
            cout << "|";
            cout << endl;
        }
    }
    cout << "   --";
    for (int i = 0; i < board_size; i++)
    {
        cout << "--";
    }
    cout << endl;
}

void make_move(int a, int b) {
    system("cls");
    if (block[a][b] != 0) {
        cout << "--------------------------" << endl << "|  Block already taken!  |" << endl << "--------------------------" << endl;
        player_turn = !player_turn;
    }
    else {
        block[a][b] = player_turn + 1;
        oss << "Player " << player_turn + 1 << " place a stone at " << a << " " << b;
        write_trace(oss.str());
    }
}
void about() {
    menu = '2';
    do {
        system("cls");
        cout << "1. About Go Game" << endl;
        cout << "2. How to play" << endl;
        cout << "3. Rules" << endl;
        cout << "4. Back" << endl;
        check_input();
        if (input[0] == '1'){
            system("cls");
            cout << "About Go game" << endl << "Go is a fascinating strategy board game that's been popular for at least 2,500 years, and probably more. Its simple rules and deep strategies have intrigued everyone from emperors to peasants for hundreds of generations. And they still do today. The game Go has fascinated people for thousands of years." << endl;
            system("pause");
        }
        if (input[0] == '2') {
            system("cls");
            cout << "How to play" << endl << "Each player 1 and player 2 will input their move position by inputting the corresponding coordinates." << endl;
            system("pause");
        }
        if (input[0] == '3') {
            system("cls");
            cout << "Rules" << endl << "A game of Go starts with an empty board. Each player has an effectively unlimited supply of pieces (called stones), one taking the black stones, the other taking white. The main object of the game is to use your stones to form territories by surrounding vacant areas of the board. It is also possible to capture your opponent's stones by completely surrounding them. Players take turns, placing one of their stones on a vacant point at each turn, with Black playing first.Note that stones are placed on the intersections of the lines rather than in the squares and once played stones are not moved.However they may be captured, in which case they are removed from the board, and kept by the capturing player as prisoners." << endl;
            system("pause");
        }
        if (input[0] == '4') {
            menu = '0';
        }
        
    } while (menu == '2');
}

void kill_block() {
    for (int i = 0; i <= board_size; i++) {
        for (int j = 0; j <= board_size; j++)
        {
            if ((blockOfGroup[i][j] == block[i][j]) && (block[i][j] == player_turn +1)) {
                if (player_turn == 0) {
                    ++player2_mark;
                }
                else {
                    ++player1_mark;
                }
                block[i][j] = 0;
            }
        }
    }
}

void check_group_block(int block_count) {
    int liberties = block_count*4;
    for (int i = 0; i < board_size; i++) {
        for (int j = 0; j < board_size; j++)
        {
            int block_liberties = 4;
            if (blockOfGroup[i][j] == player_turn + 1) {
                if ((j == board_size - 1) || (j == 0)) {
                    liberties--;
                    block_liberties--;
                }
                if ((i == board_size - 1) || (i == 0)) {
                    liberties--;
                    block_liberties--;
                }
                if ((block[i][j - 1] != 0) && (block[i][j - 1] != 9))
                {
                    liberties--;
                    block_liberties--;
                }
                if (block[i][j + 1] != 0)
                {
                    liberties--;
                    block_liberties--;
                }
                if (block[i - 1][j] != 0)
                {
                    liberties--;
                    block_liberties--;
                }
                if (block[i + 1][j] != 0)
                {
                    liberties--;
                    block_liberties--;
                }
                if (block_liberties == 3) {
                    if (blockOfGroup[i + 1][j + 1] == player_turn + 1) {}
                    {
                        if ((blockOfGroup[i + 1][j] == player_turn + 1) || (blockOfGroup[i + 1][j] == player_turn + 1)) {
                            liberties--;
                        }
                    }
                    if (block[i - 1][j - 1] == player_turn + 1) {
                        if ((block[i + 1][j] == player_turn + 1) || (blockOfGroup[i + 1][j] == player_turn + 1)) {
                            liberties--;
                        }
                    }
                    if (blockOfGroup[i + 1][j - 1] == player_turn + 1) {
                        if ((blockOfGroup[i + 1][j] == player_turn + 1) || (blockOfGroup[i + 1][j] == player_turn + 1)) {
                            liberties--;
                        }
                    }
                    if (blockOfGroup[i - 1][j + 1] == player_turn + 1) {
                        if ((blockOfGroup[i + 1][j] == player_turn + 1) || (blockOfGroup[i + 1][j] == player_turn + 1)) {
                            liberties--;
                        }
                    }
                }
                if (liberties == 0) {
                    kill_block();
                }
            }
        }
    }
    
}

void group_block() {
    int block_count = 0;
    for (int i = 0; i < board_size; i++) {
        int can_group = 2;
        for (int j = 0; j < board_size; j++)
        {
            if (can_group < 2) {
                can_group = 2;
            }
            if (block[i][j] == player_turn + 1) {
                if (j == board_size - 1) {
                    can_group--;
                }
                if (i == board_size - 1) {
                    can_group--;
                }
                if (block[i][j + 1] != player_turn + 1) {
                    can_group--;
                }
                if (block[i][j + 1] == player_turn + 1) {
                    can_group++;
                }
                if (block[i + 1][j] != player_turn + 1) {
                    can_group--;
                }
                if (block[i + 1][j] == player_turn + 1)
                {
                    can_group++;
                }
                blockOfGroup[i][j] = player_turn + 1;
                block_count++;
                if (can_group <= 0) {
                    check_group_block(block_count);
                    memset(blockOfGroup, 0, sizeof(blockOfGroup[0][0]) * 19 * 19);
                    block_count = 0;
                }
            }
        }
    }
}

void check_board() {
    player_turn = !player_turn;
    group_block();
    create_board();
    if (player1_mark == max_mark || player2_mark == max_mark) {
        win = 1;
    }
}

int coutboard()
{
    menu = '1.1';
    win = 0;
    player1_mark = 0;
    player2_mark = 0;
    player_turn = 0;
    memset(block, 0, sizeof(block[0][0]) * 19 * 19);
    do {
        system("cls");
        create_board();
        do {
            if (!player_turn) {
                cout << endl << "Player 1 please make your move: (insert 'give up' to end current game)" << endl;
            }
            else
            {
                cout << endl << "Player 2 please make your move: (insert 'give up' to end current game)" << endl;
            }
            check_input();
            if (input == "giveup") {
                oss << "<---- Game end by player "<< player_turn + 1 <<" board ---->";
                write_trace(oss.str());
                win = 1;
            }
            else {
                while (!((int)input[0] < char(board_size + 65) && (int)input[0]>64) || !((int)input[1] < char(board_size + 65) && (int)input[1]>64)) {
                    cout << "Please input again: ";
                    check_input();
                    if (input == "giveup") {
                        oss << "<---- Game end by player " << player_turn + 1 << " ---->";
                        write_trace(oss.str());
                        win = 1;
                        break;
                    }
                }
                if (win!=1){
                    make_move((int)input[0] - 65, (int)input[1] - 65);
                    check_board();
                }
            }
        } while (win == 0);
        rematch();
    } while (menu == '1.1');
    
    return 0;
}

int create_game() {
    menu = '1';
    do {
        system("cls");
        cout << "Please select size of the board" << endl;
        cout << "--------------------" << endl;
        cout << "|     1. 9x9       |" << endl;
        cout << "|     2. 13x13     |" << endl;
        cout << "|     3. 17x17     |" << endl;
        cout << "|     4. 19x19     |" << endl;
        cout << "|     5. back      |" << endl;
        cout << "--------------------" << endl;
        check_input();
        if (input[0] == '5') {
            return menu = '0';
        }
        while (!(input[0] == '1' || input[0] == '2' || input[0] == '3' || input[0] == '4'))
        {
            cout << "Please input again:" << endl;
            check_input();
            if (input[0] == '5') {
                return menu = '0';
            }
        }
        if (input[0] == '1') { board_size = 9; }
        if (input[0] == '2') { board_size = 13; }
        if (input[0] == '3') { board_size = 17; }
        if (input[0] == '4') { board_size = 19; }
        oss << "<---- Game begin with " << board_size << "x" << board_size << " ---->";
        write_trace(oss.str());
        system("cls");
        cout << "How many stones are captured to win the game?" << endl;
        check_input();
        while (!is_digits(input))
        {
            cout << "Please input a valid number:" << endl;
            check_input();
        }
        max_mark = atoi(input.c_str());
        coutboard();
        
    } while (menu == '1');
    return 0;
}

int main()
{
    try
    {
        write_trace("<---- Game start ---->");
        do {
            system("cls");
            bool valid = false;
            cout << endl << "  Welcome to Go Game  " << endl << endl;
            cout << "-----------------------" << endl;
            cout << "| 1. Start a new game |" << endl;
            cout << "| 2. About            |" << endl;
            cout << "| 3. Quit             |" << endl;
            cout << "-----------------------" << endl << endl;
            cout << "Please choose option:" << endl;
            check_input();
            while (input[0] != '1' && input[0] != '2' && input[0] != '3')
            {
                cout << "Please input again: ";
                check_input();
            }
            menu = input[0];
            switch (menu)
            {
                case '1':
                {
                    create_game();
                    break;
                }
                case '2':
                {   about();
                    break;
                }
                case '3':
                {throw QuitNow();
                    break; }
            }
        } while (menu == '0');
    }
    catch (QuitNow) { write_trace("<---- Quit Game ---->"); }
    return 0;
    
}
