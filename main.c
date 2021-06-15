#include "main.h"

static struct sigaction act, oact;

int main(void) {
    int exit = 0;

    // ncurses 초기화하기
    initscr();
    noecho();
    keypad(stdscr, TRUE);
    createScoreboard();

    createMap();

    while (!exit) {
        clear();
        switch (menu()) {
            case MENU_PLAY_EASY: {
                play();
                break;
            }

            case MENU_SCORE: {
                scoreboard();
                break;
            }

            case MENU_EXIT: {
                exit = 1;
                break;
            }

            default: {
                break;
            }
        }
    }

    endwin();
    system("clear");

    return 0;

}

char menu() {
    drawBox(0, 0, 4, 40);
    move(2, 11);
    attron(A_REVERSE);
    printw("MINESWEEPER ARCADE!");
    move(3, 14);
    printw("BY @ienground");
    attroff(A_REVERSE);
    move(6, 0);
    printw("1. Play\n");
	printw("2. Check scoreboard\n");
	printw("3. Exit\n");
	return wgetch(stdscr);
}

void play() {
    clear();

    gameTime = 0;
    act.sa_handler = timer;
    sigaction(SIGALRM, &act, &oact);

    int i, j;

    for (i = 0; i < MAP_WIDTH; i++) {
        for (j = 0; j < MAP_HEIGHT; j++) {
            map_flag[i][j] = 0;
            map_unvisited[i][j] = 0;
        }
    }

    for (i = 0; i < 6; i++) {
        for (j = 0; j < MAP_HEIGHT; j++) {
            map_unvisited[i][j] = 1;
            visitedCount++;
        }
    }

    createUserInterface();

    int command;

    do {
        alarm(1);
        command = getCommand();

        if (processCommand(command) == QUIT) {
            alarm(0);
            drawBox(30 / 2 - 1, WIDTH / 2 - 5, 1, 10);
			move(30 / 2, WIDTH / 2 - 4);
			printw("Good-bye!!");
			refresh();
			getch();

			return;
        }
    } while (!gameOver && !win);
    
    alarm(0);
	getch();
	drawBox(30 / 2 - 1, WIDTH / 2 - 5, 1, 10);
	move(30 / 2, WIDTH / 2 - 4);
    if (gameOver) {
        printw("GameOver!!");
    } else if (win) {
        printw("You Win!");
    }
	refresh();
    getch();
    newScore(score);

}

void timer() {
    gameTime++;
}

int getCommand() {
	int command = wgetch(stdscr);

	switch (command) {
		case KEY_UP:
			break;
		case KEY_DOWN:
			break;
		case KEY_LEFT:
			break;
		case KEY_RIGHT:
			break;
		case ' ': /* space key*/
			command = SPACE_BAR;
			break;
		case 't':
		case 'T':
			command = QUIT;
			break;
        case 'a':
        case 'A':
            command = LEFT_FLAG;
            break;
        case 'x':
        case 'X':
            command = BOTTOM_FLAG;
            break;
        case 'd':
        case 'D':
            command = RIGHT_FLAG;
            break;
        case 'w':
        case 'W':
            command = TOP_FLAG;
            break;
        case 'q':
        case 'Q':
            command = LEFT_TOP_FLAG;
            break;
        case 'e':
        case 'E':
            command = RIGHT_TOP_FLAG;
            break;
        case 'z':
        case 'Z':
            command = LEFT_BOTTOM_FLAG;
            break;
        case 'c':
        case 'C':
            command = RIGHT_BOTTOM_FLAG;
            break;
		default:
			command = NOTHING;
			break;
	}
	
	return command;
}

