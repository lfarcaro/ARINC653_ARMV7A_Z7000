#ifndef ARINC653_PORT_H_
#define ARINC653_PORT_H_

// Hardware includes
#include "cpu.h"
#include "cp15.h"
#include "console.h"
#include "interrupt.h"
#include "mmu.h"
#include "pmu.h"
#include "timer_tick.h"
#include "xstatus.h"
#include "xil_cache.h"
#include "xgpiops.h"

// Debug mode

#define DEBUG

// Optional hardware includes

// Core count
#define PORT_CORECOUNT CPU_CORECOUNT

// Generate console output for errors flag
#define PORT_GENERATECONSOLEOUTPUT_ERRORS true

// Generate console output for report flag
#define PORT_GENERATECONSOLEOUTPUT_REPORTS true

// Tick window (in nanoseconds)
#define PORT_TICKWINDOW 1000000

// Heap alignment
#define PORT_HEAP_ALIGNMENT 4

// Vector table addresses
#define PORT_VECTORTABLE_ADDRESS_CORE0 0x00000000
#define PORT_VECTORTABLE_ADDRESS_CORE1 0x00001000

// Vector table size
#define PORT_VECTORTABLE_SIZE 16

// -------------------- OPTIONAL MODULES START --------------------

// Interpartition communication - Sampling port - Minimum message size
#define PORT_INTERPARTITIONCOMMUNICATION_SAMPLINGPORT_MINIMUMMESSAGESIZE 0x0001

// Interpartition communication - Sampling port - Maximum message size
#define PORT_INTERPARTITIONCOMMUNICATION_SAMPLINGPORT_MAXIMUMMESSAGESIZE 0x0200

// Interpartition communication - Queuing port - Minimum message count
#define PORT_INTERPARTITIONCOMMUNICATION_QUEUINGPORT_MINIMUMMESSAGECOUNT 0x0001

// Interpartition communication - Queuing port - Maximum message count
#define PORT_INTERPARTITIONCOMMUNICATION_QUEUINGPORT_MAXIMUMMESSAGECOUNT 0x0200

// Interpartition communication - Queuing port - Minimum message size
#define PORT_INTERPARTITIONCOMMUNICATION_QUEUINGPORT_MINIMUMMESSAGESIZE 0x0001

// Interpartition communication - Queuing port - Maximum message size
#define PORT_INTERPARTITIONCOMMUNICATION_QUEUINGPORT_MAXIMUMMESSAGESIZE 0x0200

// Intrapartition communication - Buffer - Minimum message count
#define PORT_INTRAPARTITIONCOMMUNICATION_BUFFER_MINIMUMMESSAGECOUNT 0x0001

// Intrapartition communication - Buffer - Maximum message count
#define PORT_INTRAPARTITIONCOMMUNICATION_BUFFER_MAXIMUMMESSAGECOUNT 0x0200

// Intrapartition communication - Buffer - Minimum message size
#define PORT_INTRAPARTITIONCOMMUNICATION_BUFFER_MINIMUMMESSAGESIZE 0x0001

// Intrapartition communication - Buffer - Maximum message size
#define PORT_INTRAPARTITIONCOMMUNICATION_BUFFER_MAXIMUMMESSAGESIZE 0x0200

// Intrapartition communication - Blackboard - Minimum message size
#define PORT_INTRAPARTITIONCOMMUNICATION_BLACKBOARD_MINIMUMMESSAGESIZE 0x0001

// Intrapartition communication - Blackboard - Maximum message size
#define PORT_INTRAPARTITIONCOMMUNICATION_BLACKBOARD_MAXIMUMMESSAGESIZE 0x0200

// Intrapartition communication - Semaphore - Maximum value
#define PORT_INTRAPARTITIONCOMMUNICATION_SEMAPHORE_MAXIMUMVALUE 0xFFFF

// -------------------- OPTIONAL MODULES END --------------------

