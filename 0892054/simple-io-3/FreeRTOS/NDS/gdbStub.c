#include <stdio.h>
#include <string.h>

#include "gdbStub.h"
#include "gdbStubAsm.h"
#include "card_spi.h"
#include "define.h"

// Signal number for an illegal instruction
#define SIGILL 4
// Signal number for a trap instruction
#define SIGTRAP 5

#define BUFMAX 2048

static uint8_t remcomInBuffer[BUFMAX];
static uint8_t remcomOutBuffer[BUFMAX];

static const char hexChars[] = "0123456789abcdef";

uint32_t regMem[16] = {0, };

target_register_t registers[NUMREGS];
target_register_t altRegisters[NUMREGS] ;  	// Thread or saved process state
target_register_t *pRegisters = registers;     // Pointer to current set of registers

static uint32_t savedPC = 0;
static uint32_t savedInstr = 0;
static uint16_t savedThumbInstr = 0;

#define IS_THUMB_ADDR(addr)		((addr) & 1)
#define MAKE_THUMB_ADDR(addr)	((addr) | 1)
#define UNMAKE_THUMB_ADDR(addr)	((addr) & ~1)

static uint32_t retAddr = 0;

// Checked by Jun
static int memAddrCheck(uint8_t *addr)
{
	/*
	// need to check Memory address(0x02000000) (Jun)
	if((uint32_t)addr >= 0x02000000)
	{
		return 1;
	}

	return 0;
	*/

	return true;
}

// Checked by Jun
static int hex(uint8_t ch)
{
	if ((ch >= 'a') && (ch <= 'f')) return (ch - 'a' + 10);
	if ((ch >= '0') && (ch <= '9'))	return (ch - '0');
	if ((ch >= 'A') && (ch <= 'F'))	return (ch - 'A' + 10);

	return -1;
}

// Checked by Jun
static int hexToInt(uint8_t **ptr, uint32_t *intValue)
{
	int numChars = 0;
	int hexValue;

	*intValue = 0;

	while(**ptr)
	{
		hexValue = hex(**ptr);
		if (hexValue < 0)
			break;

		*intValue = (*intValue << 4) | hexValue;
		numChars++;

		(*ptr)++;
	}

	return numChars;
}

// Checked by Jun
static unsigned char* mem2hex(uint8_t *mem, uint8_t *buf, uint32_t count)
{
	int i;
	unsigned char ch;

	for(i = 0; i < count; i++)
	{
		if(!memAddrCheck(mem))
		{
			return 0;
		}

		ch = *(mem++);
		*buf++ = hexChars[ch >> 4];
		*buf++ = hexChars[ch & 0x0f];
	}
	*buf = 0;

	return buf;
}

// Checked by Jun
static unsigned char* hex2mem(uint8_t *buf, uint8_t *mem, uint32_t count)
{
	int i;
	unsigned char ch;

	for(i = 0; i < count; i++)
	{
		if(!memAddrCheck(mem))
		{
			return 0;
		}

		ch = hex(*buf++) << 4;
		ch = ch + hex(*buf++);
		*(mem++) = ch;
	}

	return mem;
}

// Checked by Jun
static unsigned char* getpacket()
{
	unsigned char *buffer = &remcomInBuffer[0];
	unsigned char checksum;
	unsigned char xmitcsum;
	int count;
	uint8_t ch;

	while (1)
	{
		ch = '\0';

		while ( ch != '$')
		{
			ch = getDebugChar();
		}

		checksum = 0;
		xmitcsum = -1;
		count = 0;

		while (count < BUFMAX - 1)
		{
			ch = getDebugChar();
			if (ch == '#')
				break;
			checksum = checksum + ch;
			buffer[count] = ch;
			count = count + 1;
		}
		buffer[count] = 0;

		if (ch == '#')
		{
			ch = getDebugChar();
			xmitcsum = hex(ch) << 4;

			ch = getDebugChar();
			xmitcsum += hex(ch);

			if (checksum != xmitcsum)
			{
				putDebugChar('-');
			}
			else
			{
				putDebugChar('+');

				if (buffer[2] == ':')
				{
					putDebugChar(buffer[0]);
					putDebugChar(buffer[1]);

					return &buffer[3];
				}

				return &buffer[0];
			}
		}
	}

	return 0;
}

