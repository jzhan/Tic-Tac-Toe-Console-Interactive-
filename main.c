#include <stdio.h>
#include <string.h>
#include <windows.h>

#define PERR(bSuccess, api) { if(!(bSuccess)) printf("%s:Error %d from %s on line %d\n", __FILE__, GetLastError(), api, __LINE__); }

void cls();
void gotoxy(HANDLE hOut, short x, short y);
int validate(HANDLE hOut, int result);
int check_board();
void move(HANDLE hOut, int x, int y, int i, int j);
void MouseEventProc(HANDLE hOut, MOUSE_EVENT_RECORD mer);
void toDigital(HANDLE hOut, char *str, int x, int y);

int turn, count;
char board[3][3];

int main() {
    char BORDER[] = "\
#############################\n\
##1      ##2      ##3      ##\n\
##       ##       ##       ##\n\
##       ##       ##       ##\n\
##       ##       ##       ##\n\
##       ##       ##       ##\n\
#############################\n\
##4      ##5      ##6      ##\n\
##       ##       ##       ##\n\
##       ##       ##       ##\n\
##       ##       ##       ##\n\
##       ##       ##       ##\n\
#############################\n\
##7      ##8      ##9      ##\n\
##       ##       ##       ##\n\
##       ##       ##       ##\n\
##       ##       ##       ##\n\
##       ##       ##       ##\n\
#############################\n";

    for(int i = 0, n = strlen(BORDER); i < n; ++i)
        if(BORDER[i] == '#') BORDER[i] = 178;
    
    for(int i = 0; i < 3; ++i)
        for(int j = 0; j < 3; ++j)
            board[i][j] = '-';

    HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE),
           hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    INPUT_RECORD InRec;
    DWORD NumRead;
    turn = 'X';

    cls(hOut);
    printf(BORDER);
    printf("Use your mouse or number keys to play\n");
    printf("Press X to EXIT\n");
    
    while(1) {
        ReadConsoleInput(hIn, &InRec, 1, &NumRead);

        if(InRec.EventType == MOUSE_EVENT) {
            MouseEventProc(hOut, InRec.Event.MouseEvent);

            if(validate(hOut, check_board())) 
                return 0;
        } else if (InRec.EventType == KEY_EVENT) {
            switch(InRec.Event.KeyEvent.uChar.AsciiChar) {
                case '1':
                    move(hOut, 4, 2, 0, 0);

                    break;
                case '2':
                    move(hOut, 13, 2, 0, 1);

                    break;
                case '3':
                    move(hOut, 22, 2, 0, 2);

                    break;
                case '4':
                    move(hOut, 4, 8, 1, 0);

                    break;
                case '5':
                    move(hOut, 13, 8, 1, 1);

                    break;
                case '6':
                    move(hOut, 22, 8, 1, 2);

                    break;
                case '7':
                    move(hOut, 4, 14, 2, 0);

                    break;
                case '8':
                    move(hOut, 13, 14, 2, 1);

                    break;
                case '9':
                    move(hOut, 22, 14, 2, 2);

                    break;
                case 'x':
                    cls(hOut);
                    SetConsoleTextAttribute(hOut, 7);

                    return 0;
            }

            if(validate(hOut, check_board())) 
                return 0;
        }
    }
}

void cls(HANDLE hOut) {
    COORD coordScreen = {0, 0};    /* here's where we'll home the
                                        cursor */ 
    BOOL bSuccess;
    DWORD cCharsWritten;
    CONSOLE_SCREEN_BUFFER_INFO csbi; /* to get buffer info */ 
    DWORD dwConSize;                 /* number of character cells in
                                        the current buffer */ 
 
    /* get the number of character cells in the current buffer */ 
 
    bSuccess = GetConsoleScreenBufferInfo(hOut, &csbi);

    PERR( bSuccess, "GetConsoleScreenBufferInfo" );
    
    dwConSize = csbi.dwSize.X * csbi.dwSize.Y;
 
    /* fill the entire screen with blanks */ 
 
    bSuccess = FillConsoleOutputCharacter(hOut, (TCHAR) ' ',
                dwConSize, coordScreen, &cCharsWritten);

    PERR(bSuccess, "FillConsoleOutputCharacter");
 
    /* get the current text attribute */ 
 
    bSuccess = GetConsoleScreenBufferInfo(hOut, &csbi);

    PERR(bSuccess, "ConsoleScreenBufferInfo");
 
    /* now set the buffer's attributes accordingly */ 
 
    bSuccess = FillConsoleOutputAttribute(hOut, csbi.wAttributes,
               dwConSize, coordScreen, &cCharsWritten);

    PERR(bSuccess, "FillConsoleOutputAttribute");
 
    /* put the cursor at (0, 0) */ 
 
    bSuccess = SetConsoleCursorPosition( hOut, coordScreen );

    PERR(bSuccess, "SetConsoleCursorPosition");
 }

