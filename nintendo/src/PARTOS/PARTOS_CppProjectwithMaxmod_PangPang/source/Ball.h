/*
 * Ball.h
 *
 *  Created on: 2010. 11. 20.
 *      Author: 최진영, 박경호, 신동우
 */

#ifndef BALL_H_
#define BALL_H_


typedef struct{
	int ball_x, ball_y; // position
	int vx, vy; // speed
}puckinfos;


class Ball {
public:
	int move_flag;
	int x,y;
	int size;
	puckinfos puck;

public:
	Ball();
	virtual ~Ball();
	void move();
	void setsize(int ball_size);
	void setimg(int bimg);
	void setlocate(int x, int y, int dx, int dy);
};

#endif /* BALL_H_ */