// Checked by Jun
static void putPacket(unsigned char *buffer)
{
	unsigned char checksum;
	int count;
	unsigned char ch;

	do
	{
		putDebugChar('$');
		count = 0;
		checksum = 0;
		while((ch = buffer[count]) != 0)
		{
			putDebugChar(ch);
			checksum += ch;
			count += 1;
		}
		putDebugChar('#');
		putDebugChar(hexChars[checksum >> 4]);
		putDebugChar(hexChars[checksum & 0x0f]);
	}while(getDebugChar() != '+');
}

// Checked by Jun
static void indicateHaltedReason()
{
	// Need to compute signal number)
	remcomOutBuffer[0] = 'S';
	remcomOutBuffer[1] = hexChars[SIGTRAP >> 4];
	remcomOutBuffer[2] = hexChars[SIGTRAP & 0x0F];
	remcomOutBuffer[3] = 0;
}

static int regOffset(regnames_t reg)
{
    int baseOffset;

    if(reg < F0)
    	return reg * 4;

    baseOffset = 16 * 4;

    if(reg < FPS)
    	return baseOffset + ((reg - F0) * 12);

    baseOffset += (8 * 12);

    if(reg <= PS)
    	return baseOffset + ((reg - FPS) * 4);

    return -1;  // Should never happen!
}

target_register_t getRegister(regnames_t reg)
{
    target_register_t val;
    int offset = regOffset(reg);

    val = -1;

    if(REGSIZE(reg) > sizeof(target_register_t) || offset == -1)
    	return -1;

    if(reg == PS)
       	return getSPSR();

    if(reg >= R0 && reg <= PC)
    	val = exceptionRegisters[offset / sizeof(target_register_t)];

//    val = pRegisters[offset / sizeof(target_register_t)];

    return val;
}

void putRegister(regnames_t which, target_register_t value)
{
    int offset = regOffset(which);

    if(REGSIZE(which) > sizeof(target_register_t) || offset == -1)
    	return;

    pRegisters[offset / sizeof(target_register_t)] = value;
}

int getRegisterAsBytes(regnames_t which, char *value)
{
    int offset = regOffset(which);

    if(offset != -1)
    {
    	memcpy(value, (char *)pRegisters + offset, REGSIZE(which));
    	return 1;
    }
    return 0;
}

int putRegisterAsBytes(regnames_t which, char *value)
{
    int offset = regOffset(which);

    if(offset != -1)
    {
    	memcpy((char *)pRegisters + offset, value, REGSIZE(which));
    	return 1;
    }
    return 0;
}

static void readRegisters()
{
	int i;
	uint8_t *ptr;
	uint32_t cpsr;
	uint32_t pc;
	ptr = &remcomOutBuffer[0];

	// Register R0 ~ R14(SP), PC, CPSR, not contains FPU
	for(i = 0; i < 15; i++)
	{
		mem2hex((uint8_t *)&exceptionRegisters[i], ptr, 4);
		ptr += 8;
	}

	// set the PC to the return addres value
	pc = retAddr;
	mem2hex((uint8_t *)&pc, ptr, 4);
	ptr += 8;

//	// floating point registers (8 x 96bit)
//	for(i = 0; i < 8; i++)
//	{
//		*ptr++ = '0';*ptr++ = '0';*ptr++ = '0';*ptr++ = '0';
//		*ptr++ = '0';*ptr++ = '0';*ptr++ = '0';*ptr++ = '0';
//		*ptr++ = '0';*ptr++ = '0';*ptr++ = '0';*ptr++ = '0';
//		*ptr++ = '0';*ptr++ = '0';*ptr++ = '0';*ptr++ = '0';
//		*ptr++ = '0';*ptr++ = '0';*ptr++ = '0';*ptr++ = '0';
//		*ptr++ = '0';*ptr++ = '0';*ptr++ = '0';*ptr++ = '0';
//	}
//
//	// floating point status register?
//	for(i = 0; i < 1; i++)
//	{
//		*ptr++ = '0';
//		*ptr++ = '1';
//		*ptr++ = '2';
//		*ptr++ = '3';
//		*ptr++ = '4';
//		*ptr++ = '5';
//		*ptr++ = '6';
//		*ptr++ = '7';
//	}

	// The CPSR
	cpsr = getSPSR();
	mem2hex((uint8_t *)&cpsr, ptr, 4);
	ptr += 8;
	*ptr = 0;
}

