#ifndef INTERRUPT_H_
#define INTERRUPT_H_

// Includes
#include "cpu.h"
#include "cp15.h"
#include "spinlock.h"
#include "xscugic.h"

// Number of cores
#define INTERRUPT_CORECOUNT CPU_CORECOUNT

// Maximum number of interrupts
#define INTERRUPT_MAXIMUMINTERRUPTS XSCUGIC_MAX_NUM_INTR_INPUTS

// Interrupt vector tables
extern void (**INTERRUPT_VECTORTABLE[INTERRUPT_CORECOUNT])(void);

// Startup method
unsigned char INTERRUPT_STARTUP(void);

// Registration method
unsigned char INTERRUPT_REGISTER(unsigned int INTERRUPT, void (*HANDLER)(void));

// Unregistration method
unsigned char INTERRUPT_UNREGISTER(unsigned int INTERRUPT);

#endif