// Specific error identifiers - Range from 100 to 999
#define ERRORIDENTIFIER_TRANSLATIONFAULT 100
#define ERRORIDENTIFIER_MEMORYALIGNMENTFAULT 101
#define ERRORIDENTIFIER_MEMORYACCESSFAULT 102
#define ERRORIDENTIFIER_CACHEFAULT 103
#define ERRORIDENTIFIER_UNDEFINEDINSTRUCTION 104

// Release cores flag
extern portUINTBASE PORT_RELEASECORES;

// Cores' entry points
extern void (*ENTRYPOINT_CORE0)(void);
extern void (*ENTRYPOINT_CORE1)(void);

// Cores' module configurations
extern MODULE_CONFIGURATION_TYPE *MODULE_CONFIGURATION_CORE0;
extern MODULE_CONFIGURATION_TYPE *MODULE_CONFIGURATION_CORE1;

// Prepare call address - Should store the address from which the current function was called to be retrieved by PORT_GETCALLADDRESS
#define PORT_PREPARECALLADDRESS(); {\
	/*Stores LR in stack*/\
	__asm("	STMFD SP!, {LR}");\
}

// Get call address - Should return the address from which the current function was called, previously stored into stack
portADDRESS PORT_GETCALLADDRESS(void);

// Hook - STARTUP_SYSTEM
portBOOLEAN PORT_HOOK_STARTUP_SYSTEM(void);

// Hook - RUN_SYSTEM
portBOOLEAN PORT_HOOK_RUN_SYSTEM(void);

// Hook - Before STARTUP_MODULE
portBOOLEAN PORT_HOOK_BEFORE_STARTUP_MODULE(void);

// Hook - After STARTUP_MODULE
portBOOLEAN PORT_HOOK_AFTER_STARTUP_MODULE(void);

// Hook - Before CREATE_PARTITION
portBOOLEAN PORT_HOOK_BEFORE_CREATE_PARTITION(portADDRESS PARTITION_INFORMATION_ADDRESS);

// Hook - After CREATE_PARTITION
portBOOLEAN PORT_HOOK_AFTER_CREATE_PARTITION(portADDRESS PARTITION_INFORMATION_ADDRESS);

// Hook - Before CREATE_PROCESS
portBOOLEAN PORT_HOOK_BEFORE_CREATE_PROCESS(portADDRESS PARTITION_INFORMATION_ADDRESS, portADDRESS PROCESS_INFORMATION_ADDRESS);

// Hook - After CREATE_PROCESS
portBOOLEAN PORT_HOOK_AFTER_CREATE_PROCESS(portADDRESS PARTITION_INFORMATION_ADDRESS, portADDRESS PROCESS_INFORMATION_ADDRESS);

// Hook - Before CREATE_ERROR_HANDLER
portBOOLEAN PORT_HOOK_BEFORE_CREATE_ERROR_HANDLER(portADDRESS PARTITION_INFORMATION_ADDRESS, portADDRESS ERRORHANDLER_INFORMATION_ADDRESS);

// Hook - After CREATE_ERROR_HANDLER
portBOOLEAN PORT_HOOK_AFTER_CREATE_ERROR_HANDLER(portADDRESS PARTITION_INFORMATION_ADDRESS, portADDRESS ERRORHANDLER_INFORMATION_ADDRESS);

// Initialize tick method
void PORT_INITIALIZETICK(void);

// Context identifier range getter - Determines the range of context identifiers that can be used by the current module
void PORT_GETCONTEXTIDENTIFIERRANGE(IDENTIFIER_TYPE *FIRST_CONTEXT_IDENTIFIER, IDENTIFIER_TYPE *LAST_CONTEXT_IDENTIFIER);

// Initialize context method - Initializes context in its creation time
void PORT_INITIALIZECONTEXT(CONTEXT_TYPE *CONTEXT);

// Prepare context method - Prepares context for running (called multiple times)
void PORT_PREPARECONTEXT(CONTEXT_TYPE *CONTEXT);

// Reset handler methods - Defined in ASM module
extern void PORT_RESETHANDLER(void);
extern void PORT_RESETHANDLER_CORE0(void);
extern void PORT_RESETHANDLER_CORE1(void);