int processCommand(int command) {
	int ret = 1;

	switch (command) {
		case QUIT:
			ret = QUIT;
			break;
		case KEY_UP:
            if (charY - 1 > 0) {
                charDirection = DIRECTION_UP;
                prevCharX = charX;
                prevCharY = charY;
                removeCharacter(prevCharY, prevCharX);
                drawCharacter(--charY, charX, charDirection);
            }
			break; 	
		case KEY_DOWN:
            if (charY + 1 < HEIGHT - 5) {
                charDirection = DIRECTION_DOWN;
                prevCharX = charX;
                prevCharY = charY;
                removeCharacter(prevCharY, prevCharX);
                drawCharacter(++charY, charX, DIRECTION_DOWN);
            }
			break;
		case KEY_RIGHT:
            if (charX + 1 < WIDTH + 1) {
                charDirection = DIRECTION_RIGHT;
                prevCharX = charX;
                prevCharY = charY;
                removeCharacter(prevCharY, prevCharX);
                drawCharacter(charY, ++charX, DIRECTION_RIGHT);
            } else if (charX + offsetX < MAP_WIDTH) {
                offsetX++;
                drawMap();
                charDirection = DIRECTION_RIGHT;
                prevCharX = charX - 1;
                prevCharY = charY;
                removeCharacter(prevCharY, prevCharX);
                drawCharacter(charY, charX, DIRECTION_RIGHT);
            }
			break;
		case KEY_LEFT:
            if (charX - 1 > 0) {
                charDirection = DIRECTION_LEFT;
                prevCharX = charX;
                prevCharY = charY;
                removeCharacter(prevCharY, prevCharX);
                drawCharacter(charY, --charX, DIRECTION_LEFT);
            } else if (charX + offsetX > 1) {
                offsetX--;
                drawMap();
                charDirection = DIRECTION_LEFT;
                prevCharX = charX;
                prevCharY = charY;
                removeCharacter(prevCharY, prevCharX);
                drawCharacter(charY, charX, DIRECTION_LEFT);
            }
			break;
		case SPACE_BAR:
			// while ((drawFlag = CheckToMove(field, nextBlock[0], blockRotate, topBlock, blockX))) {
			// 	topBlock++;
			// }
			// blockY = topBlock - 1;
			break;
        case LEFT_FLAG:
            drawFlag(charY, charX - 1);
            break;
        case BOTTOM_FLAG:
            drawFlag(charY + 1, charX);
            break;
        case RIGHT_FLAG:
            drawFlag(charY, charX + 1);
            break;
        case TOP_FLAG:
            drawFlag(charY - 1, charX);
            break;
        case LEFT_TOP_FLAG:
            drawFlag(charY - 1, charX - 1);
            break;
        case RIGHT_TOP_FLAG:
            drawFlag(charY - 1, charX + 1);
            break;
        case LEFT_BOTTOM_FLAG:
            drawFlag(charY + 1, charX - 1);
            break;
        case RIGHT_BOTTOM_FLAG:
            drawFlag(charY + 1, charX + 1);
            break;
		default:
			break;
	}
		
	return ret;
}

void drawBox(int y, int x, int height, int width) {
	int i, j;

	move(y, x);
	addch(ACS_ULCORNER);
	for (i = 0; i < width; i++) {
		addch(ACS_HLINE);
	}

	addch(ACS_URCORNER);
	for (j = 0; j < height; j++) {
		move(y + j + 1, x);
		addch(ACS_VLINE);
		move(y + j + 1, x + width + 1);
		addch(ACS_VLINE);
	}

	move(y + j + 1, x);
	addch(ACS_LLCORNER);
	for (i = 0; i < width; i++) {
		addch(ACS_HLINE);
	}
	addch(ACS_LRCORNER);
}

void createUserInterface() {
    drawBox(0, 0, HEIGHT - 6, WIDTH);
    drawBox(HEIGHT - 4, 0, 4, WIDTH);

    score = 0;
    gameOver = 0;
    win = 0;
    gameTime = 0;
    hp = 3;
    offsetX = 0;
    flagCount = 0;
    totalBombCount = 0;
    visitedCount = 0;

    // character
    charX = 1;
    charY = 15;
    prevCharX = 1;
    prevCharY = 15;
    charDirection = DIRECTION_RIGHT;

    int i, j;

    move(HEIGHT - 3, 2);
    printw("HEART : ");
    
    for (i = 0; i < hp; i++) {
        printw("O");
    }

    for (i = 0; i < 3 - hp; i++) {
        printw("X");
    }

    move(HEIGHT, 2);
    printw("SCORE : %d", score);

    drawMap();

    drawCharacter(charY, charX, charDirection);


}

void removeCharacter(int y, int x) {
    move(y, x);

    if (map_flag[x + offsetX - 1][y - 1]) {
        attron(A_REVERSE);
        addch(ACS_LANTERN);
        attroff(A_REVERSE);
    } else if (map[x + offsetX - 1][y - 1] == 11) {
        printw("*");
    } else if (map[x + offsetX - 1][y - 1] != 10) {
        if (map[x + offsetX - 1][y - 1] != 0) {
            printw("%d", map[x + offsetX - 1][y - 1]);
        } else {
            printw(" ");
        }   
    }
}

