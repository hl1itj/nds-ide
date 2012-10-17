#ifndef GDBSTUB_H_
#define GDBSTUB_H_

#include <stdint.h>
#include "FreeRTOS.h"
#include "task.h"

typedef struct REGISTER{
	uint32_t gps[16];
	uint32_t f0[3];
	uint32_t f1[3];
	uint32_t f2[3];
	uint32_t f3[3];
	uint32_t f4[3];
	uint32_t f5[3];
	uint32_t f6[3];
	uint32_t f7[3];
	uint32_t fps;
	uint32_t ps;
} GDB_Registers;

enum regnames
{
	R0,	R1,	R2,	R3,	R4,	R5,	R6,	R7,
	R8,	R9,	R10, R11, R12, SP,	LR, PC,
	F0, F1, F2, F3, F4, F5, F6, F7,
	FPS, PS
};

typedef enum regnames regnames_t;

#define NUMREGS (16 + 8 + 2)

#define REGSIZE( _x_ ) (((_x_) < F0 || (_x_) >= FPS) ? 4 : 12)

typedef unsigned int target_register_t;

#define HAL_STUB_REGISTERS_SIZE \
 ((sizeof(GDB_Registers) + sizeof(target_register_t) - 1) / sizeof(target_register_t))

#define PS_N 0x80000000
#define PS_Z 0x40000000
#define PS_C 0x20000000
#define PS_V 0x10000000

enum target_signal
  {
    /* Used some places (e.g. stop_signal) to record the concept that
       there is no signal.  */
    TARGET_SIGNAL_0 = 0,
    TARGET_SIGNAL_FIRST = 0,
    TARGET_SIGNAL_HUP = 1,
    TARGET_SIGNAL_INT = 2,
    TARGET_SIGNAL_QUIT = 3,
    TARGET_SIGNAL_ILL = 4,
    TARGET_SIGNAL_TRAP = 5,
    TARGET_SIGNAL_ABRT = 6,
    TARGET_SIGNAL_EMT = 7,
    TARGET_SIGNAL_FPE = 8,
    TARGET_SIGNAL_KILL = 9,
    TARGET_SIGNAL_BUS = 10,
    TARGET_SIGNAL_SEGV = 11,
    TARGET_SIGNAL_SYS = 12,
    TARGET_SIGNAL_PIPE = 13,
    TARGET_SIGNAL_ALRM = 14,
    TARGET_SIGNAL_TERM = 15,
    TARGET_SIGNAL_URG = 16,
    TARGET_SIGNAL_STOP = 17,
    TARGET_SIGNAL_TSTP = 18,
    TARGET_SIGNAL_CONT = 19,
    TARGET_SIGNAL_CHLD = 20,
    TARGET_SIGNAL_TTIN = 21,
    TARGET_SIGNAL_TTOU = 22,
    TARGET_SIGNAL_IO = 23,
    TARGET_SIGNAL_XCPU = 24,
    TARGET_SIGNAL_XFSZ = 25,
    TARGET_SIGNAL_VTALRM = 26,
    TARGET_SIGNAL_PROF = 27,
    TARGET_SIGNAL_WINCH = 28,
    TARGET_SIGNAL_LOST = 29,
    TARGET_SIGNAL_USR1 = 30,
    TARGET_SIGNAL_USR2 = 31,
    TARGET_SIGNAL_PWR = 32,
    /* Similar to SIGIO.  Perhaps they should have the same number.  */
    TARGET_SIGNAL_POLL = 33,
    TARGET_SIGNAL_WIND = 34,
    TARGET_SIGNAL_PHONE = 35,
    TARGET_SIGNAL_WAITING = 36,
    TARGET_SIGNAL_LWP = 37,
    TARGET_SIGNAL_DANGER = 38,
    TARGET_SIGNAL_GRANT = 39,
    TARGET_SIGNAL_RETRACT = 40,
    TARGET_SIGNAL_MSG = 41,
    TARGET_SIGNAL_SOUND = 42,
    TARGET_SIGNAL_SAK = 43,
    TARGET_SIGNAL_PRIO = 44,
  };

enum stop_type {
  STOP_UNKNOWN,
  STOP_HOST_BREAK,
  STOP_STEP_BREAK,
  STOP_BREAKPOINT,
  STOP_WATCHPOINT,
  STOP_RWATCHPOINT,
  STOP_AWATCHPOINT
};


#ifdef __cplusplus
extern "C" {
#endif

// brief Initialises the debugger stub and the supplied comms interface.
static int memAddrCheck(uint8_t *addr);
static int hex(uint8_t ch);
static int hexToInt(uint8_t **ptr, uint32_t *intValue);
static unsigned char* mem2hex(uint8_t *mem, uint8_t *buf, uint32_t count);
static unsigned char* hex2mem(uint8_t *buf, uint8_t *mem, uint32_t count);
static unsigned char* getpacket();
static void putPacket(unsigned char *buffer);
static void indicateHaltedReason();
static int regOffset(regnames_t reg);
target_register_t getRegister(regnames_t reg);
void putRegister(regnames_t which, target_register_t value);
int getRegisterAsBytes(regnames_t which, char *value);
int putRegisterAsBytes(regnames_t which, char *value);
static void readRegisters();
static void writeRegisters(uint8_t *ptr);
static void readMemory(uint8_t *ptr);
static void writeMemory(uint8_t *ptr);
static void debugStub();
static uint32_t v5TSemantics(void);
static uint32_t insWillExecute(uint32_t ins);
static uint32_t RmShifted(uint32_t shift);
static uint32_t *targetIns(uint32_t *pc, uint32_t ins);
static uint32_t targetThumbIns(uint32_t pc, uint16_t ins);
static void singleStep();
static void clearStep();
static void jumpBack(uint32_t ret_addr);
static uint32_t computeReturnAddress();
int initDebug();
static void build_t_packet();
static void exceptionHandler();
static void firstHandler();
void waitInput();

#ifdef __cplusplus
};
#endif

#endif /* GDBSTUB_H_ */
