/*
 * water.h
 *
 *  Created on: 2010. 12. 17.
 *      Author: ywyw
 */

#ifndef WATER_H_
#define WATER_H_

class water {
public:
	int num; // 물방울 넘버
	int enable; // 활성화 여부
	int x;
	int y;
public:
	water();
	virtual ~water();
	void move(int move_cnt);
	void water_setting(int pal_num, int obj_num);
};

#endif /* WATER_H_ */
