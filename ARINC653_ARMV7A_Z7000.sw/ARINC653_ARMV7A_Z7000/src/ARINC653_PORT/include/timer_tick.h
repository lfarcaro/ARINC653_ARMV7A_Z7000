#ifndef TIMER_TICK_H_
#define TIMER_TICK_H_

// Includes
#include "interrupt.h"

// Auto increment value
#define TIMER_TICK_INCREMENTVALUE 250000

// Startup method
unsigned char TIMER_TICK_STARTUP(void);

// Run method
unsigned char TIMER_TICK_RUN(void (*HANDLER)(void));

// Shutdown method
unsigned char TIMER_TICK_SHUTDOWN(void);

#endif
