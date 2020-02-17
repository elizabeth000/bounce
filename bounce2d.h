/*
 * 头文件。定义结构体，和常用的常量。
 */
 
#include<stdio.h>

#define MY_SYMBOL	'O'
#define BLANK		' '	
#define X_POS 10
#define Y_POS 10
#define TOP_ROW 2
#define LEFT_EDGE 2
#define RIGHT_EDGE 50
#define BOT_ROW 20
#define X_TTM 5
#define Y_TTM 8
#define TICK_TIME 20
#define PADDLE_LENGTH 5

struct ppball
{
	int x_pos;
	int y_pos;
	int x_ttg; //初始时间。也就是距离下一次移动还有多久
	int y_ttg;
	int x_ttm; //间隔时间。间隔多久发出信号使其移动
	int y_ttm;
	int x_dir;
	int y_dir;
	int lastx_pos;
	int lasty_pos;//上一次画线的位置
	char symbol;
};
struct userPaddle
{
	int lastpx_pos;
	int lastpy_pos;
	int px_pos;
	int py_pos;
};
