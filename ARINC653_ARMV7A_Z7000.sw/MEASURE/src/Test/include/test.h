#ifndef TEST_H_
#define TEST_H_

// ARINC653 core includes
#include "arinc653_core.h"

// Test LED macros
#define TEST_LED1_ON(); {\
	Xil_Out32(0xE000A000 + (2 * 0x00000008U) + 0x00000000, 0xFFFE0000);\
}
#define TEST_LED2_ON(); {\
	Xil_Out32(0xE000A000 + (2 * 0x00000008U) + 0x00000000, 0xFFFD0000);\
}
#define TEST_LED1_OFF(); {\
	Xil_Out32(0xE000A000 + (2 * 0x00000008U) + 0x00000000, 0xFFFEFFFF);\
}
#define TEST_LED2_OFF(); {\
	Xil_Out32(0xE000A000 + (2 * 0x00000008U) + 0x00000000, 0xFFFDFFFF);\
}

// Test counter values
#define TEST_COUNTER_TINY 0x120 // LED on/off loop should run in <~1ms
#define TEST_COUNTER_TINYSMALL 0x680 // LED on/off loop should run in <~5ms
#define TEST_COUNTER_SMALL 0xD00 // LED on/off loop should run in <~10ms
#define TEST_COUNTER_SMALLMEDIUM 0x1E00 // LED on/off loop should run in <~25ms
#define TEST_COUNTER_MEDIUM 0x4100 // LED on/off loop should run in <~50ms
#define TEST_COUNTER_MEDIUMLARGE 0x8400 // LED on/off loop should run in <~100ms
#define TEST_COUNTER_LARGE 0x14000 // LED on/off loop should run in <~250ms
#define TEST_COUNTER_LARGEHUGE 0x37AB8 // LED on/off loop should run in <~750ms
#define TEST_COUNTER_HUGE 0x54400 // LED on/off loop should run in <~1s

// Test startup method
unsigned char TEST_STARTUP(void);

#endif