void drawCharacter(int y, int x, int direction) {
    int i, j;

    if (map[x + offsetX - 1][y - 1] == 11 && !map_unvisited[x + offsetX - 1][y - 1]) { // 지뢰
        hp--;
        move(HEIGHT - 3, 10);
        for (i = 0; i < hp; i++) {
            printw("O");
        }

        for (i = 0; i < 3 - hp; i++) {
            printw("X");
        }
    }

    if (hp == 0) {
        gameOver = 1;
    }

    if (visitedCount + flagCount >= MAP_WIDTH * MAP_HEIGHT) {
        win = 1;
    }

    move(y, x);

    attron(A_REVERSE);
    switch (charDirection) {
        case DIRECTION_RIGHT: {
            printw(">");
            break;
        }

        case DIRECTION_DOWN: {
            printw("v");
            break;
        }

        case DIRECTION_LEFT: {
            printw("<");
            break;
        }

        case DIRECTION_UP: {
            printw("^");
            break;
        }
    }
    attroff(A_REVERSE);
    
    if (!map_unvisited[x + offsetX - 1][y - 1]) {
        score += 10;
        move(HEIGHT, 2);
        printw("SCORE : %d", score);

        if (!map_flag[x + offsetX - 1][y - 1]) { // 폭탄 있다고 표시한 곳
            visitedCount++;
        }
    }

    
    map_unvisited[x + offsetX - 1][y - 1] = 1;
}

void drawFlag(int y, int x) {
    move(y, x);
    if (x > 0 && x < WIDTH + 1 && y > 0 && y < MAP_HEIGHT + 1) {
        if (!map_flag[x + offsetX - 1][y - 1]) {
            attron(A_REVERSE);
            addch(ACS_LANTERN);
            attroff(A_REVERSE);
            map_flag[x + offsetX - 1][y - 1] = 1;
            if (map_unvisited[x + offsetX - 1][y - 1]) {
                visitedCount--;
            }
            flagCount++;
        } else {
            if (!map_unvisited[x + offsetX - 1][y - 1]) {
                addch(ACS_CKBOARD);
            } else if (map[x + offsetX - 1][y - 1] == 11) {
                printw("*");
            } else if (map[x + offsetX - 1][y - 1] != 0) {
                printw("%d", map[x + offsetX - 1][y - 1]);
            } else {
                printw(" ");
            }

            if (map_unvisited[x + offsetX - 1][y - 1]) {
                visitedCount++;
            }
            map_flag[x + offsetX - 1][y - 1] = 0;
            flagCount--;
        }
    }

    if (visitedCount + flagCount >= MAP_WIDTH * MAP_HEIGHT) {
        win = 1;
    }
}

void createMap() {
    int px = 0;
    int py = 15;

    srand(time(NULL));

    int i = 0, j;

    map[px][py] = 1;

    int pointX[70];
    int pointY[70];

    i = 0;

    pointY[0] = 15;

    while (i < 70) {
        int choiceX = rand() % 134 + 6;
        int choiceY = rand() % 30;
        int straight = rand() % 2;
        int isExist = 0;
        
        for (j = 0; j < i; j++) {
            if (pointX[j] == choiceX) {
                isExist = 1;
                break;
            }
        }

        if (!isExist) {
            if (i - 1 > 0) {
                if (straight) {
                    pointY[i] = pointY[i - 1];
                } else {
                    pointY[i] = choiceY;
                }
            }
            pointX[i++] = choiceX;
        }
    }

    for (i = 0; i < 70; i++) {
        for (j = 0; j < 70; j++) {
            if (pointX[i] < pointX[j]) {
                int temp = pointX[i];
                pointX[i] = pointX[j];
                pointX[j] = temp;
            }
        }
    }


    for (i = 0; i < MAP_HEIGHT; i++) {
        for (j = 0; j < MAP_WIDTH; j++) {
            map[j][i] = 0;
        }
    }

    for (i = 0; i < 70; i++) {
        map[pointX[i]][pointY[i]] = 10;
        
        if (i + 1 < 70) {
            for (j = pointX[i]; j <= pointX[i + 1]; j++) {
                map[j][pointY[i]] = 10;
            }

            int small = pointY[i];
            int big = pointY[i + 1];

            if (small > big) {
                small = pointY[i + 1];
                big = pointY[i];
            }

            for (j = small; j < big; j++) {
                map[pointX[i + 1]][j] = 10;
            }
        }
    }

    for (i = 6; i < MAP_WIDTH; i++) {
        for (j = 0; j < MAP_HEIGHT; j++) {
            if (map[i][j] != 1) {
                int bomb = rand() % 5;
                if (bomb == 0) {
                    totalBombCount++;
                    map[i][j] = 11;
                }
            }
        }
    }

    for (i = 0; i < 6; i++) {
        for (j = 0; j < MAP_HEIGHT; j++) {
            map_unvisited[i][j] = 1;
            visitedCount++;
        }
    }

    for (i = 0; i < MAP_WIDTH; i++) {
        for (j = 0; j < MAP_HEIGHT; j++) {
            int cnt = getBombCount(i, j);
            if (map[i][j] != 11) {
                map[i][j] = cnt;
            }
        }
    }
}

