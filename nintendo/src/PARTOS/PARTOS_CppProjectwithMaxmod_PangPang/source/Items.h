/*
 * Items.h
 *
 *  Created on: 2010. 11. 20.
 *      Author: 최진영, 박경호, 신동우
 */

#ifndef ITEMS_H_
#define ITEMS_H_

class Items {
public:
	int x,y;
	int move_flag;
	int size;

public:
	Items();
	virtual ~Items();
	void change();
	void SetLocation(int ch_x, int ch_y);
	void move();
};

#endif /* ITEMS_H_ */