static void writeRegisters(uint8_t *ptr)
{
	int i;
	uint32_t pc;

	// Register R0 ~ R14(SP), PC, CPSR
	for(i = 0; i < 15; i++)
	{
		hex2mem(ptr, (uint8_t *)&exceptionRegisters[i], 4);
		ptr += 8;
	}
	// set the PC to the return addres value
	hex2mem(ptr, (uint8_t *)&pc, 4);
	ptr += 8;
	retAddr = pc;

//	// skip the floaing point registers and floating point status register
//	ptr += 8 * (96 / 8 * 2);
//	ptr += 8;
//
//	// CPSR
//	ptr += 8;

	strcpy((char *)remcomOutBuffer, "OK");
}


static void readMemory(uint8_t *ptr)
{
	uint32_t addr;
	uint32_t length;
	int error = 1;

	if(hexToInt(&ptr, &addr))
	{
		if(*ptr++ == ',')
		{
			if(hexToInt(&ptr, &length))
			{
				if(!mem2hex((uint8_t *)addr, remcomOutBuffer, length))
				{
					strcpy((char *)remcomOutBuffer, "E03");
					return;
				}
				error = 0;
			}
		}
	}

	if(error)
		strcpy((char *)remcomOutBuffer,"E01");
}

static void writeMemory(uint8_t *ptr)
{
	uint32_t addr;
	uint32_t length;
	int error = 1;

	if(hexToInt(&ptr, &addr))
	{
		if(*ptr++ == ',')
		{
			if(hexToInt(&ptr, &length))
			{
				if(*ptr++ == ':')
				{
					if(hex2mem(ptr, (uint8_t *)addr, length))
					{
						strcpy((char *)remcomOutBuffer, "OK");
					}
					else
					{
						strcpy((char *)remcomOutBuffer, "E03");
						return;
					}
					error = 0;
				}
			}
		}
	}

	if(error)
	{
		strcpy((char *)remcomOutBuffer, "E02");
	}
}

// Process the GDB messages.
static void debugStub()
{
	int returnNow = 0;
	int sendReply;

	uint8_t *ptr;

	clearStep();

//	iprintf("Stub Code Entered\n");

	while(!returnNow)
	{
		sendReply = 1;

		memset(remcomOutBuffer, 0, sizeof(remcomOutBuffer));

		ptr = getpacket();

//		iprintf("CMD:");
//		iprintf((char *)ptr);
//		iprintf("\n");

//		waitInput();

		switch(*ptr++)
		{
		// Indicate the reason the target halted
		case '?':
			indicateHaltedReason();
			break;

		// Read general registers
		case 'g':
			readRegisters();
			break;

		// Write general registers
		case 'G':	   /* set the value of the CPU registers - return OK */
			writeRegisters(ptr);
			break;

		// Read length  bytes of memory starting at address addr
		case 'm':
			readMemory(ptr);
			break;

		// Write length bytes of memory starting at address addr
		case 'M':
			writeMemory(ptr);
			break;

		case 'q':
			break;

		case 'Q':
			break;

		case 'H':
			strcpy((char *)remcomOutBuffer, "OK");
			break;

		case 'z':
		case 'Z':
			break;

		// Single step
		case 's':
			singleStep();
//			iprintf("single step end\n");
			returnNow = 1;
			sendReply = 0;
			break;

		// Continue
		//case 'C':
		case 'c':
			returnNow = 1;
			sendReply = 0;
			break;

		// Kill request
		case 'k':
			sendReply = 0;
			break;
		}

		if(sendReply)
		{
//			iprintf("REPLY: ");
//			iprintf( (char *)remcomOutBuffer);
//			iprintf("\n");

			putPacket(remcomOutBuffer);
		}
	}

//	iprintf("I-Cache Flush\n");
	ICInvalidateAll();
//	iprintf("D-Cache Flush\n");
	DCFlushAll();

//	iprintf("Stub complete\n");
}

