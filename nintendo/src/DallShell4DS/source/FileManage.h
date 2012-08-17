/*
 * FileManage.h
 *
 *  Created on: 2011. 7. 24.
 *      Author: Jaewon Lee
 */

#ifndef FILEMANAGE_H_
#define FILEMANAGE_H_

#include <nds/ndstypes.h>
#include <sys/dir.h>

struct dirent *pent; // dirent 구조체 변수 선언
struct stat st; // stat 구조체 변수 선언


typedef struct {
	char label[256]; // 파일 명
	s8 ext; //0=folder 1=unknown 2=music 3=image 4=text 5=game
	bool switched;
} items;

items list[256];// 파일리스트 갯수
int position, files; // 현재 위치, 파일 수
int number_of_files; // 총파일 갯수
char *file_tsize;
char cur_dir[1024]; // 현재 디렉토리
s8 alive; // 실행중 표시 구분 변수
int folders; // 폴더 구분 변수
bool sel_created; // 선택된 파일 표시
s8 selected; // 현재 위치 함수
DIR* pdir;
//DIR_ITER* dir;
int textheight;
int mode; // File Mode
int re;
int rv;


/******** File Manage Function *******/
void fileInit();
void fileListing(bool change);
void clearlist();
void createitem(char *filename, s8 ext);
void updatefiles ();
void getextension(int i);
void godown();
void goup();
void checkitems();
int checkDownloadDevice();
void clearitems(bool pad);
void gotolastdir();
void checkFileMenu();
void addFolder();
void deleteFolderAndFiles();
int deleteDirTree( const char *aDirPath) ;
void initFileAndDirectory();
void moveDirectoryAndFiles();

#endif /* FILEMANAGE_H_ */


