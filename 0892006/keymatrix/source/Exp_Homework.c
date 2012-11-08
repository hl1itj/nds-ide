#include "FreeRTOS.h"
#include "task.h"
#include <nds.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "sevencore_io.h"

#define MAX_SIZE 8
#define INIT 99
u8 temp[MAX_SIZE] = { 0x70, 0x60, 0x50, 0x40, 0x30, 0x20, 0x10, 0x00 };
#define TRUE 1
#define FALSE 0
void Exp_4_Homework_A(void) {
    u8 key, scan = 0;
    int i, j, z;
    u8 led_state[MAX_SIZE];
    int chk;
    int key_pressed;

    key_pressed = FALSE;
    chk = TRUE;

    for (i = 0; i < MAX_SIZE; i++)
        led_state[i] = INIT;

    for (i = 0; i < NUM_7SEG_LED; i++)                    // Turn Off All
        writeb_virtual_io(SEG7LED, 0x80 + (i << 4));

    while (1) {

        if (key_pressed == FALSE && readb_virtual_io(PUSH_SW) == VIRTUAL_SW_1) {
            if (chk == TRUE)
                chk = FALSE;
            else
                chk = TRUE;

            for (i = 0; i < NUM_7SEG_LED; i++) {
                writeb_virtual_io(SEG7LED, 0x80 + (i << 4)); //PUSH_SW가 SW1이 눌리면 화면 초기
                led_state[i] = INIT;
            }

            key_pressed = FALSE;

        }

        if (key_pressed == FALSE) {
            writeb_virtual_io(KEY_MATRIX, 0x80 >> scan);
            key = scan * 4;
            switch (readb_virtual_io(KEY_MATRIX)) {
            case 8:
                key += 1;
                break;
            case 4:
                key += 2;

                break;
            case 2:
                key += 3;

                break;
            case 1:
                key += 4;
                if (key == 16)
                    key = 0;

                break;
            default:
                key = 255;

                break;
            }
            scan++;
            if (scan == 4)
                scan = 0;

            if (key < 16) {

                if (chk == FALSE) {

                    for (z = 0; z < MAX_SIZE - 1; z++) {
                        led_state[z] = led_state[z + 1];
                    }
                    led_state[7] = key;

                    for (j = 0; j < MAX_SIZE; j++) {
                        if (led_state[j] != INIT)
                            writeb_virtual_io(SEG7LED, temp[j] + led_state[j]);
                    }
                } else {

                    for (z = MAX_SIZE - 1; z > 0; z--) {
                        led_state[z] = led_state[z - 1];
                    }
                    led_state[0] = key;

                    for (j = MAX_SIZE - 1; j >= 0; j--) {
                        if (led_state[j] != INIT)
                            writeb_virtual_io(SEG7LED, temp[j] + led_state[j]);
                    }
                }

                key_pressed = TRUE;
            }
        }
        if (key_pressed == TRUE && readb_virtual_io(PUSH_SW) != VIRTUAL_SW_1
                && readb_virtual_io(KEY_MATRIX) == 0) {
            key_pressed = FALSE;
        }

        if (NDS_SWITCH() & KEY_START)
            break;
        vTaskDelay(MSEC2TICK(20) );
    }
    while (NDS_SWITCH() & KEY_START)
        vTaskDelay(MSEC2TICK(10) );        // Wait while START KEY is being pressed
}