static uint32_t v5TSemantics(void)
{
    uint32_t id;

    id = v5T_semantics_asm();

    return ((id >> 16) & 0xff) >= 5;
}

// 명령어를 실행할 지의 여부를 판단
static uint32_t insWillExecute(uint32_t ins)
{
    // uint32_t psr = get_register(PS);  // condition codes
	uint32_t psr = getSPSR();  // condition codes
    uint32_t res = 0;

    // State Flag
    switch((ins & 0xF0000000) >> 28)
    {
    // EQ : Z flag = Set State(1)이면 같다는 의미이므로 명령어가 실행 됨
    // 이와 같은 방식으로 각 조건 니모닉에 대해 CPSR의 상태 플래그를 비교
    case 0x0: // EQ
        res = ((psr & PS_Z) == PS_Z) ? TRUE : FALSE;
        break;
    case 0x1: // NE
    	res = ((psr & PS_Z) != PS_Z) ? TRUE : FALSE;
        break;
    case 0x2: // CS
    	res = ((psr & PS_C) == PS_C) ? TRUE : FALSE;
        break;
    case 0x3: // CC
    	res = ((psr & PS_C) != PS_C) ? TRUE : FALSE;
        break;
    case 0x4: // MI
    	res = ((psr & PS_N) == PS_N) ? TRUE : FALSE;
        break;
    case 0x5: // PL
    	res = ((psr & PS_N) != PS_N) ? TRUE : FALSE;
        break;
    case 0x6: // VS
    	res = ((psr & PS_V) == PS_V) ? TRUE : FALSE;
        break;
    case 0x7: // VC
    	res = ((psr & PS_V) != PS_V) ? TRUE : FALSE;
        break;
    case 0x8: // HI
    	res = (((psr & PS_C) == PS_C) && ((psr & PS_Z) != PS_Z)) ? TRUE : FALSE;
        break;
    case 0x9: // LS
    	res = (((psr & PS_C) != PS_C) || ((psr & PS_Z) == PS_Z)) ? TRUE : FALSE;
        break;
    case 0xA: // GE
        res = (((psr & (PS_N|PS_V)) == (PS_N|PS_V)) || ((psr & (PS_N|PS_V)) == 0)) ? TRUE : FALSE;
        break;
    case 0xB: // LT
        res = (((psr & (PS_N|PS_V)) == PS_N) || ((psr & (PS_N|PS_V)) == PS_V)) ? TRUE : FALSE;
        break;
    case 0xC: // GT
        res = ((((psr & (PS_N|PS_V)) == (PS_N|PS_V)) || ((psr & (PS_N|PS_V)) == 0))
        		&& ((psr & PS_Z) == 0)) ? TRUE : FALSE;
        break;
    case 0xD: // LE
        res = ((((psr & (PS_N|PS_V)) == PS_N) || ((psr & (PS_N|PS_V)) == PS_V))
        		|| ((psr & PS_Z) == PS_Z)) ? TRUE : FALSE;
        break;
    case 0xE: // AL
        res = TRUE;
        break;
    // NV 다시 보기
    case 0xF: // NV
    	if(((ins & 0x0E000000) >> 24) == 0xA)
    		res = TRUE;
    	else
    		res = FALSE;
        break;
    }
    return res;
}

