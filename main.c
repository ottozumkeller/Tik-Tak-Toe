#define _NO_CRT_STDIO_INLINE
#define _CRT_SECURE_NO_WARNINGS

#include <conio.h>
#include <stdio.h>
#include <windows.h>

int end(int mrk[])
{
    int end = 1;
    for (int i = 0; i < 9; i++)
    {
        if (i < 3 &&
            ((mrk[3 * i + 0] & mrk[3 * i + 1] & mrk[3 * i + 2]) || 
            (mrk[1 * i + 0] & mrk[1 * i + 3] & mrk[1 * i + 6]) ||
            (mrk[0] & mrk[4] & mrk[8]) || 
            (mrk[2] & mrk[4] & mrk[6])))
        {
            return 2;
        }
        if (mrk[i] == 0)
        {
            end = 0;
        }
    }
    return end;
}

int minimax(int mrk[], int is_max)
{
    int best = is_max ? -1 : 1;
    for (int i = 0; i < 9; i++)
    {
        if (end(mrk))
        {
            return end(mrk) * best;
        }
        if (mrk[i] == 0)
        {
            mrk[i] = is_max + 1;
            const int score = minimax(mrk, is_max == 0);
            mrk[i] = 0;
            best = is_max ? max(score, best) : min(score, best);
        }
    }
    return best;
}

void main()
{
    int mrk[9], mult = 1, plyr = -1, midcols = 0, midrows = 0, x = 0, y = 0;
    while (1)
    {
        CONSOLE_SCREEN_BUFFER_INFO info;
        GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);
        x += info.dwSize.X / 2 + 1 - midcols;
        y += info.dwSize.Y / 2 + 1 - midrows;
        midcols = info.dwSize.X / 2 + 1;
        midrows = info.dwSize.Y / 2 + 1;
        printf("\33[%d;%dH\33(0\33[J", midrows - 3, midcols - 6);
        if (end(mrk) || plyr == -1)
        {
            // menu screen
            x = midcols - (3 - 6 * mult);
            y = midrows;
            printf(
                "\33[B   %s\33[8D\33[2B[1]   [2]\33[8D\33[2BPLAYERS\33[%d;%dH",
                plyr < 0 ? " START " : (end(mrk) == 2 ? (plyr ? "O WINS!" : "X WINS!") : "  TIE  "),
                y,
                x
            );
            switch (_getch())
            {
            case 224:
                mult ^= 1;
                break;
            case 13:
                for (int i = 0; i < 9; i++)
                {
                    mrk[i] = 0;
                }
                x = midcols;
                plyr = 0;
            }
        }
        else
        {
            // game screen
            static const char plyr_arr[3] = {' ', 'O', 'X'};
            printf("lqqqwqqqwqqqk\33[13D\33[Bx %c x %c x %c x\33[13D\33[Btqqqnqqqnqqqu\33[13D\33[Bx %c x %c x %c "
                   "x\33[13D\33[Btqqqnqqqnqqqu\33[13D\33[Bx %c x %c x %c x\33[13D\33[Bmqqqvqqqvqqqj\33[%d;%dH",
                   plyr_arr[mrk[0]], plyr_arr[mrk[1]], plyr_arr[mrk[2]], plyr_arr[mrk[3]], plyr_arr[mrk[4]],
                   plyr_arr[mrk[5]], plyr_arr[mrk[6]], plyr_arr[mrk[7]], plyr_arr[mrk[8]], y, x);
            if (mult == 1 || plyr == 0)
            {
                switch (_getch())
                {
                case 224: {
                    int d = _getch() - 76;
                    if (d % 2)
                    {
                        if ((x - midcols + (d *= 4)) / 8 == 0) x += d;
                    }
                    else
                    {
                        if ((y - midrows + (d /= 2)) / 4 == 0) y += d;
                    }
                    break;
                }
                case 13: {
                    const int move = ((x - midcols + 4) + (y - midrows + 2) * 6) / 4;
                    if (mrk[move] == 0)
                    {
                        mrk[move] = plyr + 1;
                        plyr ^= 1;
                    }
                }
                }
            }
            else
            {
                int move, best = -1;
                for (int i = 0; i < 9; i++)
                {
                    if (mrk[i] == 0)
                    {
                        mrk[i] = 2;
                        const int score = minimax(mrk, 0);
                        mrk[i] = 0;
                        if (score >= best)
                        {
                            best = score;
                            move = i;
                        }
                    }
                }
                mrk[move] = plyr + 1;
                plyr ^= 1;
            }
        }
    }
}