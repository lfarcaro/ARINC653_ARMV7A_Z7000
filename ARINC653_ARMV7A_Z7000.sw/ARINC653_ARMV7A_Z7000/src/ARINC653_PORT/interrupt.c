// Includes
#include "interrupt.h"

// Cores' interrupt vector tables
void (*INTERRUPT_VECTORTABLE_CORE0[INTERRUPT_MAXIMUMINTERRUPTS])(void);
void (*INTERRUPT_VECTORTABLE_CORE1[INTERRUPT_MAXIMUMINTERRUPTS])(void);

// Interrupt vector tables
void (**INTERRUPT_VECTORTABLE[INTERRUPT_CORECOUNT])(void) = {
	INTERRUPT_VECTORTABLE_CORE0,
	INTERRUPT_VECTORTABLE_CORE1
};

// XScuGic configuration data pointer
static XScuGic_Config *XScuGicConfigPtr[INTERRUPT_CORECOUNT];

// XScuGic instance data
static XScuGic XScuGicInst[INTERRUPT_CORECOUNT];

// Spin lock
static SPINLOCK_TYPE INTERRUPT_SPINLOCK = SPINLOCK_UNLOCKED;

// Default interrupt handler
static void INTERRUPT_DEFAULTHANDLER(void) {
}

// Startup method
unsigned char INTERRUPT_STARTUP(void) {
	unsigned int CORE;
	unsigned int INTERRUPT;

	// Locks spin lock
	SPINLOCK_LOCK(&INTERRUPT_SPINLOCK);

	// Gets core ID
	CORE = CP15CoreIDGet();

	// Assigns the default handler for all interrupts
	for (INTERRUPT = 0; INTERRUPT < INTERRUPT_MAXIMUMINTERRUPTS; INTERRUPT++) {
		INTERRUPT_VECTORTABLE[CORE][INTERRUPT] = INTERRUPT_DEFAULTHANDLER;
	}

	// Sets core ID
	XScuGic_SetCpuID(CORE);

	// Looks up the interrupt controller's configuration
	XScuGicConfigPtr[CORE] = XScuGic_LookupConfig(XPAR_SCUGIC_SINGLE_DEVICE_ID);
	if (XScuGicConfigPtr[CORE] == NULL) {
		goto EXIT_ERROR;
	}

	// Initializes the interrupt controller's configuration
	if (XScuGic_CfgInitialize(&XScuGicInst[CORE], XScuGicConfigPtr[CORE], XScuGicConfigPtr[CORE]->CpuBaseAddress) != XST_SUCCESS) {
		goto EXIT_ERROR;
	}

	// Enables interrupts
	Xil_ExceptionEnable();

	// Unlocks spin lock
	SPINLOCK_UNLOCK(&INTERRUPT_SPINLOCK);
	return 1;

// Exit with error label
EXIT_ERROR:

	// Unlocks spin lock
	SPINLOCK_UNLOCK(&INTERRUPT_SPINLOCK);
	return 0;
}

// Registration method
unsigned char INTERRUPT_REGISTER(unsigned int INTERRUPT, void (*HANDLER)(void)) {
	unsigned int CORE;

	// Locks spin lock
	SPINLOCK_LOCK(&INTERRUPT_SPINLOCK);

	// Gets core ID
	CORE = CP15CoreIDGet();

	// Verifies core
	if (CORE >= INTERRUPT_CORECOUNT) {
		goto EXIT_ERROR;
	}

	// Verifies interrupt
	if (INTERRUPT >= INTERRUPT_MAXIMUMINTERRUPTS) {
		goto EXIT_ERROR;
	}

	// Registers interrupt
	INTERRUPT_VECTORTABLE[CORE][INTERRUPT] = HANDLER;

	// Enables the interrupt
	XScuGic_Enable(&XScuGicInst[CORE], INTERRUPT);

	// Unlocks spin lock
	SPINLOCK_UNLOCK(&INTERRUPT_SPINLOCK);
	return 1;

// Exit with error label
EXIT_ERROR:

	// Unlocks spin lock
	SPINLOCK_UNLOCK(&INTERRUPT_SPINLOCK);
	return 0;
}

// Unregistration method
unsigned char INTERRUPT_UNREGISTER(unsigned int INTERRUPT) {
	unsigned int CORE;
	unsigned int INDEX;
	unsigned char DISABLE;

	// Locks spin lock
	SPINLOCK_LOCK(&INTERRUPT_SPINLOCK);

	// Gets core ID
	CORE = CP15CoreIDGet();

	// Verifies core
	if (CORE >= INTERRUPT_CORECOUNT) {
		goto EXIT_ERROR;
	}

	// Verifies interrupt
	if (INTERRUPT >= INTERRUPT_MAXIMUMINTERRUPTS) {
		goto EXIT_ERROR;
	}

	// Unregisters interrupt
	INTERRUPT_VECTORTABLE[CORE][INTERRUPT] = INTERRUPT_DEFAULTHANDLER;

	// Sets disable flag
	DISABLE = TRUE;

	// Assigns the default handler for all interrupts
	for (INDEX = 0; INDEX < INTERRUPT_CORECOUNT; INDEX++) {
		if (INTERRUPT_VECTORTABLE[CORE][INTERRUPT] != INTERRUPT_DEFAULTHANDLER) {
			DISABLE = FALSE;
		}
	}

	// Verifies disable flag
	if (DISABLE) {

		// Disables the interrupt
		XScuGic_Disable(&XScuGicInst[CORE], INTERRUPT);
	}

	// Unlocks spin lock
	SPINLOCK_UNLOCK(&INTERRUPT_SPINLOCK);
	return 1;

// Exit with error label
EXIT_ERROR:

	// Unlocks spin lock
	SPINLOCK_UNLOCK(&INTERRUPT_SPINLOCK);
	return 0;
}
