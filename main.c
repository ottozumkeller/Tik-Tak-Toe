/*
   ┌─────────────────────────┬────────┐
   │ Best standart file size │ 1219 B │
   ├─────────────────────────┼────────┤
   │ Best minimal file size  │ 1160 B │
   ├─────────────────────────┼────────┤
   │ Best barebone file size │  981 B │
   └─────────────────────────┴────────┘
*/

#define _NO_CRT_STDIO_INLINE
#define _CRT_SECURE_NO_WARNINGS

// #define _DEBUG
// #define _COLOR
#define _AI

#include <stdio.h>
#include <windows.h>
#include <conio.h>

#define PLYRO 'O'
#define PLYRX 'X'

int end(int mrk[]) {
    int end = 1;
    for (int i = 0; i < 9; i++) {
        if (i < 3 && ((mrk[3 * i] & mrk[3 * i + 1] & mrk[3 * i + 2]) || (mrk[i] & mrk[i + 3] & mrk[i + 6]) || (mrk[0] & mrk[4] & mrk[8]) || (mrk[2] & mrk[4] & mrk[6]))) {
            return 2;
        } else if (!mrk[i]) {
            end = 0;
        }
    }
    return end;
}

#ifdef _AI
int minimax(int mrk[], int is_max) {
    int best = 1 - 2 * is_max;
    if (end(mrk)) {
        return (end(mrk) - 1) * best * 2;
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
#endif

void main() {
    int mrk[9] = { 0 }, mult = 0, plyr = 0, midcolumns = 0, midrows = 0, x = 0, y = 0;
#ifndef _AI
    mult = 1;
#endif
    #ifdef _DEBUG
    int _d = 0, _move = 0, _bgc = 0, _fgc = 0;
    #endif
    while (1) {
        CONSOLE_SCREEN_BUFFER_INFO info;
        GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);
        int columns = info.srWindow.Right - info.srWindow.Left + 1;
        int rows = info.srWindow.Bottom - info.srWindow.Top + 3;
        x += columns / 2 - midcolumns;
        y += rows / 2 - midrows;
        midcolumns = columns / 2;
        midrows = rows / 2;
        printf("\33[?1049h\33[%dH\33(0", midrows - 3);
        int feed = midcolumns - 6;
        char lines[142] = "\33[%dGlqqqwqqqwqqqk\n\33[%dGx %c x %c x %c x\n\33[%dGtqqqnqqqnqqqu\n\33[%dGx %c x %c x %c x\n\33[%dGtqqqnqqqnqqqu\n\33[%dGx %c x %c x %c x\n\33[%dGmqqqvqqqvqqqj";
        if (!end(mrk) == !plyr) {
            sprintf(lines, "\n\33[%dG %s \n\n\33[%dG  [1]   [2]  \n\n\33[%dG  PLAYER(S)", feed, !plyr ? "SELECT MODE" : (end(mrk) == 2 ? (plyr == 1 ? "  O WINS! " : "  X WINS! ") : "    TIE!   "), feed, feed);
        }
        static const char plyr_arr[3] = { ' ', PLYRO, PLYRX };
        printf(lines, feed, feed, plyr_arr[mrk[0]], plyr_arr[mrk[1]], plyr_arr[mrk[2]], feed, feed, plyr_arr[mrk[3]], plyr_arr[mrk[4]], plyr_arr[mrk[5]], feed, feed, plyr_arr[mrk[6]], plyr_arr[mrk[7]], plyr_arr[mrk[8]], feed);
        if (end(mrk) || !plyr) {
            x = midcolumns - (3 - 6 * mult);
            y = midrows;
        }
        #ifdef _DEBUG
        printf("\33[HMRK: %d%d%d%d%d%d%d%d%d; PLYR: %d; MULT: %d; X: %d; Y: %d; D: %d: MOVE: %d; BGC: %d; FGC: %d", mrk[0], mrk[1], mrk[2], mrk[3], mrk[4], mrk[5], mrk[6], mrk[7], mrk[8], plyr, mult, x, y, _d, _move, _bgc, _fgc);
        #endif
        printf("\33[%d;%dH", y, x);
        int inp = mult || plyr != 2 || end(mrk) ? _getch() : 0;
        if (!end(mrk) && plyr) {
            int move = 0, best = -1;
#ifdef _AI
            if (!mult && plyr == 2) {
                for (int i = 0; i < 9; i++) {
                    if (mrk[i] == 0) {
                        mrk[i] = 2;
                        int score = minimax(mrk, 0);
                        mrk[i] = 0;
                        if (score > best) {
                            best = score;
                            move = i;
                        }
                    }
                }
            }
            else {
#endif
                switch (inp) {
                case 224:;
                    int d = _getch() - 76;
                    #ifdef _DEBUG
                    _d = d + 76;
                    #endif
                    if (d % 2) {
                        d *= 4;
                        x += (abs(x - midcolumns + d) < 5) ? d : -2 * d;
                    } else {
                        d /= 2;
                        y += (abs(y - midrows + d) < 3) ? d : -2 * d;
                    }
                    break;
                case 13:;
                    move = ((x - midcolumns + 4) + (y - midrows + 2) * 6) / 4;
                }
#ifdef _AI
            }
#endif
            if (!mult && plyr == 2 || inp == 13 && !mrk[move]) {
                plyr = (((mrk[move] = plyr) - 1) ^ !end(mrk)) + 1;
            }
            #ifdef _DEBUG
            _move = move;
            #endif
        }
        else {
            switch (inp) {
#ifdef _AI
            case 224:;
                mult ^= _getch() % 2;
                break;
#endif
#ifndef _AI
            case 224:;
                printf("\a");
                break;
#endif
            case 13:;
                memset(mrk, 0, 36);
                x = midcolumns;
                y = midrows;
                plyr = mult * (GetTickCount64() % 2) + 1;
            }
        }
        #ifdef _COLOR
        if (isxdigit(inp)) {
            printf("%X\33[D\33[48;5;%dm", inp % 39 - 9, inp % 39 - 9);
            int fgc = _getch();
            if (isxdigit(fgc) && inp != fgc) {
                #ifdef _DEBUG
                _fgc = fgc - 9;
                _bgc = inp - 9;
                #endif
                printf("\33[38;5;%dm", fgc % 39 - 9);
            }
        }
        #endif
    }
}