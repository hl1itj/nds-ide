/*
 * Stage.h
 *
 *  Created on: 2010. 11. 20.
 *      Author: 최진영, 박경호, 신동우
 */

#ifndef STAGE_H_
#define STAGE_H_

#include "Lope.h"
#include "Character.h"
#include "Ball.h"
#include "Items.h"

class Stage {
public:
	Character character;
	Lope lope;
	Items item;

	Ball ball;
	Ball ball2;
	Ball ball3;
	Ball ball4;
	Ball ball5;
	Ball ball6;
	Ball ball7;
	Ball ball8;

	int isGameOver;
	int isItem;

public:
	Stage();
	virtual ~Stage();
	void Collision_Ball_Character();
	void Collision_Ball_Lope();
	void Collision_Character_Item();
	void Location_Update();
	void Image_Update();
};

#endif /* STAGE_H_ */
