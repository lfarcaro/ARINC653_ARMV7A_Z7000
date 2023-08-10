// Includes
#include "timer_tick.h"

#define XPAR_GLOBAL_TMR_COUNTERREGISTER1 (XPAR_GLOBAL_TMR_BASEADDR + 0x00)
#define XPAR_GLOBAL_TMR_COUNTERREGISTER2 (XPAR_GLOBAL_TMR_BASEADDR + 0x04)
#define XPAR_GLOBAL_TMR_CONTROLREGISTER (XPAR_GLOBAL_TMR_BASEADDR + 0x08)
#define XPAR_GLOBAL_TMR_INTERRUPTSTATUSREGISTER (XPAR_GLOBAL_TMR_BASEADDR + 0x0C)
#define XPAR_GLOBAL_TMR_COMPARATORVALUEREGISTER1 (XPAR_GLOBAL_TMR_BASEADDR + 0x10)
#define XPAR_GLOBAL_TMR_COMPARATORVALUEREGISTER2 (XPAR_GLOBAL_TMR_BASEADDR + 0x14)
#define XPAR_GLOBAL_TMR_AUTOINCREMENTREGISTER (XPAR_GLOBAL_TMR_BASEADDR + 0x18)

// Startup method
unsigned char TIMER_TICK_STARTUP(void) {

	// Shuts down timer
	TIMER_TICK_SHUTDOWN();

	// Sets global timer counter register to zero
	Xil_Out32(XPAR_GLOBAL_TMR_COUNTERREGISTER1, 0x00000000);
	Xil_Out32(XPAR_GLOBAL_TMR_COUNTERREGISTER2, 0x00000000);
	return 1;
}

// Run method
unsigned char TIMER_TICK_RUN(void (*HANDLER)(void)) {

	// Initializes global timer comparator value register to zero
	Xil_Out32(XPAR_GLOBAL_TMR_COMPARATORVALUEREGISTER1, 0x00000000);
	Xil_Out32(XPAR_GLOBAL_TMR_COMPARATORVALUEREGISTER2, 0x00000000);

	// Sets global timer auto increment register
	Xil_Out32(XPAR_GLOBAL_TMR_AUTOINCREMENTREGISTER, TIMER_TICK_INCREMENTVALUE);

	// Registers the timer's interrupt handler
	INTERRUPT_REGISTER(XPAR_GLOBAL_TMR_INTR, HANDLER);

	// Enables global timer with Auto increment, Comparison and IRQ
	Xil_Out32(XPAR_GLOBAL_TMR_CONTROLREGISTER, Xil_In32(XPAR_GLOBAL_TMR_CONTROLREGISTER) | 0x0000000F);
	return 1;
}

// Shutdown method
unsigned char TIMER_TICK_SHUTDOWN(void) {

	// Fully disables global timer
	Xil_Out32(XPAR_GLOBAL_TMR_CONTROLREGISTER, 0x00000000);
	return 1;
}
