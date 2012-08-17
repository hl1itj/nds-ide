/*
 * game.h
 *
 *  Created on: 2010. 12. 16.
 *      Author: ywyw
 */

#ifndef CRYSTAL_H_
#define CRYSTAL_H_



/************************************* Class Area ****************************************/

/* 게임 진행 관련 클래스 - 점수 및 다른 사항들에 대한 flag나 점수들..*/
class GameProcess {
public:
	int score;      // 게임 점수
	int oddmonster; // 남은 몬스터의 수
	int stage_num;  // 단계 카운트
	int crystals;

	GameProcess() {
		score = 0;
		oddmonster = 3;
		stage_num = 1;
		crystals = 0;
	}
};

/*********************************** Function Area **************************************/
/* 게임 진행 관련 함수 */
void init_singleplay(); // 싱글플레이어 게임  초기화
void singleplay_start(); // 싱글플레이어 게임 스타트!

#endif /* CRYSTAL_H_ */
