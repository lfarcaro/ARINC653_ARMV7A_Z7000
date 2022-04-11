	// Imports C declarations
	#include "xparameters.h"
	// From xscugic_hw.h:
	#define XSCUGIC_HI_PEND_OFFSET 0x00000018U /**< Highest Pending Interrupt Register */
	#define XSCUGIC_INT_ACK_OFFSET 0x0000000CU /**< Interrupt ACK Reg */
	#define XSCUGIC_EOI_OFFSET 0x00000010U /**< End of Interrupt Reg */
	#define XSCUGIC_ACK_INTID_MASK 0x000003FFU /**< Interrupt ID */
	// From xscutimer_hw.h:
	#define XSCUTIMER_ISR_OFFSET 0x0000000CU /**< Timer Interrupt Status Register */
	#define XSCUTIMER_ISR_EVENT_FLAG_MASK 0x00000001U /**< Event flag */

	// References global symbols
	.extern PORT_RELEASECORES
	.extern ENTRYPOINT_CORE0

	// Variables and functions addresses
	.extern SCHEDULER
	.extern TICK
	.extern PORT_ABORTERRORHANDLER
	.extern PORT_UNDEFINEDINSTRUCTIONERRORHANDLER
	.extern INTERRUPT_VECTORTABLE

// Mode constants
.set MODE_MASK, 0x1F
.set MODE_USR, 0x10
.set MODE_FIQ, 0x11
.set MODE_IRQ, 0x12
.set MODE_SVC, 0x13
.set MODE_ABT, 0x17
.set MODE_UND, 0x1B
.set MODE_SYS, 0x1F

// Service constants
.set SERVICE_TICK, 0
.set SERVICE_ENTERCORE, 1
.set SERVICE_EXITCORE, 2
.set SERVICE_YIELD, 3
.set SERVICE_SWITCH, 4
.set SERVICE_DISABLEINTERRUPTS, 5
.set SERVICE_ENABLEINTERRUPTS, 6

// Core getter
	.align 4
	.global PORT_GETCORE
PORT_GETCORE: .func

	// Returns CPU ID
	MRC p15, 0, R0, c0, c0, 5
	AND R0, R0, #0xF
	BX LR
	.endfunc

// System running method
	.align 4
	.global PORT_RUNSYSTEM
PORT_RUNSYSTEM: .func

	// Reads CPU ID
	MRC p15, 0, R0, c0, c0, 5
	AND R0, R0, #0xF
	CMP R0, #0
PORT_RUNSYSTEM_HANGOTHERCPUS:
	BNE PORT_RUNSYSTEM_HANGOTHERCPUS

	// Releases all other cores
	LDR R0, =#1
	LDR R1, =_PORT_RELEASECORES
	LDR R1, [R1]
	STR R0, [R1]
	DSB
	SEV

	// Proceeds to Core #0 entry point
	LDR R0, =_ENTRYPOINT_CORE0
	LDR R0, [R0]
	LDR R0, [R0]
	BX R0
	.endfunc