static uint32_t RmShifted(uint32_t shift)
{
    uint32_t Rm = getRegister(shift & 0x00F);
    uint32_t shiftCount;
    if((shift & 0x010) == 0) {
    	shiftCount = (shift & 0xF80) >> 7;
    } else {
    	shiftCount = getRegister((shift & 0xF00) >> 8);
    }
    switch((shift & 0x060) >> 5) {
    case 0x0: // Logical left
        Rm <<= shiftCount;
        break;
    case 0x1: // Logical right
        Rm >>= shiftCount;
        break;
    case 0x2: // Arithmetic right
        Rm = (uint32_t)((uint32_t)Rm >> shiftCount);
        break;
    case 0x3: // Rotate right
        if(shiftCount == 0) {
            // Special case, RORx
            Rm >>= 1;
            if(getRegister(PS) & PS_C) Rm |= 0x80000000;
        } else {
            Rm = (Rm >> shiftCount) | (Rm << (32 - shiftCount));
        }
        break;
    }
    return Rm;
}

// Instruction을 Decode하여
// 해당 Instruction을 실행하였을 때 PC가 어떻게 변화되는지 체크해서
// GDB의 Step 명령을 통해 다음 번에 Breakpoint가 적용되어야 하는 주소를 계산한다.
static uint32_t *targetIns(uint32_t *pc, uint32_t ins)
{
    uint32_t newPC, offset, op2;
    uint32_t Rn;
    int i, reg_count, c;

    switch((ins & 0x0C000000) >> 26) {
    case 0x0:
        // BX or BLX
        if((ins & 0x0FFFFFD0) == 0x012FFF10) {
        	newPC = (uint32_t)getRegister(ins & 0x0000000F);
            return ((uint32_t *)newPC);
        }
        // Data processing
        newPC = (uint32_t)(pc + 1);
        if((ins & 0x0000F000) == 0x0000F000) {
            // Destination register is PC
            if ((ins & 0x0FBF0000) != 0x010F0000) {
                Rn = (uint32_t)getRegister((ins & 0x000F0000) >> 16);
                if ((ins & 0x000F0000) == 0x000F0000) Rn += 8;  // PC prefetch!
                if ((ins & 0x02000000) == 0) {
                    op2 = RmShifted(ins & 0x00000FFF);
                } else {
                    op2 = ins & 0x000000FF;
                    i = (ins & 0x00000F00) >> 8;  // Rotate count
                    op2 = (op2 >> (i*2)) | (op2 << (32-(i*2)));
                }
                switch ((ins & 0x01E00000) >> 21) {
                case 0x0: // AND
                	newPC = Rn & op2;
                    break;
                case 0x1: // EOR
                	newPC = Rn ^ op2;
                    break;
                case 0x2: // SUB
                	newPC = Rn - op2;
                    break;
                case 0x3: // RSB
                	newPC = op2 - Rn;
                    break;
                case 0x4: // ADD
                	newPC = Rn + op2;
                    break;
                case 0x5: // ADC
                    c = (getRegister(PS) & PS_C) != 0;
                    newPC = Rn + op2 + c;
                    break;
                case 0x6: // SBC
                    c = (getRegister(PS) & PS_C) != 0;
                    newPC = Rn - op2 + c - 1;
                    break;
                case 0x7: // RSC
                    c = (getRegister(PS) & PS_C) != 0;
                    newPC = op2 - Rn +c - 1;
                    break;
                case 0x8: // TST
                case 0x9: // TEQ
                case 0xA: // CMP
                case 0xB: // CMN
                    break; // PC doesn't change
                case 0xC: // ORR
                	newPC = Rn | op2;
                    break;
                case 0xD: // MOV
                	newPC = op2;
                    break;
                case 0xE: // BIC
                	newPC = Rn & ~op2;
                    break;
                case 0xF: // MVN
                	newPC = ~op2;
                    break;
                }
            }
        }
        return ((uint32_t *)newPC);
    case 0x1:
        if((ins & 0x02000010) == 0x02000010) {
            // Undefined!
            return (pc + 1);
        } else {
            if((ins & 0x00100000) == 0) {
                // STR
                return (pc + 1);
            } else {
                // LDR
                if((ins & 0x0000F000) != 0x0000F000) {
                    // Rd not PC
                    return (pc + 1);
                } else {
                    Rn = (uint32_t)getRegister((ins & 0x000F0000) >> 16);
                    if((ins & 0x000F0000) == 0x000F0000) Rn += 8;  // PC prefetch!
                    if(ins & 0x01000000) {
                        // Add/subtract offset before
                        if((ins & 0x02000000) == 0) {
                            // Immediate offset
                            if(ins & 0x00800000) {
                                // Add offset
                                Rn += (ins & 0x00000FFF);
                            } else {
                                // Subtract offset
                                Rn -= (ins & 0x00000FFF);
                            }
                        } else {
                            // Offset is in a register
                            if(ins & 0x00800000) {
                                // Add offset
                                Rn += RmShifted(ins & 0x00000FFF);
                            } else {
                                // Subtract offset
                                Rn -= RmShifted(ins & 0x00000FFF);
                            }
                        }
                    }
                    return ((uint32_t *)*(uint32_t *)Rn);
                }
            }
        }
        return (pc + 1);
    case 0x2:  // Branch, LDM/STM
    	if((ins & 0x02000000) == 0) {
    		// LDM/STM
    		if((ins & 0x00100000) == 0) {
    			// STM
    			return (pc + 1);
    		} else {
    			// LDM
    			if((ins & 0x00008000) == 0) {
    				// PC not in list
    				return (pc + 1);
    			} else {
    				Rn = (uint32_t)getRegister((ins & 0x000F0000) >> 16);
    				if((ins & 0x000F0000) == 0x000F0000) Rn += 8;  // PC prefetch!
    				offset = ins & 0x0000FFFF;
    				reg_count = 0;
    				for(i = 0;  i < 15;  i++) {
    					if(offset & (1<<i)) reg_count++;
    				}
    				if(ins & 0x00800000) {
    					// Add offset
    					Rn += reg_count * 4;
    				} else {
    					// Subtract offset
    					Rn -= 4;
    				}
    				return ((uint32_t *)*(uint32_t *)Rn);
    			}
    		}
    	} else {
    		// Branch
    		if(insWillExecute(ins)) {
    			offset = (ins & 0x00FFFFFF) << 2;
    			if(ins & 0x00800000) offset |= 0xFC000000;  // sign extend
    			newPC = (uint32_t)(pc + 2) + offset;
    			// If its BLX, make new_pc a thumb address.
    			if((ins & 0xFE000000) == 0xFA000000) {
    				if((ins & 0x01000000) == 0x01000000)
    					newPC |= 2;
    				newPC = MAKE_THUMB_ADDR(newPC);
    			}
    			return ((uint32_t *)newPC);
    		} else {
    			// Falls through
    			return (pc + 1);
    		}
    	}
    case 0x3:  // Coprocessor & SWI
    	if(((ins & 0x03000000) == 0x03000000) && insWillExecute(ins)) {
    		// SWI
    		// 이 부분 수정해야함
    		// return (unsigned long *)(CYGNUM_HAL_VECTOR_SOFTWARE_INTERRUPT * 4);
    		return (uint32_t *)(2 * 4);
    	} else {
    		return (pc + 1);
    	}
    default:
        // Never reached - but fixes compiler warning.
        return 0;
    }
}