void drawMap() {
    // 지뢰는 map[i][j] = 11
    // 길은 10
    int i, j;
    for (j = 0; j < MAP_HEIGHT; j++) {
        for (i = 0; i < WIDTH; i++) {
            move(j + 1, i + 1);
            if (map_unvisited[i + offsetX][j]) {
                if (map[i + offsetX][j] == 11) {
                    printw("*");
                } else if (map[i + offsetX][j] != 0) {
                    printw("%d", map[i + offsetX][j]);
                } else {
                    printw(" ");
                }
            } else {
                addch(ACS_CKBOARD);
            }
        }
    }
}

int getBombCount(int x, int y) {
    int cnt = 0;

    if (x > 0) {
        if (y > 0) {
            if (map[x - 1][y - 1] == 11) {
                cnt++;
            }
        }

        if (map[x - 1][y] == 11) {
            cnt++;
        }

        if (y < MAP_HEIGHT - 1) {
            if (map[x - 1][y + 1] == 11) {
                cnt++;
            }
        }
    }

    if (x < MAP_WIDTH - 1) {
        if (y > 0) {
            if (map[x + 1][y - 1] == 11) {
                cnt++;
            }
        }

        if (map[x + 1][y] == 11) {
            cnt++;
        }

        if (y < MAP_HEIGHT - 1) {
            if (map[x + 1][y + 1] == 11) {
                cnt++;
            }
        }
    }

    if (y > 0) {
        if (map[x][y - 1] == 11) {
            cnt++;
        }
    }

    if (y < MAP_HEIGHT - 1) {
        if (map[x][y + 1] == 11) {
            cnt++;
        }
    }

    return cnt;
}

void createScoreboard() {
    FILE *fp = fopen("scoreboard.txt", "r");
    int i, j, score = 0;
    char name[NAMELEN];
    
    if (fp == NULL) return;

    fscanf(fp, "%d", &num);
	for (i = 0; i < num; i++) {
		fscanf(fp, "%s %d", name, &score);
		newNode = malloc(sizeof(Node));
		strcpy(newNode->name, name);
		newNode->score = score;

		if (i == 0) { // 헤드 노드 세팅
			head = newNode;
			newNode->next = NULL;
		} else {
			Node *p = head;

			if (head->score <= score) {
				newNode->next = head;
				head = newNode;
			} else if (i == 1) { // 두번째 노드 세팅
				head->next = newNode;
				newNode->next = NULL;
			} else {
				for (j = 0; j < i - 1; j++) { //0~ 현재 전까지
					if (p->next->score <= score) { // 현재 점수가 p 다음 노드의 점수보다 높으면 순서 바꾸기.
						newNode->next = p->next;
						p->next = newNode;
						break;
					} else if (j == i - 2) { // 바로 마지막 노드
						p->next->next = newNode;
						newNode->next = NULL;
						break;
					} else { // 기타 케이스는 계속 넘어가야 함
						p = p->next;
					}
				}
			}
		}
	}

	fclose(fp);
}

