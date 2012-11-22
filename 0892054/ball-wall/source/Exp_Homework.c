// Free RTOS Headers
// See API reference Document in http://www.freertos.org/

#include <FreeRTOS.h>
#include <task.h>
#include <queue.h>
#include <semphr.h>

// NDS / Sevencore Board Headers
#include <nds.h>
#include <sevencore_io.h>

#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define COLOR_RED       RGB(31,  0,  0) /* Bright Red  	*/
#define COLOR_WHITE     RGB(31, 31, 31) /* Bright White */
#define COLOR_BLACK     RGB( 0,  0,  0)	/* Black : Zero	*/

#define BOX_WIDTH	8
#define BOX_HEIGHT	8
#define BOX_Y_POS	13
#define BOX_X_MAX	(SCREEN_WIDTH / BOX_WIDTH)

// 필요한 global 선언

void
draw_my_box(int pos_x, int pos_y, u16 color)
{
	int i, j;
    u32 *basePoint, pixel;

    pixel = (color << 16) + color;
    for (i = 0; i < BOX_HEIGHT; i++) {
 	    basePoint = (u32 *)BG_GFX +
 	      ((((pos_y * BOX_HEIGHT) + i) * SCREEN_WIDTH) + pos_x * BOX_WIDTH) / 2;
    	for (j = 0; j < (BOX_WIDTH / 2); j++)
    		*basePoint++ = pixel;
	}
}

void
draw_my_wall(int pos_x, int pos_y, u16 color)
{
	// draw big white box
}

extern xTaskHandle BallTask;

void
Exp_8_Homework_A(void)
{
    while (1) {
    	// 키 매트릭스에서 지정한 위치에 white box
    	// START key가 눌리면 종료
    }
	while (NDS_SWITCH() & KEY_START)
		vTaskDelay(10);		// Wait while START KEY is being pressed
}

void
Exp_8_Homework_B(void)
{
    // 각종 선언 및 초기화 (세마포 포함)
    vTaskResume(BallTask);

	while (1) {
		// key = getkey();
		// 키 매트릭스의 입력 만큼 큰 흰 박스로 벽을 그림
		// 박스의 움직임과 겹치지 않도록
	}
}

portTASK_FUNCTION(Ball_Task, pvParameters)
{
	// 필요한 선언, 초기화

	while (1) {
		// 2초를 주기로 작은 빨간 Box를 좌우로 움직임
		// 벽을 통과하지 않도록 주의
	}
}

// Key Matrix Scanning Task

void
key_init(void)
{
	int i;
	u8 key;

	for (i = 0; i < MAX_KEY_LOG; i++)
		xQueueReceive(KeyQueue, &key, 0);
}

int
kbhit(void)
{
	u8 key;
	int ret = xQueuePeek(KeyQueue, &key, 0);
	return (ret == pdPASS);
}
u8
getkey(void)
{
	u8 key;
	xQueueReceive(KeyQueue, &key, portMAX_DELAY);
	return key;
}

portTASK_FUNCTION(Key_Task, pvParameters)
{
	// Key Task
}
