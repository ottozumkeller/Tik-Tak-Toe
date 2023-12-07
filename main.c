#define _NO_CRT_STDIO_INLINE
#define _CRT_SECURE_NO_WARNINGS

#include<stdio.h>
#include<windows.h>
#include<conio.h>

#define PLAYER1 'O'
#define PLAYER2 'X'
#define COLUMNS 19
#define ROWS    9

int end(int mrk[]) {
    int end = 1;
    for (int i = 0; i < 9; i++) {
        const int j = 3 * i;
        if (i < 3 && (((mrk[j] & mrk[j + 1] & mrk[j + 2])) || ((mrk[i] & mrk[i + 3] & mrk[i + 6])) || ((mrk[0] & mrk[4] & mrk[8])) || ((mrk[2] & mrk[4] & mrk[6])))) {
            return 2;
        } else if (!mrk[i]) {
            end = 0;
        }
    }
    return end;
}

int minimax(int mrk[], int is_max) {
    int best = 1 - 2 * is_max;
    const int win = end(mrk);
    if (win) {
        return (win - 1) * best;
    }
    for (int i = 0; i < 9; i++) {
        if (!mrk[i]) {
            mrk[i] = is_max + 1;
            const int score = minimax(mrk, !is_max);
            mrk[i] = 0;
            best = is_max ? max(score, best) : min(score, best);
        }
    }
    return best;
}

void main() {
    SetWindowLongW(GetConsoleWindow(), GWL_STYLE, WS_VISIBLE | WS_CAPTION | WS_SYSMENU);
    int x = (COLUMNS + 1) / 2 - 3, y = (ROWS + 1) / 2, mrk[9] = { 0 }, mult = 0, plyr = 0;
    while (1) {
        if (end(mrk) || !plyr) {
            x = (COLUMNS + 1) / 2 - (3 - 6 * mult);
            y = (ROWS + 1) / 2;
        }
        printf("\x1b[8;%d;%dt", ROWS, COLUMNS);
        SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), (COORD) { COLUMNS, ROWS });
        printf("\x1b]2; \a\x1b[2J\x1b[%dH\x1b(0", (ROWS - 5) / 2);
        char rows[7][15] = {
            "lqqqwqqqwqqqk\n",
            "x   x   x   x\n",
            "tqqqnqqqnqqqu\n",
            "x   x   x   x\n",
            "tqqqnqqqnqqqu\n",
            "x   x   x   x\n",
            "mqqqvqqqvqqqj\r"
        };
        int index = 0;
        for (int i = 0; i < 7; i++) {
            static const int plyr_arr[3] = { ' ', PLAYER1, PLAYER2 };
            if (!end(mrk) ^ !plyr) {
                for (int j = 2; j < 11; j++) {
                    if (rows[i][j - 2] == 'x') {
                        rows[i][j] = plyr_arr[mrk[index++]];
                    }
                }
            } else {
                sprintf(rows[i], "\n");
                sprintf(rows[1], !plyr ? "SELECT MODE\n" : (end(mrk) == 2 ? "%c WINS!\n" : "TIE\n"), plyr_arr[plyr]);
                sprintf(rows[3], "[1]   [2]\n");
                sprintf(rows[5], "PLAYER(S)\r");
            }
            printf("\x1b[%dG%s", (COLUMNS - strlen(rows[i])) / 2 + 2, rows[i]);
        }
        printf("\x1b[%d;%dH", y, x);
        const int inp = end(mrk) || mult || plyr != 2 ? _getch() : 0;
        if (!end(mrk) && plyr) {
            if (!mult && plyr == 2) {
                int best = -1, move;
                for (int i = 0; i < 9; i++) {
                    if (!mrk[i]) {
                        mrk[i] = 2;
                        const int score = minimax(mrk, 0);
                        mrk[i] = 0;
                        if (score > best) {
                            best = score;
                            move = i;
                        }
                    }
                }
                mrk[move] = 2;
                plyr = ((plyr - 1) ^ !end(mrk)) + 1;
            } else {
                switch (inp) {
                case 224:;
                    int d = _getch() - 76;
                    if (d % 2) {
                        d *= 4;
                        if (abs(x - (COLUMNS + 1) / 2 + d) < 5) x += d; // for non wrapping cursor change "d * -8" to "0"
                    }
                    else {
                        d /= 2;
                        if (abs(y - (ROWS + 1) / 2 + d) < 3) y += d; // for non wrapping cursor change "-d" to "0"
                    }
                    break;
                case 13:;
                    const int pos = ((y - (ROWS - 7) / 2) / 3) * 3 + (x - (COLUMNS - 13) / 2) % 3;
                    if (!mrk[pos]) {
                        plyr = (((mrk[pos] = plyr) - 1) ^ !end(mrk)) + 1;
                    }
                }
            }
        } else {
            switch (inp) {
            case 224:;
                mult ^= _getch() % 2;
                break;
            case 13:;
                for (int i = 0; i < 9; i++) {
                    mrk[i] = 0;
                }
                x = (COLUMNS + 1) / 2;
                srand(GetTickCount64());
                plyr = mult ? rand() % 2 + 1 : 1;
            }
        }
        if (isxdigit(inp)) {
            printf("%c\x1b[D", toupper(inp));
            const int fgc = _getch();
            if (isxdigit(fgc) && inp != fgc) {
                printf("\x1b[48;5;%dm\x1b[38;5;%dm", inp % 39 - 9, fgc % 39 - 9);
            }
        }
    }
}