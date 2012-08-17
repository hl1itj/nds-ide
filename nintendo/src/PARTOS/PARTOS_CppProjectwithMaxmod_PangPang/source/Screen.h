/*
 * Screen.h
 *
 *  Created on: 2010. 11. 20.
 *      Author: 최진영, 박경호, 신동우
 */

#ifndef SCREEN_H_
#define SCREEN_H_

class Screen {
public:
	int stage;

public:
	Screen();
	virtual ~Screen();
	void Ready_Screen();
	void SCreen_Change();

};

#endif /* SCREEN_H_ */
