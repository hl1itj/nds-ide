/*
 * crystal.h
 *
 *  Created on: 2010. 12. 16.
 *      Author: ywyw
 */

#ifndef CRYSTAL_H_
#define CRYSTAL_H_

class crystal {
public:
	int x ,y;
public:
	crystal();
	virtual ~crystal();
	void move(int pal_num, int obj_num);
	void destory();
};

#endif /* CRYSTAL_H_ */
