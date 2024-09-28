#define _NO_CRT_STDIO_INLINE
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <windows.h>
#include <conio.h>

#define debug(...) {char args[128]; sprintf(args, __VA_ARGS__);  OutputDebugStringA(args);}

int end(int mrk[]) {
	int end = 1;
	for (int i = 0; i < 9; i++) {
		if (i < 3 && ((mrk[3 * i] & mrk[3 * i + 1] & mrk[3 * i + 2]) || (mrk[i] & mrk[i + 3] & mrk[i + 6]) || (mrk[0] & mrk[4] & mrk[8]) || (mrk[2] & mrk[4] & mrk[6]))) {
			return 2;
		}
		if (!mrk[i]) {
			end = 0;
		}
	}
	return end;
}

int minimax(int mrk[], int is_max) {
	int best = is_max ? -1 : 1;
	for (int i = 0; i < 9; i++) {
		if (end(mrk)) {
			return end(mrk) * best;
		}
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
	int mrk[9], mult = 1, plyr = -1, midcols = 0, midrows = 0, x = 0, y = 0;
	while (1) {
		CONSOLE_SCREEN_BUFFER_INFO info;
		GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);
		const int cols = info.dwSize.X + 2, rows = info.dwSize.Y + 2;
		x += cols / 2 - midcols;
		y += rows / 2 - midrows;
		midcols = cols / 2;
		midrows = rows / 2;
		printf("\33[2J\33[3J\33[%d;%dH\33(0", midrows - 3, midcols - 6);
		if (end(mrk) || plyr == -1) {
			printf("\33[B%s \33[13D\33[2B  [1]   [2]  \33[13D\33[2B  PLAYER(S)", plyr == -1 ? "SELECT MODE" : (end(mrk) == 2 ? (plyr ? "  O WINS!  " : "  X WINS!  ") : "    TIE!   "));
			x = midcols - (4 - 6 * mult);
			y = midrows;
		}
		else {
			static const char plyr_arr[3] = { ' ', 'O', 'X' };
			printf("lqqqwqqqwqqqk\33[13D\33[Bx %c x %c x %c x\33[13D\33[Btqqqnqqqnqqqu\33[13D\33[Bx %c x %c x %c x\33[13D\33[Btqqqnqqqnqqqu\33[13D\33[Bx %c x %c x %c x\33[13D\33[Bmqqqvqqqvqqqj", plyr_arr[mrk[0]], plyr_arr[mrk[1]], plyr_arr[mrk[2]], plyr_arr[mrk[3]], plyr_arr[mrk[4]], plyr_arr[mrk[5]], plyr_arr[mrk[6]], plyr_arr[mrk[7]], plyr_arr[mrk[8]]);
		}
		#ifdef _DEBUG
		debug("\nMRK:  %d%d%d%d%d%d%d%d%d\nPLYR: %d\nEND:  %d\nMULT: %d\nCOLS: %d\nROWS: %d\nX:    %d\nY:    %d\n", mrk[0], mrk[1], mrk[2], mrk[3], mrk[4], mrk[5], mrk[6], mrk[7], mrk[8], plyr, end(mrk), mult, cols, rows, x, y);
		#endif
		printf("\33[%d;%dH", y, x);
		if (end(mrk) || plyr == -1) {
			switch (_getch()) {
			case 224:
				mult ^= 1;
				break;
			case 13:
				for (int i = 0; i < 9; i++) {
					mrk[i] = 0;
				}
				x = midcols;
				plyr = 0;
			}
		}
		else if (mult || !plyr) {
			switch (_getch()) {
			case 224:;
				int d = _getch() - 76;
				if (d % 2) {
					d *= 4;
					x += (x - midcols + d) / 5 ? -2 * d : d;
				}
				else {
					d /= 2;
					y += (y - midrows + d) / 4 ? -2 * d : d;
				}
				break;
			case 13:;
				const int move = ((x - midcols + 4) + (y - midrows + 2) * 6) / 4;
				if (!mrk[move]) {
					mrk[move] = plyr + 1;
					plyr ^= 1;
				}
			}
		}
		else {
			int move, best = -1;
			for (int i = 0; i < 9; i++) {
				if (!mrk[i]) {
					mrk[i] = 2;
					const int score = minimax(mrk, 0);
					mrk[i] = 0;
					if (score >= best) {
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