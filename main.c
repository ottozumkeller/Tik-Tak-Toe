#define _NO_CRT_STDIO_INLINE // must be defined for crinkler to work

#include<stdio.h>
#include<windows.h>
#include<conio.h>

#define PLAYER1		'X' // player 1 mark (must be capital)
#define PLAYER2		'O' // player 2 mark (must be capital)
#define COLUMNS		19
#define ROWS		9

int end(char mrk[]) {
	int end = 1;
	for (int i = 0; i < 9; i++) {
		if (i < 3 && (((mrk[3 * i] & mrk[3 * i + 1] & mrk[3 * i + 2]) >= PLAYER2) || ((mrk[i] & mrk[i + 3] & mrk[i + 6]) >= PLAYER2) || ((mrk[0] & mrk[4] & mrk[8]) >= PLAYER2) || ((mrk[2] & mrk[4] & mrk[6]) >= PLAYER2))) {
			return 2;
		} else if (mrk[i] == ' ') {
			end = 0;
		}
	}
	return end;
}

void print(char mrk[], char plyr_char) {
	printf("\x1b[?25h\x1b[8;%d;%dt\x1b]2; \a\x1b[2J\x1b[%dH\x1b(0", ROWS, COLUMNS, (ROWS - 7) / 2 + 1);
	char args[7][14] = { "lqqqwqqqwqqqk", "", "tqqqnqqqnqqqu", "", "tqqqnqqqnqqqu", "", "mqqqvqqqvqqqj" };
	for (int i = 0; i < 7; i++) {
		int j = (i + 1) / 3;
		sprintf(args[2 * j + 1], "x %c x %c x %c x", mrk[3 * j], mrk[3 * j + 1], mrk[3 * j + 2]);
		if (end(mrk)) {
			sprintf(args[i], "\x1b[?25l");
			sprintf(args[2], end(mrk) == 2 ? "%c WINS!" : "TIE!", plyr_char);
			sprintf(args[4], " NEW GAME?\a");
		}
		printf("\x1b[%dG%s\n", (COLUMNS - strlen(args[i])) / 2 + 1, args[i]);
	}
}

void main() {
	srand(GetTickCount64());
	int plyr = rand() % 2, x = (COLUMNS + 1) / 2, y = (ROWS + 1) / 2;
	char mrk[9] = { ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' };
	SetWindowLongW(GetConsoleWindow(), GWL_STYLE, WS_VISIBLE | WS_CAPTION | WS_SYSMENU);
	system("mode 13,1");
	print(mrk, '\0');
	do {
		printf("\x1b[%d;%dH", y, x);
		switch (_getch()) {
			case 224:
			{
				int d = _getch() - 76;
				if (d % 2) {
					int nx = x + d * 4;
					if (nx > (COLUMNS - 13) / 2 && nx < (COLUMNS - (COLUMNS - 13) / 2)) {
						x = nx;
					}
				} else {
					int ny = y + d / 2;
					if (ny > (ROWS - 7) / 2 && ny < (ROWS - (ROWS - 7) / 2)) {
						y = ny;
					}
				}
			}
			break;
			case 13:
			{
				int pos = ((y - (ROWS - 7) / 2) / 3) * 3 + (x - (COLUMNS - 13) / 2) % 3;
				if (mrk[pos] == ' ') {
					print(mrk, mrk[pos] = PLAYER1 - !(plyr ^= !end(mrk)) * (PLAYER1 - PLAYER2));
				}
			}
			break;
		}
	} while (!end(mrk));
	_getch();
	main();
}