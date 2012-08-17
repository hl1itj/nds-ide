/*
 * princess.h
 *
 *  Created on: 2010. 12. 17.
 *      Author: ywyw
 */

#ifndef PRINCESS_H_
#define PRINCESS_H_

class princess{
public:
	int x;
	int y;
public:
	princess();
	virtual ~princess();
	void move(int move_cnt);
};

#endif /* PRINCESS_H_ */
