#include<stdio.h>
#include<string.h>
#include<curses.h>
#include"set_ticker.h"
#include"bounce2d.h"
#include<unistd.h>

struct ppball the_ball ;
struct userPaddle paddle;

int set_welcome();
void set_up();
void end_up();
void move_paddle(int dir);
void GameOver();
int score = 0;

int main()
{


	char c;
	set_up();
	
	while(1)
	{
		c = getchar();
		if( c == 'Q')
			return 0;
		if( c == 'f' && the_ball.x_ttm>0 && the_ball.y_ttm > 0 )
		{
			the_ball.x_ttm --;

			the_ball.y_ttm --;
		}
		/*if( c == 's')
		{
			the_ball.x_ttm ++;
			the_ball.y_ttm ++;
		}*/
		if( c == 'w')
		{
			move_paddle(-1);
		}
		if( c == 's')
		{
			move_paddle(1);
		}
			
	}
	end_up();

	return 0;
}

void set_up()
{
	int setting = -1;
	void ball_move(int signum);
	void set_block();
	void set_paddle();
	the_ball.x_pos = X_POS;
	the_ball.y_pos = Y_POS;
	the_ball.x_ttg = the_ball.x_ttm = X_TTM;
	the_ball.y_ttg = the_ball.y_ttm = Y_TTM;
	the_ball.x_dir = 1;
	the_ball.y_dir = 1;
	the_ball.symbol = MY_SYMBOL;
	initscr();
	noecho();
	crmode();
	
	setting = set_welcome();
	while(setting == -1)
		continue;
	clear();
	refresh();

	set_block();//设置边框
	set_paddle();//设置接球挡板
	signal(SIGINT,SIG_IGN);
	mvaddch(the_ball.y_pos,the_ball.x_pos,the_ball.symbol);
	the_ball.lastx_pos = the_ball.x_pos;
	the_ball.lasty_pos = the_ball.y_pos;
	refresh();
	signal(SIGALRM,ball_move);
	if(setting == 0)
		set_ticker(TICK_TIME * 2);
	if(setting == 1)
		set_ticker(TICK_TIME);
	if(setting == 2)
		set_ticker(TICK_TIME /2);
}

