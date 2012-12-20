#ifndef GDBSTUBASM_H_
#define GDBSTUBASM_H_

// brief Calling this function will cause a jump to the debugger.
void BreakPoint( void);

// Flush the instruction cache (nicked from libnds)
void ICInvalidateAll( void);

// Flush the data cache (nicked from libnds)
void DCFlushAll( void);

// Enable the interrupts in the CPSR
void enable_IRQs( void);

// Disable the interrupts in the CPSR
void disable_IRQs( void);

// Return the SPSR register value
uint32_t getSPSR( void);

// Return the CPSR register value
//uint32_t getCPSR( void);

// Set the banked R13 and R14 register values
uint32_t setBankedReg( uint32_t r13, uint32_t r14);

uint32_t setRegs(uint32_t* regs);

uint32_t test_func(void);

uint32_t v5T_semantics_asm(void);

uint32_t add(uint32_t a, uint32_t b);

uint32_t mul(uint32_t a, uint32_t b);

#endif /* GDB_UTILITIES_H_ */
