/*
 * Main.c
 *
 *  Created on: 2011. 7. 24.
 *      Author: Jaewon Lee
 */
//#include <string.h>

#include "Common.h"
#include "dallshell.h"
#include "FileManage.h"
#include "Graphic.h"
//#include "Download.h"
//#include "DownConf.h"
//#include "Graphic.h"
//#include "KeyControl.h"

static portTASK_FUNCTION( testTask1, pvParameters );
static portTASK_FUNCTION( testTask2, pvParameters );

int device_channel = 0;

xTaskHandle xShellTaskHandle;
xTaskHandle xDownloadTaskHandle;

int main(void)
{
	xTaskCreate( testTask1,
					     (const signed char * const)"testTask1",
					     2048,
					     (  void * ) NULL,
					     tskIDLE_PRIORITY + 3,
					     &xShellTaskHandle);

	xTaskCreate( testTask2,
						 (const signed char * const)"testTask2",
						 2048,
						 (  void * ) NULL,
						 tskIDLE_PRIORITY + 2,
						 &xDownloadTaskHandle);

	//vTaskSuspend(xDownloadTaskHandle);

	vTaskStartScheduler();
	while(1)
		;

	return 0;
}

// File Processing Task
static portTASK_FUNCTION( testTask1, pvParameters )
{
	PA_Init();
	PA_InitVBL();
	/***************/
	//consoleDemoInit();
	fatInitDefault(); //Initialise fat library

	PA_LoadDefaultText(1, 2);
	PA_LoadDefaultText(0, 2);

	// Image Processing
	loadBackground();
	loadSprite();
	createSprite();

	clearlist(); // 파일리스트 초기화
	fileInit();
	fileListing(true); // 파일리스트 출력
	updatefiles();

	while(1)
	{
		// Download module testing...
		if(Pad.Newpress.Start)
		{
			//vTaskSuspend(xShellTaskHandle);
			vTaskPrioritySet(xDownloadTaskHandle, tskIDLE_PRIORITY + 4 );
			vTaskResume(xDownloadTaskHandle);
		}
		else
		{
			checkitems();
			checkFileMenu();
			device_channel = checkDownloadDevice();
		}
		PA_WaitForVBL();
	}
}

// Download Task
static portTASK_FUNCTION( testTask2, pvParameters )
{
	//vTaskDelete(xShellTaskHandle);
	vTaskSuspend(xShellTaskHandle);
	while(1)
	{
		vTaskSuspend(xShellTaskHandle);
		down_execute(device_channel);
		vTaskPrioritySet(xDownloadTaskHandle, tskIDLE_PRIORITY + 2 );
		vTaskResume(xShellTaskHandle);
		vTaskSuspend(xDownloadTaskHandle);
	}
}
