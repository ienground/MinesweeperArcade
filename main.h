#ifndef _MAIN_H_
#define _MAIN_H_

#include <stdio.h>
#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <time.h>

#define WIDTH 90
#define HEIGHT 35
#define MAP_WIDTH 140
#define MAP_HEIGHT 29
#define NOTHING	0
#define QUIT	't'
#define SPACE_BAR 'p'
#define LEFT_FLAG 'a'
#define TOP_FLAG 'w'
#define RIGHT_FLAG 'd'
#define BOTTOM_FLAG 'x'
#define LEFT_TOP_FLAG 'q'
#define RIGHT_TOP_FLAG 'e'
#define LEFT_BOTTOM_FLAG 'z'
#define RIGHT_BOTTOM_FLAG 'c'
#define NAMELEN 20


#define DIRECTION_RIGHT 0
#define DIRECTION_DOWN 1
#define DIRECTION_UP 2
#define DIRECTION_LEFT 3

// MENU
#define MENU_PLAY_EASY '1'
#define MENU_SCORE '2'
#define MENU_EXIT '3'

typedef struct __Node {
	char name[NAMELEN];
	int score;
	struct __Node *next;
} Node;

int num;
Node *head, *tail, *newNode;

int gameTime;
int gameOver = 0;
int win = 0;
int score;
int hp;
int charX, charY, charDirection;
int prevCharX, prevCharY;
int map[MAP_WIDTH][MAP_HEIGHT];
int map_flag[MAP_WIDTH][MAP_HEIGHT];
int map_unvisited[MAP_WIDTH][MAP_HEIGHT];
int offsetX = 0;
int flagCount = 0;
int totalBombCount = 0;
int visitedCount = 0;
int gamemode = 0; // 0 EASY 1 HARD

char menu();
void play();
void createUserInterface();
void timer();
int getCommand();
int processCommand(int command);
void drawBox(int y,int x, int height, int width);
void removeCharacter(int y, int x);
void drawCharacter(int y, int x, int direction);
void drawFlag(int y, int x);
void createMap();
void createScoreboard();
void scoreboard();
void writeScoreboard();
void newScore(int score);

#endif