/*
 * monster.h
 *
 *  Created on: 2010. 12. 17.
 *      Author: ywyw
 */

#ifndef MONSTER_H_
#define MONSTER_H_

class monster{
public:
    int ID;
    int x, y;
    int hp;
    monster();
    virtual ~monster();
    void move(int move_cnt);
    void monster_setting(int pal_num, int obj_num, int mon_X);
};

#endif /* MONSTER_H_ */