// target_ins의 thumb mode 버전
static uint32_t targetThumbIns(uint32_t pc, uint16_t ins)
{
	uint32_t new_pc = MAKE_THUMB_ADDR(pc + 2); // default is fall-through
	// to next thumb instruction
	uint32_t offset, arm_ins, sp;
	int i;

	switch((ins & 0xf000) >> 12)
	{
	case 0x4:
		// Check for BX or BLX
		if((ins & 0xff07) == 0x4700)
			new_pc = (uint32_t)getRegister((ins & 0x00078) >> 3);
		break;
	case 0xb:
		// push/pop
		// Look for "pop {...,pc}"
		if((ins & 0xf00) == 0xd00)
		{
			// find PC
			sp = (uint32_t)getRegister(SP);

			for(offset = i = 0; i < 8; i++) {
				if(ins & (1 << i))
					offset += 4;
			}

			new_pc = *(uint32 *)(sp + offset);

			if(!v5TSemantics())
				new_pc = MAKE_THUMB_ADDR(new_pc);
		}
		break;
	case 0xd:
		// Bcc | SWI
		// Use ARM function to check condition
		arm_ins = ((uint32_t)(ins & 0x0f00)) << 20;
		if((arm_ins & 0xF0000000) == 0xF0000000) {
			// SWI
			// new_pc = CYGNUM_HAL_VECTOR_SOFTWARE_INTERRUPT * 4;
			new_pc = 8;

		} else if(insWillExecute(arm_ins)) {
			offset = (ins & 0x00FF) << 1;
			if(ins & 0x0080) offset |= 0xFFFFFE00;  // sign extend
			new_pc = MAKE_THUMB_ADDR((uint32_t)(pc + 4) + offset);
		}
		break;
	case 0xe:
		// check for B
		if((ins & 0x0800) == 0) {
			offset = (ins & 0x07FF) << 1;
			if(ins & 0x0400) offset |= 0xFFFFF800;  // sign extend
			new_pc = MAKE_THUMB_ADDR((uint32_t)(pc + 4) + offset);
		}
		break;
	case 0xf:
		// BL/BLX (4byte instruction!)
		// First instruction (bit 11 == 0) holds top-part of offset
		if((ins & 0x0800) == 0) {
			// iprintf("1111111\n");
			offset = (ins & 0x07FF) << 12;
			if(ins & 0x0400) offset |= 0xFF800000;  // sign extend
			// Get second instruction
			// Second instruction (bit 11 == 1) holds bottom-part of offset
			ins = *(uint16_t*)(pc + 2);

			// iprintf("second ins : %08X\n", ins);

			// Check for BL/BLX
			if((ins & 0xE800) == 0xE800) {
				offset |= (ins & 0x07ff) << 1;
				new_pc = (uint32_t)(pc + 4) + offset;
				// If its BLX, force a full word alignment
				// Otherwise, its a thumb address.
				if(!(ins & 0x1000))
					new_pc &= ~3;
				else
					new_pc = MAKE_THUMB_ADDR(new_pc);
			}
		}
		break;
	}

	return new_pc;
}