int set_welcome()
{
	move((BOT_ROW-TOP_ROW)/2,(RIGHT_EDGE-LEFT_EDGE)/2);
	standout();
	addstr("welcome to our GAME!");
	standend();
	
	move((BOT_ROW-TOP_ROW)/2+1,(RIGHT_EDGE-LEFT_EDGE)/2);
	addstr("you can choose three modes: E for easy,N for normal,H for hard");
	refresh();
	char c = getchar();
	if(c == 'E')
	{
		
		return 0;
	}
	if(c == 'N')
	{
		
		return 1;

	}
	if( c == 'H')
	{
		
		return 2;
	}

}
void set_block()
{
	move(TOP_ROW-1,LEFT_EDGE-1);
	addstr("*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*");
	//hline('*',10);
	move(TOP_ROW,LEFT_EDGE-1);
	vline('*',BOT_ROW-TOP_ROW+1);
	move(BOT_ROW+1,LEFT_EDGE-1);
	//hline('*',RIGHT_EDGE-LEFT_EDGE);
        addstr("*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*");
	move((BOT_ROW-TOP_ROW)/2,RIGHT_EDGE+10);
	addstr("tips: w up s down");
}
void set_paddle()
{
	paddle.py_pos = (BOT_ROW-TOP_ROW)/2;
	paddle.px_pos = RIGHT_EDGE+1;
	move(paddle.py_pos,paddle.px_pos);
	vline('*',PADDLE_LENGTH);
	
}
void move_paddle(int dir)
{
	paddle.lastpx_pos = paddle.px_pos;
	paddle.lastpy_pos = paddle.py_pos;
	
	if((dir < 0 && paddle.lastpy_pos + dir < TOP_ROW)||(dir > 0 && paddle.lastpy_pos + dir > BOT_ROW)) //挡板不能再移动了
	{
		return;
	}else
	{
		move(paddle.lastpy_pos,paddle.lastpx_pos); //擦除旧挡板
		vline(' ',PADDLE_LENGTH);
		paddle.py_pos += dir;
		move(paddle.py_pos,paddle.px_pos); //在新位置绘制挡板
		vline('*',PADDLE_LENGTH);	
	}

}
void ball_move(int signum)
{
	int curx_pos,cury_pos,moved,flag;
	signal(SIGALRM,ball_move);
	int bounce_or_lose(struct ppball* bp);
	curx_pos = the_ball.x_pos;
	cury_pos = the_ball.y_pos;
	moved = 0;
	flag = 0;
	if(the_ball.y_ttm > 0 && the_ball.y_ttg -- == 1) //初始时间为1，也就是y可以进行移动了
	{
		the_ball.y_pos += the_ball.y_dir;
		the_ball.y_ttg = the_ball.y_ttm;
		moved = 1;
		flag = 1;
	}
	if(the_ball.x_ttm > 0 && the_ball.x_ttg -- == 1)
	{
		the_ball.x_pos += the_ball.x_dir;
		the_ball.x_ttg = the_ball.x_ttm;
		moved = 1;
	}
	if(moved == 1)
	{
		//mvaddch(cury_pos,curx_pos,BLANK);
		//mvaddch(cury_pos,curx_pos,BLANK);
		if(flag == 1)
		{
			mvaddch(the_ball.lasty_pos,the_ball.lastx_pos,BLANK);
			//mvaddch(cury_pos,curx_pos,BLANK);
			mvaddch(the_ball.y_pos,the_ball.x_pos,the_ball.symbol);
			the_ball.lastx_pos = the_ball.x_pos;
			the_ball.lasty_pos = the_ball.y_pos;
			move(LINES-1,COLS-1);
			refresh();

		}	
		int result = bounce_or_lose(&the_ball);
		if(result == -1)
		{
			GameOver();
		}
		move(LINES-1,COLS-1);
		refresh();
	}
	/*
	if(the_ball.y_ttm > 0 && the_ball.y_ttg >1)
	{
		the_ball.y_ttg -= 1;
		the_ball.y_pos += the_ball.y_dir;
	}	
		
	if(the_ball.x_ttm > 0 && the_ball.x_ttg >1){
		the_ball.x_ttg -= 1;
		the_ball.x_pos += the_ball.x_dir;
	}
	if(the_ball.x_ttg == 1 && the_ball.y_ttg == 1)
	{
		the_ball.x_ttg = the_ball.x_ttm;
		the_ball.y_ttg = the_ball.y_ttm;
		moved = 1;
	}
	if(moved == 1)
	{
		mvaddch(cury_pos,curx_pos,BLANK);
		mvaddch(cury_pos,curx_pos,BLANK);
		mvaddch(the_ball.y_pos,the_ball.x_pos,the_ball.symbol);
		bounce_or_lose(&the_ball);
		move(LINES-1,COLS-1);
		refresh();
	}*/
	//以上注释为书中源代码

	signal(SIGALRM,ball_move);
}

int bounce_or_lose(struct ppball*bp)
{
	int res = 0;
	if(bp->x_pos == LEFT_EDGE)
	{
		bp->x_dir = 1;
		res = 1;
	}
	if((bp->x_pos == RIGHT_EDGE && bp->y_pos< paddle.py_pos) ||(bp->x_pos == RIGHT_EDGE && bp->y_pos > paddle.py_pos + PADDLE_LENGTH))
	{
		return -1;
	}
	if(bp->x_pos == RIGHT_EDGE)
	{
		bp->x_dir = -1;
		res = 1;
		score += 1;
	}
	if(bp->y_pos == TOP_ROW)
	{
		bp->y_dir = 1;
		res = 1;
	}else if(bp->y_pos == BOT_ROW)
	{
		bp->y_dir = -1;
		res = 1;
	}
	return res;
	
}
void end_up()
{
	set_ticker(0);
	endwin();
}
void GameOver()
{
	char s[1024];
	clear();
	move((BOT_ROW-TOP_ROW)/2,(RIGHT_EDGE-LEFT_EDGE)/2);
	standout();
	addstr("GameOver!");
	standend();
	sprintf(s,"%d",score);
	move((BOT_ROW-TOP_ROW)/2+3,(RIGHT_EDGE-LEFT_EDGE)/2);
	addstr("your score is:");
	move((BOT_ROW-TOP_ROW)/2+3,(RIGHT_EDGE-LEFT_EDGE)/2+15);
	addstr(s);
	move((BOT_ROW-TOP_ROW)/2+5,(RIGHT_EDGE-LEFT_EDGE)/2);
	addstr("press Q to quit!");
	refresh();
	set_ticker(0);
}
