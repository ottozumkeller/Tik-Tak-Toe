#define _NO_CRT_STDIO_INLINE
#define _CRT_SECURE_NO_WARNINGS

#include<stdio.h>
#include<windows.h>
#include<conio.h>

#define PLAYER1 'O'
#define PLAYER2 'X'
#define COLUMNS 19
#define ROWS 9

int end(int mrk[]) {
    int end = 1;
    for (int i = 0; i < 9; i++) {
        int j = 3 * i;
        if (i < 3 && (((mrk[j] & mrk[j + 1] & mrk[j + 2])) || ((mrk[i] & mrk[i + 3] & mrk[i + 6])) || ((mrk[0] & mrk[4] & mrk[8])) || ((mrk[2] & mrk[4] & mrk[6])))) {
            return 2;
        } else if (!mrk[i]) {
            end = 0;
        }
    }
    return end;
}

void print(int mrk[], int plyr) {
    printf("\x1b[8;%d;%dt\x1b]2; \a\x1b[2J\x1b[%dH\x1b(0", ROWS, COLUMNS, (ROWS - 7) / 2 + 1);
    SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), (COORD) { COLUMNS, ROWS });
    char args[7][15] = {
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
        int plyr_arr[3] = { ' ', PLAYER1, PLAYER2 };
        if (!end(mrk) ^ !plyr) {
            for (int j = 2; j < 11; j++) {
                if (args[i][j - 2] == 'x') {
                    args[i][j] = plyr_arr[mrk[index++]];
                }
            }
        } else {
            sprintf(args[i], "\n");
            sprintf(args[1], !plyr ? "SELECT MODE\n" : (end(mrk) == 2 ? "%c WINS!\n" : "TIE\n"), plyr_arr[plyr]);
            sprintf(args[3], "[1]   [2]\n");
            sprintf(args[5], "PLAYER(S)\n");
        }
        printf("\x1b[%dG%s", (COLUMNS - strlen(args[i])) / 2 + 2, args[i]);
    }
}

int minimax(int mrk[], int is_max) {
    int scores[2] = { 0, 1 - 2 * is_max };
    if (end(mrk)) {
        return scores[end(mrk) - 1];
    }
    int best = 1 - 2 * is_max;
    for (int i = 0; i < 9; i++) {
        if (!mrk[i]) {
            mrk[i] = is_max + 1;
            int score = minimax(mrk, !is_max);
            mrk[i] = 0;
            best = is_max ? max(score, best) : min(score, best);
        }
    }
    return best;
}

int best_move(int mrk[]) {
    int best = -1, move = 0;
    for (int i = 0; i < 9; i++) {
        if (!mrk[i]) {
            mrk[i] = 2;
            int score = minimax(mrk, 0);
            mrk[i] = 0;
            if (score > best) {
                best = score;
                move = i;
            }
        }
    }
    return move;
}

int move(int dir, int pos, int step, int lbound, int ubound) {
    int npos = pos + (dir / abs(dir)) * step;
    return npos > lbound && npos < ubound ? npos : pos;
}

void main() {
    SetWindowLongW(GetConsoleWindow(), GWL_STYLE, WS_VISIBLE | WS_CAPTION | WS_SYSMENU);
    int x = (COLUMNS + 1) / 2 - 3, y = (ROWS + 1) / 2, mrk[9] = { 0 }, mult = 0, plyr = 0;
    while (1) {
        int c;
        if (end(mrk) || !plyr) {
            x = (COLUMNS + 1) / 2 - (3 - 6 * mult);
            y = (ROWS + 1) / 2;
        }
        print(mrk, plyr);
        printf("\x1b[%d;%dH", y, x);
        if (end(mrk) || mult || plyr != 2) {
            c = _getch();
        }
        if (!end(mrk) && plyr) {
            if (!mult && plyr == 2) {
                mrk[best_move(mrk)] = 2;
                plyr = ((plyr - 1) ^ !end(mrk)) + 1;
            } else {
                switch (c) {
                case 224:;
                    int d = _getch() - 76;
                    if (d % 2) {
                        x = move(d, x, 4, (COLUMNS - 13) / 2, COLUMNS - (COLUMNS - 13) / 2);
                    }
                    else {
                        y = move(d, y, 2, (ROWS - 7) / 2, ROWS - (ROWS - 7) / 2);
                    }
                    break;
                case 13:;
                    int pos = ((y - (ROWS - 7) / 2) / 3) * 3 + (x - (COLUMNS - 13) / 2) % 3;
                    if (!mrk[pos]) {
                        mrk[pos] = plyr;
                        plyr = ((plyr - 1) ^ !end(mrk)) + 1;
                    }
                }
            }
        } else {
            switch (c) {
            case 224:;
                int k = _getch() - 76;
                if (k % 2) {
                    x = move(k, x, 6, (COLUMNS + 1) / 2 - 3, (COLUMNS + 1) / 2 + 3);
                }
                mult ^= 1;
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
        if (isxdigit(c)) {
            printf("%c\x1b[D", toupper(c));
            int d = _getch();
            if (isxdigit(d)) {
                const int col_arr[8] = { 0, 4, 2, 6, 1, 5, 3, 7 };
                printf("\x1b[%dm\x1b[%dm", col_arr[((c - 48) % 39) % 8] + (c > 55 ? 100 : 40), col_arr[((d - 48) % 39) % 8] + (d > 55 ? 90 : 30));
            }
        }
    }
}