void gotoxy(HANDLE hOut,short x, short y) {
    COORD coord = {x, y};

    SetConsoleCursorPosition(hOut, coord);
}

int validate(HANDLE hOut, int result) {
    if(result) {
        gotoxy(hOut, 30, 9);

        if(result == 1)
            toDigital(hOut, "X Wins", 32, 7);
        else if(result == 2)
            toDigital(hOut, "O Wins", 32, 7);

        SetConsoleTextAttribute(hOut, 7);

        if(result == 3)
            printf("Draw");

        gotoxy(hOut, 0, 20);

        return 1;
    }

    return 0;
}

int check_board() {
    for (int i = 0; i < 3; i++) {
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

void move(HANDLE hOut, int x, int y, int i, int j) {  
    if(board[i][j] != '-')
        return;

    board[i][j] = turn;
    count = count + 1;

    if(turn == 'X') {
        SetConsoleTextAttribute(hOut, 0xC);

        for(int i = 0; i < 3; ++i) {
            gotoxy(hOut, x + 2 - i, y + i);
            putchar(178);

            gotoxy(hOut, x + i, y + i);
            putchar(178);
        }
    } else {
        SetConsoleTextAttribute(hOut, 0xA);

        for(int i = 0; i < 3; ++i) {
            gotoxy(hOut, x, y + i);

            if(i == 1)
                printf("\xB2 \xB2");
            else printf("\xB2\xB2\xB2");
        }
    }

    if(turn == 'X') turn = 'O';
    else turn = 'X';
}

void MouseEventProc(HANDLE hOut, MOUSE_EVENT_RECORD mer) {
    if(mer.dwEventFlags == 0 && mer.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED || 
        mer.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED & MOUSE_MOVED) {
        short x = mer.dwMousePosition.X,
              y = mer.dwMousePosition.Y;
        
        if(x > 1 && x < 9 && y > 0 && y < 6) {
            move(hOut, 4, 2, 0, 0);
        } else if(x > 10 && x < 18 && y > 0 && y < 6) {
            move(hOut, 13, 2, 0, 1);
        } else if(x > 19 && x < 27 && y > 0 && y < 6) {
           move(hOut, 22, 2, 0, 2);
        } else if(x > 1 && x < 9 && y > 6 && y < 12) {
            move(hOut, 4, 8, 1, 0);
        } else if(x > 10 && x < 18 && y > 6 && y < 12) {
            move(hOut, 13, 8, 1, 1);
        } else if(x > 19 && x < 27 && y > 6 && y < 12) {
            move(hOut, 22, 8, 1, 2);
        } else if(x > 1 && x < 9 && y > 12 && y < 18) {
            move(hOut, 4, 14, 2, 0);
        } else if(x > 10 && x < 18 && y > 12 && y < 18) {
            move(hOut, 13, 14, 2, 1);
        } else if(x > 19 && x < 27 && y > 12 && y < 18) {
            move(hOut, 22, 14, 2, 2);
        } 
    } else if(mer.dwEventFlags == 0 && mer.dwButtonState == RIGHTMOST_BUTTON_PRESSED || 
        mer.dwButtonState == RIGHTMOST_BUTTON_PRESSED & MOUSE_MOVED) {
        gotoxy(hOut, mer.dwMousePosition.X, mer.dwMousePosition.Y);
    }
}

void toDigital(HANDLE hOut, char *str, int x, int y) {
    int pattern[26] = {15269425, 32045630, 33047071, 32032318, 33061407, 33062416,
                       33050175, 18415153, 14815374, 32564782, 18444881, 17318431, 
                       18732593, 18667121, 15255086, 32045584, 33080925, 32045649, 
                       33061951, 32641156, 18400831, 18393412, 18405233, 18157905,
                       18157700, 32575775};
 
    int num_pattern[10] = {33150783, 13504671, 15309599, 31505470, 2304994, 33060926, 15235630,
                        32586884, 15252014, 15252526};
 
    int l, m;
    int len = strlen(str);

    for(int j = 4; j >= 0; --j) {
        gotoxy(hOut, x, y);

        for(int k = 0; k < len; k++) {
            if(str[k] == ' ') {
                printf("  ");
                
                continue;
            }
    
            if(str[k] >= '0' && str[k] <= '9') {
                m = (num_pattern[str[k] - '0'] >> (j * 5));
                l = 4;
            } else if(str[k] == '\'') {
                m = (26624 >> (j * 3));
                l = 2;
            } else {
                m = (pattern[tolower(str[k]) - 'a'] >> (j * 5));
                l = 4;
            }
    
            for(; l >= 0; --l)
                printf("%c", ((m & (1 << l))? (char) 178:' '));
    
            printf(" ");
        }

        printf("\n");

        y += 1;
    }
}
