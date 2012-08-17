/*
 * hero.h
 *
 *  Created on: 2010. 12. 17.
 *      Author: ywyw
 */

#ifndef HERO_H_
#define HERO_H_

class hero{
public:
	int x, y;
	int water_cnt;
public:
	hero();
	~hero();
	void move();
};


#endif /* HERO_H_ */
