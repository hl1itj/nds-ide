/*
 * Character.h
 *
 *  Created on: 2010. 11. 20.
 *      Author: 최진영, 박경호, 신동우
 */


#ifndef CHARACTER_H_
#define CHARACTER_H_


class Character {
public:
	int x, y;	// 케릭터의 위치 좌표.
	int isShild;  // 쉴드가 있나?

public:
	Character();
	virtual ~Character();
	void move();
};

#endif /* CHARACTER_H_ */