void scoreboard() {
	clear();
	move(0, 0);

	int x = 0, y = 0, count, rank;
	char name[100];

	printw("1. List scores from X to Y\n");
	printw("2. List scores by a specific name\n");
	printw("3. delete a specific rank\n");
    printw("T. Quit\n");

	while (TRUE) {
		char command = wgetch(stdscr);
		if (command == 'T' || command == 't') {
			break;
		}

		switch (command) {
			case '1': {
				x = 0, y = 0;
				echo();
				printw("X: ");
				scanw("%d", &x);
				printw("Y: ");
				scanw("%d", &y);
				noecho();

				if (x == 0) x = 1;
				if (y == 0 || y > num) y = num;

				printw("       name       |     score\n");
				printw("--------------------------------------------\n");

				if (x > y || x < 0 || y < 0 || y > num || head == NULL) {
                    if (head == NULL) {
                        printw("head null");
                    }
					printw("search failure: no rank in the list\n");
				} else {
					Node *p = head;
					
					for (count = 1; count <= y; count++) {
						if (count >= x) {
							printw("%-17s | %6d\n", p->name, p->score);
						}

						p = p->next; // 다음으로 이동
					}
				}
				break;
			}

			case '2': {
				echo();
				printw("Input the name: ");
				scanw("%s", name);
				noecho();

				Node *p = head;

				count = 0;

				while (p != NULL) {
					if (strcmp(p->name, name) == 0) {
						if (count == 0) {
							printw("       name       |     score\n");
							printw("--------------------------------------------\n");
						}
						printw("%-17s | %6d\n", p->name, p->score);
						count++;
					}

					p = p->next;
				}

				if (count == 0) {
					printw("search failure: no name in the list\n");
				}
				break;
			}

			case '3': {
				rank = 0;
				count = 0;

				echo();
				printw("Input the rank: ");
				scanw("%d", &rank);
				noecho();

				Node *p = head;

				if (rank > num || rank <= 0) {
					printw("search failure: the rank not in the list\n");
				} else {
					while (count < rank - 2) { // 이전까지 탐색
						p = p->next;
						count++;
					} // 1 2 3 -> 1 3, 1 

					Node *removeNode;
					if (rank == 1) {
						removeNode = head;
						head = head->next;
					} else {
						removeNode = p->next;
						p->next = removeNode->next;
					}

					free(removeNode);
					
					printw("result: the rank deleted\n");

					num--;
					writeScoreboard();
				}

				

				break;
			}
		}
	}
}

void writeScoreboard() {
	int tmp_num = 0, i;
	FILE *fp = fopen("scoreboard.txt", "r");

	if (fp == NULL) {
		fp = fopen("scoreboard.txt", "w");
		fprintf(fp, "%d\n", num);

		Node *p = head;
		for (i = 0; i < num ; i++) {
			fprintf(fp, "%s %d\n", p->name, p->score);
			p = p->next;
		}
		fclose(fp);
	} else if (fscanf(fp, "%d", &tmp_num) == EOF) { // 빈 파일
		fclose(fp);

		fp = fopen("scoreboard.txt", "w");
		fprintf(fp, "%d\n", num);

		Node *p = head;
		for (i = 0; i < num ; i++) {
			fprintf(fp, "%s %d\n", p->name, p->score);
			p = p->next;
		}
		fclose(fp);
	} else if (num == tmp_num) {
		fclose(fp);
	} else {
		fclose(fp);

		fp = fopen("scoreboard.txt", "w");
		fprintf(fp, "%d\n", num);

		Node *p = head;
		for (i = 0; i < num ; i++) {
			fprintf(fp, "%s %d\n", p->name, p->score);
			p = p->next;
		}
		fclose(fp);
	}
}

void newScore(int score) {
	clear();
	move(0, 0);

	char name[NAMELEN];
	int i;
	printw("your name: ");
	echo();
	scanw("%s", name);
	noecho();

	Node *p = head;
	newNode = malloc(sizeof(Node));
	newNode->score = score;
	strcpy(newNode->name, name);

	if (num == 0) { // 첫 데이터
		head = newNode;
	} else {
		if (head->score <= score) { // 최고점, 가장 최상위보다 점수가 높음.
			newNode->next = p->next;
			p->next = newNode;
		} else if (num == 1) { // 데이터 한 개
			head->next = newNode;
			newNode->next = NULL;
		} else {
			for (i = 0; i < num - 1; i++) {
				if (p->next->score <= score) {
					newNode->next = p->next;
					p->next = newNode;

					if (i == 0) {
						newNode = head;
					}
					break;
				} else if (i == num - 2) { // 마지막
					p->next->next = newNode;
					newNode->next = NULL;
				} else { // 이외
					p = p->next;
				}
			}
		}
	}
	num++;

	writeScoreboard();
}