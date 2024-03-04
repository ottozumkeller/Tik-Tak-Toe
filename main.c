#define _NO_CRT_STDIO_INLINE
#define _CRT_SECURE_NO_WARNINGS

#include<stdio.h>
#include<windows.h>
#include<conio.h>

#define PLYR1 'O'
#define PLYR2 'X'

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
    int columns = 0, rows = 0, x = 0, y = 0, mrk[9] = { 0 }, mult = 0, plyr = 0;
    while (1) {
        CONSOLE_SCREEN_BUFFER_INFO info;
        GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);
        int ncolumns = info.srWindow.Right - info.srWindow.Left + 1;
        int nrows = info.srWindow.Bottom - info.srWindow.Top + 1;
        x += (ncolumns + 1) / 2 - (columns + 1) / 2;
        y += (nrows + 1) / 2 - (rows + 1) / 2;
        columns = ncolumns;
        rows = nrows;
        SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), (COORD) {columns, rows});
        printf("\33[2J\33]0; \a\33[%dH\33(0", (rows - 5) / 2);
        char lines[7][15] = {
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
            const int plyr_arr[3] = { ' ', PLYR1, PLYR2 };
            if (!end(mrk) ^ !plyr) {
                for (int j = 2; j < 11; j++) {
                    if (lines[i][j - 2] == 'x') {
                        lines[i][j] = plyr_arr[mrk[index++]];
                    }
                }
            }
            else {
                sprintf(lines[i], "\n");
                sprintf(lines[1], !plyr ? " SELECT MODE \n" : (end(mrk) == 2 ? "   %c WINS!  \n" : "     TIE!    \n"), plyr_arr[plyr]);
                sprintf(lines[3], "  [1]   [2]  \n");
                sprintf(lines[5], "  PLAYER(S)  \r");
            }
            printf("\33[%dG%s", (columns - 11) / 2, lines[i]);
        }
        if (end(mrk) || !plyr) {
            x = (columns + 1) / 2 - (3 - 6 * mult);
            y = (rows + 1) / 2;
        }
        printf("\33[%d;%dH", y, x);
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
                        x += (abs(x - (columns + 1) / 2 + d) < 5) ? d : -2 * d;
                    } else {
                        d /= 2;
                        y += (abs(y - (rows + 1) / 2 + d) < 3) ? d : -2 * d;
                    }
                    break;
                case 13:;
                    const int pos = ((y - (rows - 7) / 2) / 3) * 3 + (x - (columns - 13) / 2) % 3;
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
                memset(mrk, 0, 36);
                x = (columns + 1) / 2;
                y = (rows + 1) / 2;
                srand(GetTickCount64());
                plyr = mult ? rand() % 2 + 1 : 1;
            }
        }
        if (isxdigit(inp)) {
            printf("%c\33[D", toupper(inp));
            const int fgc = _getch();
            if (isxdigit(fgc) && inp != fgc) {
                printf("\33[48;5;%dm\33[38;5;%dm", inp % 39 - 9, fgc % 39 - 9);
            }
        }
    }
}