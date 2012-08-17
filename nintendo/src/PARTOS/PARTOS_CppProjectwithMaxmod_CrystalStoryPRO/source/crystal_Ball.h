/*
 * crystal_Ball.h
 *
 *  Created on: 2010. 12. 16.
 *      Author: ywyw
 */

#ifndef CRYSTAL_BALL_H_
#define CRYSTAL_BALL_H_

class crystal_Ball {
public:
	int x, y;
	int go_ball;
	crystal_Ball();
	virtual ~crystal_Ball();
	int move(int spcae_x, int water_cnt); //
	void destory();
	void ball_setting(int pal_num, int obj_num);

};


#endif /* CRYSTAL_BALL_H_ */
