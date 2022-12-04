#include <stdio.h>
#include <string.h>
#include <windows.h>

#define PERR(bSuccess, api) { if(!(bSuccess)) printf("%s:Error %d from %s on line %d\n", __FILE__, GetLastError(), api, __LINE__); }

void cls();
void gotoxy(short x, short y);
int check_board();
void MouseEventProc(MOUSE_EVENT_RECORD mer);

int turn, count;
char board[3][3];

int main() {
    cls();

    char BORDER[] = "\
#############################\n\
##       ##       ##       ##\n\
##       ##       ##       ##\n\
##       ##       ##       ##\n\
##       ##       ##       ##\n\
##       ##       ##       ##\n\
#############################\n\
##       ##       ##       ##\n\
##       ##       ##       ##\n\
##       ##       ##       ##\n\
##       ##       ##       ##\n\
##       ##       ##       ##\n\
#############################\n\
##       ##       ##       ##\n\
##       ##       ##       ##\n\
##       ##       ##       ##\n\
##       ##       ##       ##\n\
##       ##       ##       ##\n\
#############################\n";

    for(int i = 0, n = strlen(BORDER); i < n; ++i)
        if(BORDER[i] == '#') BORDER[i] = 178;

    printf(BORDER);
    printf("Use your mouse to play\n");
    printf("Press X to EXIT\n");

    for(int i = 0; i < 3; ++i) {
        for(int j = 0; j < 3; ++j) {
            board[i][j] = '-';
        }
    }

    INPUT_RECORD InRec;
    DWORD NumRead;
    HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);
    turn = 'X';

    while(1) {
        ReadConsoleInput(hIn, &InRec, 1, &NumRead);

        if(InRec.EventType == MOUSE_EVENT) {
            MouseEventProc(InRec.Event.MouseEvent);

            int result = check_board();

            if(result) {
                gotoxy(30, 9);

                if(result == 1)
                    printf("X win\n");
                else if(result == 2)
                    printf("O win\n");

                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);

                if(result == 3)
                    printf("Draw");

                gotoxy(0, 20);

                return 0;
            }
        } else if (InRec.EventType == KEY_EVENT) {
            if(InRec.Event.KeyEvent.uChar.AsciiChar == 'x') {
                cls();
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);

                return 0;
            }
        }
    }
}

void cls() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD coordScreen = {0, 0};    /* here's where we'll home the
                                        cursor */ 
    BOOL bSuccess;
    DWORD cCharsWritten;
    CONSOLE_SCREEN_BUFFER_INFO csbi; /* to get buffer info */ 
    DWORD dwConSize;                 /* number of character cells in
                                        the current buffer */ 
 
    /* get the number of character cells in the current buffer */ 
 
    bSuccess = GetConsoleScreenBufferInfo(hConsole, &csbi);
    PERR( bSuccess, "GetConsoleScreenBufferInfo" );
    dwConSize = csbi.dwSize.X * csbi.dwSize.Y;
 
    /* fill the entire screen with blanks */ 
 
    bSuccess = FillConsoleOutputCharacter(hConsole, (TCHAR) ' ',
       dwConSize, coordScreen, &cCharsWritten);
    PERR( bSuccess, "FillConsoleOutputCharacter" );
 
    /* get the current text attribute */ 
 
    bSuccess = GetConsoleScreenBufferInfo(hConsole, &csbi);
    PERR( bSuccess, "ConsoleScreenBufferInfo" );
 
    /* now set the buffer's attributes accordingly */ 
 
    bSuccess = FillConsoleOutputAttribute(hConsole, csbi.wAttributes,
               dwConSize, coordScreen, &cCharsWritten);
    PERR(bSuccess, "FillConsoleOutputAttribute");
 
    /* put the cursor at (0, 0) */ 
 
    bSuccess = SetConsoleCursorPosition( hConsole, coordScreen );
    PERR(bSuccess, "SetConsoleCursorPosition");
 }