// Core getter - Defined in ASM module
extern portUINTBASE PORT_GETCORE(void);

// System running method - Defined in ASM module
extern void PORT_RUNSYSTEM(void);

// Abort handler method - Defined in ASM module
extern void PORT_ABORTHANDLER(void);

// Undefined instruction handler method - Defined in ASM module
extern void PORT_UNDEFINEDINSTRUCTIONHANDLER(void);

// IRQ handler method - Defined in ASM module
extern void PORT_IRQHANDLER(void);

// FIQ handler method - Defined in ASM module
extern void PORT_FIQHANDLER(void);

// SVC handler method - Defined in ASM module
extern void PORT_SVCHANDLER(void);

// Enter core method - Defined in ASM module
extern void PORT_ENTERCORE(void);

// Exit core method - Defined in ASM module
extern void PORT_EXITCORE(void);

// Disable interrupts method - Defined in ASM module
extern void PORT_DISABLEINTERRUPTS(void);

// Enable interrupts method - Defined in ASM module
extern void PORT_ENABLEINTERRUPTS(void);

// Exit privileged mode method - Defined in ASM module
extern void PORT_EXITPRIVILEGEDMODE(void);

// Tick method - Defined in ASM module
extern void PORT_TICK(void);

// Yield method - Defined in ASM module
extern void PORT_YIELD(void);

// Switch method - Defined in ASM module
extern void PORT_SWITCH(void);

// Start scheduler method - Defined in ASM module
extern void PORT_STARTSCHEDULER(void);

// Terminate tick method
void PORT_TERMINATETICK(void);

// Module restart method
void PORT_RESTARTMODULE(void);

// Abort error handler method
void PORT_ABORTERRORHANDLER(portUINTBASE DFAR, portUINTBASE DFSR, portUINTBASE IFAR, portUINTBASE IFSR, portUINTBASE LR);

// Undefined instruction error handler method
void PORT_UNDEFINEDINSTRUCTIONERRORHANDLER(portUINTBASE LR);

// Report system error method
void PORT_REPORTSYSTEMERROR(SYSTEM_STATE_TYPE SYSTEM_STATE, ERROR_IDENTIFIER_TYPE ERROR_IDENTIFIER, ERROR_CODE_TYPE ERROR_CODE, MESSAGE_ADDR_TYPE MESSAGE_ADDR, ERROR_MESSAGE_SIZE_TYPE LENGTH);

// Report module error method
void PORT_REPORTMODULEERROR(SYSTEM_STATE_TYPE SYSTEM_STATE, ERROR_IDENTIFIER_TYPE ERROR_IDENTIFIER, ERROR_CODE_TYPE ERROR_CODE, MESSAGE_ADDR_TYPE MESSAGE_ADDR, ERROR_MESSAGE_SIZE_TYPE LENGTH);

// Report partition error method
void PORT_REPORTPARTITIONERROR(SYSTEM_STATE_TYPE SYSTEM_STATE, ERROR_IDENTIFIER_TYPE ERROR_IDENTIFIER, ERROR_CODE_TYPE ERROR_CODE, MESSAGE_ADDR_TYPE MESSAGE_ADDR, ERROR_MESSAGE_SIZE_TYPE LENGTH);

// Report process error method
void PORT_REPORTPROCESSERROR(SYSTEM_STATE_TYPE SYSTEM_STATE, ERROR_IDENTIFIER_TYPE ERROR_IDENTIFIER, ERROR_CODE_TYPE ERROR_CODE, MESSAGE_ADDR_TYPE MESSAGE_ADDR, ERROR_MESSAGE_SIZE_TYPE LENGTH, PROCESS_ID_TYPE FAILED_PROCESS_ID, SYSTEM_ADDRESS_TYPE FAILED_ADDRESS);

// Report application message method
void PORT_REPORTAPPLICATIONMESSAGE(MESSAGE_ADDR_TYPE MESSAGE_ADDR, MESSAGE_SIZE_TYPE LENGTH);

#endif