static void singleStep()
{
	uint32_t curins;
	uint32_t t_pc;
	uint32_t pc = exceptionRegisters[15];

//	iprintf("single step\n");

	if(getSPSR() & 0x20)	// Thumb
	{
//		iprintf("Thumb mode\n");

		savedPC = targetThumbIns(pc, *(uint16_t*)pc);
	}
	else	// ARM
	{
//		iprintf("ARM mode\n");

		curins = *(uint32_t*)pc;
		if(insWillExecute(curins))
		{
			savedPC = (uint32_t)targetIns((uint32_t*)pc, curins);
		}
		else
		{
			savedPC = pc + 4;
		}
	}

	if(IS_THUMB_ADDR(savedPC))	// Thumb instruction
	{
		t_pc = UNMAKE_THUMB_ADDR(savedPC);
		savedThumbInstr = *(uint16_t*)t_pc;

//		iprintf("addr : %08X, instr : %08X\n", t_pc, savedThumbInstr);

		*(uint16_t*)t_pc = 0xBEBE;
	}
	else	// ARM instruction
	{
		savedInstr = *(uint32_t*)savedPC;

//		iprintf("addr : %08X, instr : %08X\n", savedPC, savedInstr);

		*(uint32_t*)savedPC = 0xE7FFDEFE;
	}

//	waitInput();
}

static void clearStep()
{
	unsigned long t_pc;

	if(savedPC != 0) {
		// Restore instruction according to type
		if(IS_THUMB_ADDR(savedPC)) {
			// Thumb instruction
			t_pc = UNMAKE_THUMB_ADDR(savedPC);
			*(uint16_t*)t_pc = savedThumbInstr;
		} else {
			// ARM instruction
			*(uint32_t*)savedPC = savedInstr;
		}
		savedPC = 0;
	}
}

// Trying to get back, setting the values for the registers
static void jumpBack(uint32_t ret_addr)
{
	int i;

	/* r0-r12 and pc values are copied in the ldm instruction */
	for(i = 0; i < 13; i++)
	{
		regMem[i] = exceptionRegisters[i];
	}
	regMem[i] = ret_addr;

	// the banked registers, r13 and r14, are placed directly
	setBankedReg(exceptionRegisters[13], exceptionRegisters[14]);

//	iprintf("setBankedReg end\n");

	// switch processor mode and set the values
	setRegs(regMem);

//	iprintf("setReg end\n");
}

