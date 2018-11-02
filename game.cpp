// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   game.cpp                                           :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: ysushkov <marvin@42.fr>                    +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2018/08/20 10:28:33 by ysushkov          #+#    #+#             //
//   Updated: 2018/10/06 17:43:55 by ysushkov         ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#include <iostream>
#include <ncurses.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

using namespace std;
bool 		gameOver;
const int	width = 20;
const int	height = 20;
int			x, y, fruitX, fruitY, score, speed, level;
int 		tailX[100], tailY[100];
int 		nTail;
enum 		eDirection
{
	STOP = 0, LEFT, RIGHT, UP, DOWN
};
eDirection	dir;

void Setup()
{
	gameOver = false;
	dir = STOP;
	x = width / 2 - 1;
	y = height / 2 - 1;
	fruitX = rand() % width;
	fruitY = rand() % height;
	score = 0;
	speed = 500;
	level = 1;
}

void Draw()
{
	for (int i = 0; i < width; i++)
		mvprintw(0, i, "#");
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			if (j == 0 || j == width - 1)
				mvprintw(i, j, "#");
			else if (i == y && j == x)
				mvprintw(y, x, "0");
			else if (i == fruitY && j == fruitX)
				mvprintw(fruitY, fruitX, "F");
			for (int k = 0; k < nTail; k++)
				if (tailX[k] == j && tailY[k] == i)
					mvprintw(tailY[k], tailX[k], "o");
		}
		cout << endl;
	}
	for (int i = 0; i < width; i++)
		mvprintw(height - 1, i, "#");
}

void Input()
{
	switch (getch())
	{
	case KEY_LEFT:
		dir = LEFT;
		break;
	case KEY_RIGHT:
		dir = RIGHT;
		break;
	case KEY_UP:
		dir = UP;
		break;
	case KEY_DOWN:
		dir = DOWN;
		break;
	}
}

void Logic()
{
	int prevX = tailX[0];
	int prevY = tailY[0];
	int prev2X, prev2Y;
	tailX[0] = x;
	tailY[0] = y;
	for(int i = 1; i < nTail; i++)
	{
		prev2X = tailX[i];
		prev2Y = tailY[i];
		tailX[i] = prevX;
		tailY[i] = prevY;
		prevX = prev2X;
		prevY = prev2Y;
	}
	switch (dir)
	{
		case LEFT:
			x--;
			break ;
		case RIGHT:
			x++;
			break ;
		case UP:
			y--;
			break;
		case DOWN:
			y++;
			break;
		default:
			gameOver = false;
	}
	if (x == 19)
		x = 1;
	else if (x == 0)
		x = 18;
	if (y == 19)
		y = 1;
	else if (y == 0)
		y = 18;
	for (int i = 0; i < nTail; i++)
	{
		if (tailX[i] == x && tailY[i] == y)
			gameOver = true;
	}
	if (x == fruitX && y == fruitY)
	{
		score += 10;
		nTail++;
		fruitX = rand() % width;
		fruitY = rand() % height;
		if (fruitX == 0)
			fruitX++;
		if (fruitX == 19)
			fruitX--;
		if (fruitY == 0)
			fruitY++;
		if (fruitY == 19)
			fruitY--;
		if (score % 100 == 0)
		{
			level++;
			speed -= 50;
			if (speed < 0)
				speed = 1;
		}
	}
}

int main(void)
{
	Setup();
	initscr();
	curs_set(FALSE);
	noecho();
	cbreak();
	keypad(stdscr, true);
	while (!gameOver)
	{
		nodelay(stdscr, true);
		usleep(27500);
		timeout(speed);
		clear();
		Draw();
		mvprintw(22, 0, "SCORE: %d", score);
		mvprintw(21, 0, "LEVEL: %d", level);
		Input();
		Logic();
		refresh();
	}
	endwin();
	return 0;
}