void gotoxy(short x, short y) {
    COORD coord = {x, y};

    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

int check_board() {
    int i;
    
    for (i = 0; i < 3; i++) {
        /*Check Horizontal*/
        if (board[i][0] == board[i][1] && board[i][1] == board[i][2]) {
            if (board[i][0] == 'X')
                return 1;
            else if (board[i][0] == 'O')
                return 2;
        }
        
        /*Check Vertikal*/
        if (board[0][i] == board[1][i] && board[1][i] == board[2][i]) {
            if (board[0][i] == 'X')
                return 1;
            else if (board[0][i] == 'O')
                return 2;
        }
    }
    
    /*Check Diagonal*/
    if (board[0][0] == board[1][1] && board[1][1] == board[2][2]) {
        if (board[0][0] == 'X')
            return 1;
        else if (board[0][0] == 'O')
            return 2;
    }else if (board[0][2] == board[1][1] && board[1][1] == board[2][0]) {
        if (board[0][2] == 'X')
            return 1;
        else if (board[0][2] == 'O')
            return 2;
    }

    if(count == 9) return 3;
    else return 0;
}

void MouseEventProc(MOUSE_EVENT_RECORD mer) {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);

    if(mer.dwEventFlags == 0 && mer.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED || 
        mer.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED & MOUSE_MOVED) {
        short x = mer.dwMousePosition.X,
              y = mer.dwMousePosition.Y,
              flag = 0,
              start_x, start_y;
        
        if(x > 1 && x < 9 && y > 0 && y < 6) {
            start_x = 4;
            start_y = 2;

            if(board[0][0] == '-') {
                board[0][0] = turn;
                flag = 1;
            }
        } else if(x > 10 && x < 18 && y > 0 && y < 6) {
            start_x = 13;
            start_y = 2;

            if(board[0][1] == '-') {
                board[0][1] = turn;
                flag = 1;
            }
        } else if(x > 19 && x < 27 && y > 0 && y < 6) {
            start_x = 22;
            start_y = 2;

            if(board[0][2] == '-') {
                board[0][2] = turn;
                flag = 1;
            }
        } else if(x > 1 && x < 9 && y > 6 && y < 12) {
            start_x = 4;
            start_y = 8;

            if(board[1][0] == '-') {
                board[1][0] = turn;
                flag = 1;
            }
        } else if(x > 10 && x < 18 && y > 6 && y < 12) {
            start_x = 13;
            start_y = 8;

            if(board[1][1] == '-') {
                board[1][1] = turn;
                flag = 1;
            }
        } else if(x > 19 && x < 27 && y > 6 && y < 12) {
            start_x = 22;
            start_y = 8;

            if(board[1][2] == '-') {
                board[1][2] = turn;
                flag = 1;
            }
        } else if(x > 1 && x < 9 && y > 12 && y < 18) {
            start_x = 4;
            start_y = 14;

            if(board[2][0] == '-') {
                board[2][0] = turn;
                flag = 1;
            }
        } else if(x > 10 && x < 18 && y > 12 && y < 18) {
            start_x = 13;
            start_y = 14;

            if(board[2][1] == '-') {
                board[2][1] = turn;
                flag = 1;
            }
        } else if(x > 19 && x < 27 && y > 12 && y < 18) {
            start_x = 22;
            start_y = 14;

            if(board[2][2] == '-') {
                board[2][2] = turn;
                flag = 1;
            }
        } 

        if(flag) {
            count += 1;
             if(turn == 'X') {
                SetConsoleTextAttribute(hOut, 0xC);

                for(int i = 0; i < 3; ++i) {
                    gotoxy(start_x + 2 - i, start_y + i);

                    putchar(178);

                    gotoxy(start_x + i, start_y + i);

                    putchar(178);
                }
            } else {
                SetConsoleTextAttribute(hOut, 0xA);

                for(int i = 0; i < 3; ++i) {
                    gotoxy(start_x, start_y + i);

                    if(i == 1)
                        printf("\xB2 \xB2");
                    else printf("\xB2\xB2\xB2");
                }
            }

            if(turn == 'X') turn = 'O';
            else turn = 'X';
        }
       
    } else if(mer.dwEventFlags == 0 && mer.dwButtonState == RIGHTMOST_BUTTON_PRESSED || 
        mer.dwButtonState == RIGHTMOST_BUTTON_PRESSED & MOUSE_MOVED) {
        gotoxy(mer.dwMousePosition.X, mer.dwMousePosition.Y);
    }
}