static uint32_t computeReturnAddress()
{
	uint32_t returnAddress;
	uint32_t currentMode = getCPSR() & 0x1f;
	uint32_t thumbState = getSPSR() & 0x20;

	// Data / Prefetch Abort
	if(currentMode == 0x17)
	{
		returnAddress = exceptionRegisters[PC] - 4;

		if((uint32_t)BreakPoint == returnAddress)
		{
			exceptionRegisters[PC] += 4;
			returnAddress += 4;
		}
	}
	// Undefined Instruction
	else
	{
		returnAddress = exceptionRegisters[PC] - 4;

		if(thumbState)
		{
			returnAddress += 2;
		}
	}

	return returnAddress;
}

// Initialize the GDB Stub.
int initDebug()
{
	setExceptionHandler(firstHandler);

	return true;
}

static void build_t_packet()
{
	uint8_t *ptr;
	uint32_t currentMode = getCPSR() & 0x1f;
	int i;
	uint32_t spsr_value;

	memset(remcomOutBuffer, 0, sizeof(remcomOutBuffer));

	// send out the T packet
	ptr = &remcomOutBuffer[0];
	*ptr++ = 'T';
	if(currentMode == 0x17 )
	{
		*ptr++ = hexChars[SIGTRAP >> 4];
		*ptr++ = hexChars[SIGTRAP & 0xf];
	}
	else
	{
//		*ptr++ = hexchars[SIGILL >> 4];
//		*ptr++ = hexchars[SIGILL & 0xf];
		*ptr++ = hexChars[SIGTRAP >> 4];
		*ptr++ = hexChars[SIGTRAP & 0xf];
	}

	for(i = 0; i < 15; i++)
	{
		*ptr++ = hexChars[i >> 4];
		*ptr++ = hexChars[i & 0xf];
		*ptr++ = ':';
		mem2hex((uint8_t *)&exceptionRegisters[i], ptr, 4);
		ptr += 8;
		*ptr++ = ';';
	}
	// the PC register
	*ptr++ = hexChars[0xf >> 4];
	*ptr++ = hexChars[0xf & 0xf];
	*ptr++ = ':';
	mem2hex((uint8_t *)&retAddr, ptr, 4);
	ptr += 8;
	*ptr++ = ';';

	// the CPSR register
	spsr_value = getSPSR();
	*ptr++ = hexChars[0x19 >> 4];
	*ptr++ = hexChars[0x19 & 0xf];
	*ptr++ = ':';
	mem2hex((uint8_t *)&spsr_value, ptr, 4);
	ptr += 8;
	*ptr++ = ';';
	*ptr = 0;
}

static void exceptionHandler()
{
	exceptionRegisters[15] = *(uint32_t *)0x027FFD98;

//	iprintf("Exception Address : %08X\n", exceptionRegisters[15]);

	retAddr = computeReturnAddress();

//	iprintf("Return Address : %08X\n", retAddr);

	build_t_packet();

	putPacket(remcomOutBuffer);

	debugStub();

//	iprintf("jump back before\n");

	jumpBack(retAddr);

//	iprintf("jump back after\n");
	//jumpBack(exceptionRegisters[15]);
}

static void firstHandler()
{
	exceptionRegisters[15] = *(uint32_t *)0x027FFD98;

//	iprintf("Exception Address : %08X\n", exceptionRegisters[15]);

	retAddr = computeReturnAddress();

//	iprintf("Return Address : %08X\n", retAddr);

	setExceptionHandler(exceptionHandler);

	debugStub();

//	iprintf("jump back before\n");

	//jumpBack(debug_stub_descr.ret_addr);
	jumpBack(exceptionRegisters[15]);

//	iprintf("jump back after\n");
}

void waitInput()
{
	uint32_t pressedKey;

	while(1)
	{
		scanKeys();
		pressedKey = keysDown();

		if(pressedKey & KEY_A)
		{
			break;
		}
	}
}
