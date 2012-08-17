/*
 * Shild.h
 *
 *  Created on: 2010. 11. 20.
 *      Author: 최진영, 박경호, 신동우
 */

#ifndef SHILD_H_
#define SHILD_H_

class Shild {
public:
	int x, y;

public:
	Shild();
	virtual ~Shild();
	void SetLocation(int ch_x, int ch_y);
	void move();
};

#endif /* SHILD_H_ */