// Save general context macro - Should be used in SYS mode
.macro macroPORT_SAVEGENERALCONTEXT AUXILIARY_MODE
	// Pushes R0-R1 into auxiliary mode stack
	CPS \AUXILIARY_MODE
	STMFD SP!, {R0, R1}
	CPS #MODE_SYS

	// Gets CPU ID
	MRC p15, 0, R1, c0, c0, 5
	AND R1, R1, #0xF
	LSL R1, R1, #2 // Prepares core index in array

	// Gets CORE_CURRENT_CONTEXT pointer
	LDR R0, _CORE_CURRENT_CONTEXT
	ADD R0, R0, R1
	LDR R0, [R0]
	// Gets CORE_CURRENT_CONTEXT.PORT_CONTEXT element pointer
	LDR R0, [R0, #12]

	// Points context end
	ADD R0, R0, #((2+(32*2)+2+15)*4)

	// Saves general purpose registers
	// Saves R2 to R14
	STMFD R0!, {R2-R14}
	// Pops R0-R1 into R2-R3 from auxiliary mode stack
	CPS \AUXILIARY_MODE
	LDMFD SP!, {R2, R3}
	CPS #MODE_SYS
	// Saves R0-R1 from R2-R3
	STMFD R0!, {R2, R3}

	// Saves NEON/VFP coprocessor registers
	// Saves FPSCR and FPEXC
	VMRS R1, FPSCR
	VMRS R2, FPEXC
	STMFD R0!, {R1, R2}
	// Saves D0 to D31
	VSTMDB R0!, {D16-D31}
	VSTMDB R0!, {D0-D15}
	.endm
// Save specific context macro
// R2 = LR
// R3 = CPSR
.macro macroPORT_SAVESPECIFICCONTEXT
	// Gets CPU ID
	MRC p15, 0, R1, c0, c0, 5
	AND R1, R1, #0xF
	LSL R1, R1, #2 // Prepares core index in array

	// Gets CORE_CURRENT_CONTEXT pointer
	LDR R0, _CORE_CURRENT_CONTEXT
	ADD R0, R0, R1
	LDR R0, [R0]
	// Gets CORE_CURRENT_CONTEXT.PORT_CONTEXT element pointer
	LDR R0, [R0, #12]

	// Points other information start
	ADD R1, R0, #((2+(32*2)+2+15)*4)

	// Nothing to save

	// Points CPSR and LR end
	ADD R1, R0, #((2)*4)

	// Saves CPSR and LR
	STMFD R1!, {R2, R3}
	.endm
// Restore specific context macro
// R2 = LR
// R3 = CPSR
.macro macroPORT_RESTORESPECIFICCONTEXT
	// Gets CPU ID
	MRC p15, 0, R1, c0, c0, 5
	AND R1, R1, #0xF
	LSL R1, R1, #2 // Prepares core index in array

	// Gets CORE_CURRENT_CONTEXT pointer
	LDR R0, _CORE_CURRENT_CONTEXT
	ADD R0, R0, R1
	LDR R0, [R0]
	// Gets CORE_CURRENT_CONTEXT.PORT_CONTEXT element pointer
	LDR R0, [R0, #12]

	// Points other information start
	ADD R1, R0, #((2+(32*2)+2+15)*4)

	// Restores CORE_CURRENT_CONTEXT.PORT_CONTEXT.CONTEXT_IDENTIFIER element
	LDR R2, [R1, #(0*4)]
	MCR p15, #0, R2, c13, c0, #1 // Sets CONTEXTIDR
	// Restores CORE_CURRENT_CONTEXT.PORT_CONTEXT.FLTRANSLATIONTABLE_ADDRESS element
	LDR R2, [R1, #(1*4)]
	MCR p15, #0, R2, c2, c0, #0 // Sets TTBR0
	DMB

	// Points context start
	ADD R1, R0, #0

	// Restores CPSR and LR
	LDMFD R1!, {R2, R3}
	.endm
// Restore general context macro - Should be used in SYS mode
.macro macroPORT_RESTOREGENERALCONTEXT AUXILIARY_MODE
	// Gets CPU ID
	MRC p15, 0, R1, c0, c0, 5
	AND R1, R1, #0xF
	LSL R1, R1, #2 // Prepares core index in array

	// Gets CORE_CURRENT_CONTEXT pointer
	LDR R0, _CORE_CURRENT_CONTEXT
	ADD R0, R0, R1
	LDR R0, [R0]
	// Gets CORE_CURRENT_CONTEXT.PORT_CONTEXT element pointer
	LDR R0, [R0, #12]

	// Points context start
	ADD R1, R0, #((2)*4)

	// Restores NEON/VFP coprocessor registers
	// Restores D0 to D31
	VLDMIA R1!, {D0-D15}
	VLDMIA R1!, {D16-D31}
	// Restores FPSCR and FPEXC
	LDMFD R1!, {R2, R3}
	VMSR FPSCR, R2
	VMSR FPEXC, R3

	// Restores general registers
	// Restores R0 to R14
	LDMFD R1, {R0-LR}^
	.endm

	.text
	.code 32

// Abort handler method
	.align 4
	.global PORT_ABORTHANDLER
PORT_ABORTHANDLER: .func
	// Corrects LR
	SUB LR, LR, #8

	// Switches into SYS mode
	CPS #MODE_SYS
	// Saves general context
	macroPORT_SAVEGENERALCONTEXT #MODE_ABT
	// Saves specific context
	CPS #MODE_ABT
	MOV R2, LR
	MRS R3, SPSR
	CPS #MODE_SYS
	macroPORT_SAVESPECIFICCONTEXT
	// Switches into ABT mode
	CPS #MODE_ABT

	// Prepares handler parameters
	MOV R0, LR
	STMFD SP!, {R0}
	MRC p15, #0, R0, c6, c0, #0 // Reads DFAR
	MRC p15, #0, R1, c5, c0, #0 // Reads DFSR
	MRC p15, #0, R2, c6, c0, #2 // Reads IFAR
	MRC p15, #0, R3, c5, c0, #1 // Reads IFSR
	// Enters handler
	BL PORT_ABORTERRORHANDLER
	// Pops handler parameters
	LDMFD SP!, {R0}

	// Clears data and prefetch exception registers
	MOV R0, #0
	MCR p15, #0, R0, c6, c0, #0 // Clears DFAR
	MCR p15, #0, R0, c5, c0, #0 // Clears DFSR
	MCR p15, #0, R0, c6, c0, #2 // Clears IFAR
	MCR p15, #0, R0, c5, c0, #1 // Clears IFSR

	// Calls scheduler
	BL SCHEDULER

	// Switches into SYS mode
	CPS #MODE_SYS
	// Restores specific context
	macroPORT_RESTORESPECIFICCONTEXT
	CPS #MODE_ABT
	MOV LR, R2
	MSR SPSR_cxfs, R3
	CPS #MODE_SYS
	// Restores general context
	macroPORT_RESTOREGENERALCONTEXT #MODE_ABT
	// Switches into ABT mode
	CPS #MODE_ABT

	// Returns
	MOVS PC, LR
	.endfunc

// Undefined instruction handler method
	.align 4
	.global PORT_UNDEFINEDINSTRUCTIONHANDLER
PORT_UNDEFINEDINSTRUCTIONHANDLER: .func
	// Corrects LR
	SUB LR, LR, #4

	// Switches into SYS mode
	CPS #MODE_SYS
	// Saves general context
	macroPORT_SAVEGENERALCONTEXT #MODE_UND
	// Saves specific context
	CPS #MODE_UND
	MOV R2, LR
	MRS R3, SPSR
	CPS #MODE_SYS
	macroPORT_SAVESPECIFICCONTEXT
	// Switches into UND mode
	CPS #MODE_UND

	// Prepares handler parameters
	MOV R0, LR
	// Enters handler
	BL PORT_UNDEFINEDINSTRUCTIONERRORHANDLER

	// Calls scheduler
	BL SCHEDULER

	// Switches into SYS mode
	CPS #MODE_SYS
	// Restores specific context
	macroPORT_RESTORESPECIFICCONTEXT
	CPS #MODE_UND
	MOV LR, R2
	MSR SPSR_cxfs, R3
	CPS #MODE_SYS
	// Restores general context
	macroPORT_RESTOREGENERALCONTEXT #MODE_UND
	// Switches into UND mode
	CPS #MODE_UND

	// Returns
	MOVS PC, LR
	.endfunc

// IRQ handler method
	.align 4
	.global PORT_IRQHANDLER
PORT_IRQHANDLER: .func
	// Corrects LR
	SUB LR, LR, #4

	// Saves critical context
	STMFD SP!, {R0-R12,LR}

	// Gets CPU ID
	MRC p15, 0, R1, c0, c0, 5
	AND R1, R1, #0xF
	LSL R1, R1, #2 // Prepares core index in array

	// Loads interrupt handler vector table address
	LDR R0, _INTERRUPT_VECTORTABLE
	ADD R0, R0, R1
	LDR R0, [R0]
	// Reads interrupt number
	LDR R1, ADDR_CONTROLLER_INTERRUPT_NUMBER
	LDR R1, [R1]
	LDR R2, MASK_CONTROLLER_INTERRUPT_NUMBER
	AND R2, R1, R2
	// Calculates ISR address
	LDR R0, [R0, R2, lsl #2]
	// Saves interrupt number into stack (without moving SP)
	STR R1, [SP, #-4]
	// Saves ISR address into stack (without moving SP)
	STR R0, [SP, #-8]

	// Restores changed registers from critical context (without moving SP)
	LDMFD SP, {R0-R2}

	// Allocates stack for interrupt number
	ADD SP, SP, #-4

	// Loads default return address
	LDR LR, _PORT_IRQHANDLER_EXITISR_RESTORECONTEXT

	// Enters ISR using address in stack (without moving SP)
	LDR PC, [SP, #-4]
	.endfunc

// IRQ handler exit ISR and restore context method
	.align 4
PORT_IRQHANDLER_EXITISR_RESTORECONTEXT: .func
	// Saves temporary registers
	STMFD SP!, {R0-R1}

	// Acknowledges interrupt at controller level
	LDR R1, [SP, #+8]
	LDR R0, ADDR_CONTROLLER_INTERRUPT_ACKNOWLEDGE
	STR R1, [R0]
	DSB

	// Restores temporary registers
	LDMFD SP!, {R0-R1}

	// Discards interrupt number
	ADD SP, SP, #4

	// Restores critical context
	LDMFD SP!, {R0-R12,LR}

	// Returns
	MOVS PC, LR
	.endfunc

// IRQ handler exit ISR and keep context method
	.align 4
PORT_IRQHANDLER_EXITISR_KEEPCONTEXT: .func
	// Saves temporary registers
	STMFD SP!, {R0-R1}

	// Acknowledges interrupt at controller level
	LDR R1, [SP, #+8]
	LDR R0, ADDR_CONTROLLER_INTERRUPT_ACKNOWLEDGE
	STR R1, [R0]
	DSB

	// Restores temporary registers
	LDMFD SP!, {R0-R1}

	// Discards interrupt number
	ADD SP, SP, #4

	// Discards critical context
	ADD SP, SP, #(14 * 4)

	// Returns
	MOVS PC, LR
	.endfunc

// FIQ handler method
	.align 4
	.global PORT_FIQHANDLER
PORT_FIQHANDLER: .func
	// Corrects LR
	SUB LR, LR, #4
	// Returns
	MOVS PC, LR
	.endfunc

// SVC handler method
	.align 4
	.global PORT_SVCHANDLER
PORT_SVCHANDLER: .func
	// Reserves stack space for handler address
	SUB SP, SP, #4

	// Pushes service number register
	STMFD SP!, {R0}

	// Reads SVC number
	SUB R0, LR, #4 // Calculates SVC instruction address
	LDR R0, [R0] // Reads SVC instruction
	BIC R0, R0, #0xFF000000 // Masks SVC number

	// Handles tick service
	CMP R0, #SERVICE_TICK
	LDREQ R0, _PORT_SVCHANDLER_TICK
	STREQ R0, [SP, #4]
	LDMEQFD SP!, {R0, PC}

	// Handles enter core service
	CMP R0, #SERVICE_ENTERCORE
	LDREQ R0, _PORT_SVCHANDLER_ENTERCORE
	STREQ R0, [SP, #4]
	LDMEQFD SP!, {R0, PC}

	// Handles exit core service
	CMP R0, #SERVICE_EXITCORE
	LDREQ R0, _PORT_SVCHANDLER_EXITCORE
	STREQ R0, [SP, #4]
	LDMEQFD SP!, {R0, PC}

	// Handles yield service
	CMP R0, #SERVICE_YIELD
	LDREQ R0, _PORT_SVCHANDLER_YIELD
	STREQ R0, [SP, #4]
	LDMEQFD SP!, {R0, PC}

	// Handles switch service
	CMP R0, #SERVICE_SWITCH
	LDREQ R0, _PORT_SVCHANDLER_SWITCH
	STREQ R0, [SP, #4]
	LDMEQFD SP!, {R0, PC}

	// Handles disable interrupts service
	CMP R0, #SERVICE_DISABLEINTERRUPTS
	LDREQ R0, _PORT_SVCHANDLER_DISABLEINTERRUPTS
	STREQ R0, [SP, #4]
	LDMEQFD SP!, {R0, PC}

	// Handles enable interrupts service
	CMP R0, #SERVICE_ENABLEINTERRUPTS
	LDREQ R0, _PORT_SVCHANDLER_ENABLEINTERRUPTS
	STREQ R0, [SP, #4]
	LDMEQFD SP!, {R0, PC}

	// Invalid service called
PORT_SVCHANDLER_INFINITELOOP:
	B PORT_SVCHANDLER_INFINITELOOP
	.endfunc

// Tick service handler
	.align 4
PORT_SVCHANDLER_TICK: .func
	// Switches into SYS mode
	CPS #MODE_SYS
	// Saves general context
	macroPORT_SAVEGENERALCONTEXT #MODE_SVC
	// Saves specific context
	CPS #MODE_IRQ
	LDR R2, [SP, #56] // LR is read from IRQ stack
	MRS R3, SPSR
	CPS #MODE_SYS
	macroPORT_SAVESPECIFICCONTEXT
	// Switches into SVC mode
	CPS #MODE_SVC

	// Calls tick
	STMFD SP!, {LR}
	BL TICK
	LDMFD SP!, {LR}

	// Switches into SYS mode
	CPS #MODE_SYS
	// Restores specific context
	macroPORT_RESTORESPECIFICCONTEXT
	CPS #MODE_IRQ
	MOV LR, R2
	MSR SPSR_cxfs, R3
	CPS #MODE_SYS
	// Restores general context
	macroPORT_RESTOREGENERALCONTEXT #MODE_SVC
	// Switches into SVC mode
	CPS #MODE_SVC

	// Returns
	MOVS PC, LR
	.endfunc

// Enter core service handler
	.align 4
PORT_SVCHANDLER_ENTERCORE: .func
	// Pushes temporary registers
	STMFD SP!, {R0-R1}

	// Should disable interrupts and, if not in privileged mode,
	// enter privileged mode

	// If in USR mode, disables interrupts and enters SYS mode
	MRS R0, SPSR
	AND R1, R0, #MODE_MASK
	CMP R1, #MODE_USR
	ORR R0, R0, #0xC0
	BICEQ R0, R0, #MODE_MASK
	ORREQ R0, R0, #MODE_SYS
	MSR SPSR_cxfs, R0

	// Pops temporary registers
	LDMFD SP!, {R0-R1}

	// Returns
	MOVS PC, LR
	.endfunc

// Exit core service handler
	.align 4
PORT_SVCHANDLER_EXITCORE: .func
	// Pushes temporary registers
	STMFD SP!, {R0-R2}

	// Should enable interrupts and, if in privileged mode and
	// not in system partition context, exit privileged mode

	// Gets CPU ID
	MRC p15, 0, R1, c0, c0, 5
	AND R1, R1, #0xF
	LSL R1, R1, #2 // Prepares core index in array

	// Gets CORE_CURRENT_CONTEXT pointer
	LDR R2, _CORE_CURRENT_CONTEXT
	ADD R2, R2, R1
	LDR R2, [R2]
	// Gets CORE_CURRENT_CONTEXT.SYSTEM_PARTITION_CONTEXT element
	LDRB R2, [R2, #20]

	// Verifies if this is a system partition context
	CMP R2, #0
	MOVEQ R2, #MODE_USR // If not, enter USR mode
	MOVNE R2, #MODE_SYS // If so, stay in SYS mode

	// If in SYS mode, enables interrupts and enters USR or SYS mode
	MRS R0, SPSR
	AND R1, R0, #MODE_MASK
	CMP R1, #MODE_SYS
	BIC R0, R0, #0xC0
	BICEQ R0, R0, #MODE_MASK
	ORREQ R0, R0, R2
	MSR SPSR_cxfs, R0

	// Pops temporary registers
	LDMFD SP!, {R0-R2}

	// Returns
	MOVS PC, LR
	.endfunc

// Yield service handler
	.align 4
PORT_SVCHANDLER_YIELD: .func
	// Switches into SYS mode
	CPS #MODE_SYS
	// Saves general context
	macroPORT_SAVEGENERALCONTEXT #MODE_SVC
	// Saves specific context
	MOV R2, LR
	CPS #MODE_SVC
	MRS R3, SPSR
	CPS #MODE_SYS
	macroPORT_SAVESPECIFICCONTEXT
	// Switches into SVC mode
	CPS #MODE_SVC

	// Calls scheduler
	BL SCHEDULER

	// Switches into SYS mode
	CPS #MODE_SYS
	// Restores specific context
	macroPORT_RESTORESPECIFICCONTEXT
	CPS #MODE_SVC
	MOV LR, R2
	MSR SPSR_cxfs, R3
	CPS #MODE_SYS
	// Restores general context
	macroPORT_RESTOREGENERALCONTEXT #MODE_SVC
	// Switches into SVC mode
	CPS #MODE_SVC

	// Returns
	MOVS PC, LR
	.endfunc

// Switch service handler
	.align 4
PORT_SVCHANDLER_SWITCH: .func
	// Switches into SYS mode
	CPS #MODE_SYS
	// Restores specific context
	macroPORT_RESTORESPECIFICCONTEXT
	CPS #MODE_SVC
	MOV LR, R2
	MSR SPSR_cxfs, R3
	CPS #MODE_SYS
	// Restores general context
	macroPORT_RESTOREGENERALCONTEXT #MODE_SVC
	// Switches into SVC mode
	CPS #MODE_SVC

	// Returns
	MOVS PC, LR
	.endfunc

// Disable interrupts service handler
	.align 4
PORT_SVCHANDLER_DISABLEINTERRUPTS: .func
	// Pushes temporary registers
	STMFD SP!, {R0}

	// Disables interrupts
	MRS R0, SPSR
	ORR R0, R0, #0xC0
	MSR SPSR_cxfs, R0

	// Pops temporary registers
	LDMFD SP!, {R0}

	// Returns
	MOVS PC, LR
	.endfunc

// Enable interrupts service handler
	.align 4
PORT_SVCHANDLER_ENABLEINTERRUPTS: .func
	// Pushes temporary registers
	STMFD SP!, {R0}

	// Enables interrupts
	MRS R0, SPSR
	BIC R0, R0, #0xC0
	MSR SPSR_cxfs, R0

	// Pops temporary registers
	LDMFD SP!, {R0}

	// Returns
	MOVS PC, LR
	.endfunc

// Get call address - Should return the address from which the current function was called, previously stored by PORT_PREPARECALLADDRESS
	.align 4
	.global PORT_GETCALLADDRESS
PORT_GETCALLADDRESS: .func
	// Pops call address
	LDMFD SP!, {R0}
	// Corrects call address
	SUB R0, R0, #4

	// Returns
	MOV PC, LR
	.endfunc

// Tick method
	.align 4
	.global PORT_TICK
PORT_TICK: .func
	// Calls tick service
	SVC #SERVICE_TICK

	// Exits IRQ handler and keeps context
	B PORT_IRQHANDLER_EXITISR_KEEPCONTEXT
	.endfunc

// Enter core method
	.align 4
	.global PORT_ENTERCORE
PORT_ENTERCORE: .func
	// Enters
	STMFD SP!, {LR}
	// Calls enter core service
	SVC #SERVICE_ENTERCORE
	// Returns
	LDMFD SP!, {PC}
	.endfunc

// Exit core method
	.align 4
	.global PORT_EXITCORE
PORT_EXITCORE: .func
	// Enters
	STMFD SP!, {LR}
	// Calls exit core service
	SVC #SERVICE_EXITCORE
	// Returns
	LDMFD SP!, {PC}
	.endfunc

// Yield method
	.align 4
	.global PORT_YIELD
PORT_YIELD: .func
	// Calls yield service
	SVC #SERVICE_YIELD
	// Enters infinite loop (yield service shouldn't return)
	B PORT_INFINITELOOP
	.endfunc

// Switch method
	.align 4
	.global PORT_SWITCH
PORT_SWITCH: .func
	// Calls switch service
	SVC #SERVICE_SWITCH
	// Enters infinite loop (switch service shouldn't return)
	B PORT_INFINITELOOP
	.endfunc

// Start scheduler method
	.align 4
	.global PORT_STARTSCHEDULER
PORT_STARTSCHEDULER: .func
	// Calls yield service
	SVC #SERVICE_YIELD
	// Enters infinite loop (start scheduler service shouldn't return)
	B PORT_INFINITELOOP
	.endfunc

// Disable interrupts method
	.align 4
	.global PORT_DISABLEINTERRUPTS
PORT_DISABLEINTERRUPTS: .func
	// Enters
	STMFD SP!, {LR}
	// Calls disable interrupts service
	SVC #SERVICE_DISABLEINTERRUPTS
	// Returns
	LDMFD SP!, {PC}
	.endfunc

// Enable interrupts method
	.align 4
	.global PORT_ENABLEINTERRUPTS
PORT_ENABLEINTERRUPTS: .func
	// Enters
	STMFD SP!, {LR}
	// Calls enable interrupts service
	SVC #SERVICE_ENABLEINTERRUPTS
	// Returns
	LDMFD SP!, {PC}
	.endfunc

// Exit privileged mode method
	.align 4
	.global PORT_EXITPRIVILEGEDMODE
PORT_EXITPRIVILEGEDMODE: .func
	// Enters
	STMFD SP!, {LR}
	// Switches into USR mode
	CPS #MODE_USR
	// Returns
	LDMFD SP!, {PC}
	.endfunc

// Infinite loop
	.align 4
	.global PORT_INFINITELOOP
PORT_INFINITELOOP: .func
	// Loops forever
	B PORT_INFINITELOOP
	.endfunc

// Local label addresses
_PORT_SVCHANDLER_TICK: .word PORT_SVCHANDLER_TICK
_PORT_SVCHANDLER_ENTERCORE: .word PORT_SVCHANDLER_ENTERCORE
_PORT_SVCHANDLER_EXITCORE: .word PORT_SVCHANDLER_EXITCORE
_PORT_SVCHANDLER_YIELD: .word PORT_SVCHANDLER_YIELD
_PORT_SVCHANDLER_SWITCH: .word PORT_SVCHANDLER_SWITCH
_PORT_SVCHANDLER_DISABLEINTERRUPTS: .word PORT_SVCHANDLER_DISABLEINTERRUPTS
_PORT_SVCHANDLER_ENABLEINTERRUPTS: .word PORT_SVCHANDLER_ENABLEINTERRUPTS

// Variables and functions addresses
_CORE_CURRENT_CONTEXT: .word CORE_CURRENT_CONTEXT
_PORT_IRQHANDLER_EXITISR_RESTORECONTEXT: .word PORT_IRQHANDLER_EXITISR_RESTORECONTEXT
_PORT_IRQHANDLER_EXITISR_KEEPCONTEXT: .word PORT_IRQHANDLER_EXITISR_KEEPCONTEXT
_PORT_UNDEFINEDINSTRUCTIONERRORHANDLER: .word PORT_UNDEFINEDINSTRUCTIONERRORHANDLER
_INTERRUPT_VECTORTABLE: .word INTERRUPT_VECTORTABLE
_PORT_RELEASECORES: .word PORT_RELEASECORES
_ENTRYPOINT_CORE0: .word ENTRYPOINT_CORE0

// Interrupt controller register constants
ADDR_CONTROLLER_INTERRUPT_NUMBER: .word (XPAR_SCUGIC_0_CPU_BASEADDR + XSCUGIC_INT_ACK_OFFSET)
MASK_CONTROLLER_INTERRUPT_NUMBER: .word XSCUGIC_ACK_INTID_MASK
ADDR_CONTROLLER_INTERRUPT_ACKNOWLEDGE: .word (XPAR_SCUGIC_0_CPU_BASEADDR + XSCUGIC_EOI_OFFSET)

	.end
