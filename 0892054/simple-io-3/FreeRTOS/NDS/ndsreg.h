#ifndef __NDS_REGISTERS_H__
#define __NDS_REGISTERS_H__

#define INT_MASTER_ENABLE  (*(volatile unsigned short *)0x4000208)
#define INT_ENABLE         (*(volatile unsigned long *)0x4000210)
#define IRQ_FLAGS          (*(volatile unsigned long *)0x4000214)
#define IRQ_VECTOR 				(*(volatile unsigned long *)(0xb003ffc))
#define VBLANK_INTR_WAIT_FLAGS 	(*(volatile unsigned long *)(0xb003ff8))

#define INT_VBLANK		 	(1 << 0)
#define INT_VCOUNT			(1 << 2)
#define INT_TIMER0         	(1 << 3)
#define INT_TIMER1        	(1 << 4)
#define INT_TIMER2    	    (1 << 5)
#define INT_TIMER3      	(1 << 6)
#define INT_KEYPAD        	(1 << 12)
#define INT_IPC_SYNC		(1 << 16)
#define INT_IPC_SEND_FIFO_EMPTY (1<< 17)
#define INT_IPC_RECV_FIFO_NOT_EMPTY (1<< 18)

#define KEYINPUT			(*(volatile unsigned short *)0x4000130)
#define KEYCNT				(*(volatile unsigned short *)0x4000132)
#define BUTTON_A			(1 << 0)
#define BUTTON_B			(1 << 1)
#define BUTTON_SELECT		(1 << 2)
#define BUTTON_START		(1 << 3)
#define BUTTON_RIGHT		(1 << 4)
#define BUTTON_LEFT			(1 << 5)
#define BUTTON_UP			(1 << 6)
#define BUTTON_DOWN			(1 << 7)
#define BUTTON_R			(1 << 8)
#define BUTTON_L			(1 << 9)
#define BUTTON_IRQ			(1 << 14)
#define BUTTON_CON			(1 << 15)
#define BUTTON_ALL			(BUTTON_A | BUTTON_B | BUTTON_SELECT | BUTTON_START | BUTTON_RIGHT | BUTTON_LEFT | BUTTON_UP | BUTTON_DOWN | BUTTON_R | BUTTON_L)

#define TM0CNT_L           (*(volatile unsigned short *)0x4000100)
#define TM1CNT_L           (*(volatile unsigned short *)0x4000104)
#define TM2CNT_L           (*(volatile unsigned short *)0x4000108)
#define TM3CNT_L           (*(volatile unsigned short *)0x400010c)

#define TM0CNT_H           (*(volatile unsigned short *)0x4000102)
#define TM1CNT_H           (*(volatile unsigned short *)0x4000106)
#define TM2CNT_H           (*(volatile unsigned short *)0x400010a)
#define TM3CNT_H           (*(volatile unsigned short *)0x400010e)

#define TIMER_FREQ(n)    (-0x2000000/(n)) 
#define TIMER_FREQ_64(n)  (-(0x2000000>>6)/(n))
#define TIMER_FREQ_256(n) (-(0x2000000>>8)/(n))
#define TIMER_FREQ_1024(n) (-(0x2000000>>10)/(n))

#define TM_PRESCALER_1     0
#define TM_PRESCALER_64    1
#define TM_PRESCALER_256   2
#define TM_PRESCALER_1024  3
#define TM_NORMAL_COUNT    0 
#define TM_SEE_BELOW_COUNT (1 << 2)
#define TM_IRQ_ENABLE      (1 << 6)
#define TM_TIMER_START     (1 << 7)

#define MAXIRQHANDLER 32



#endif
