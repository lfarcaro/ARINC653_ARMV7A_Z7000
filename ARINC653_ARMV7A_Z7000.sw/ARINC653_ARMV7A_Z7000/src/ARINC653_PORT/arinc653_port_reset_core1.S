	// Imports C declarations
	#include "xparameters_ps.h"
	#include "xl2cc.h"
	// References global symbols
	.extern PORT_VECTORTABLE_CORE1
	.extern _core1_module_stack_start
	.extern _core1_module_stack_end
	.extern PORT_RELEASECORES
	.extern ENTRYPOINT_CORE1

// Mode constants
.set MODE_USR, 0x10
.set MODE_FIQ, 0x11
.set MODE_IRQ, 0x12
.set MODE_SVC, 0x13
.set MODE_ABT, 0x17
.set MODE_UND, 0x1B
.set MODE_SYS, 0x1F

// Stack sizes for different modes
.set MODE_STACKSIZE_IRQ, 0xC00
.set MODE_STACKSIZE_SVC, 0xC00
.set MODE_STACKSIZE_ABT, 0xC00
.set MODE_STACKSIZE_UND, 0xC00
.set MODE_STACKSIZE_SYSFIQUSR, 0x1000

	.section ".boot"
	.code 32

// Reset handler method
	.align 4
	.global PORT_RESETHANDLER_CORE1
PORT_RESETHANDLER_CORE1:
	// Disables interrupts
	CPSID if

	// Reads CPU ID
	MRC p15, 0, R0, c0, c0, 5
	AND R0, R0, #0xF
	CMP R0, #1
PORT_RESETHANDLER_CORE1_HANGOTHERCPUS:
	BNE PORT_RESETHANDLER_CORE1_HANGOTHERCPUS

	// Reads core revision
	MRC p15, 0, R0, c0, c0, 0
	AND R1, R0, #0x00f00000
	AND R2, R0, #0x0000000f
	ORR R0, R2, R1, LSR #20-4

	// Handles ARM errata 742230 for devices up to r2p2
	CMP R0, #0x22
	MRCLE p15, 0, R1, c15, c0, 1
	ORRLE R1, R1, #1 << 4
	MCRLE p15, 0, R1, c15, c0, 1

	// Handles ARM errata 743622 for devices r2b*
	TEQ R0, #0x00200000
	MRCEQ p15, 0, R1, c15, c0, 1
	ORREQ R1, R1, #1 << 6
	MCREQ p15, 0, R1, c15, c0, 1

	// Disables data cache
	MRC p15, 0, R0, c1, c0, 0
	BIC R0, R0, #0x0004
	MCR p15, 0, R0, c1, c0, 0
	DSB

	// Disables instruction cache
	MRC p15, 0, R0, c1, c0, 0
	BIC R0, R0, #0x1000
	MCR p15, 0, R0, c1, c0, 0
	ISB

	// Disables branch prediction
	MRC p15, 0, R0, c1, c0, 0
	BIC R0, R0, #0x0800
	MCR p15, 0, R0, c1, c0, 0
	ISB

	// Disables MMU
	MRC	p15, 0, R0, c1, c0, 0
	BIC	R0, R0, #0x1
	MCR	p15, 0, R0, c1, c0, 0

	// Temporarily sets the vector table address to the DDR location
	LDR	R0, =_PORT_VECTORTABLE_CORE1
	LDR R0, [R0]
	MCR	p15, 0, R0, c12, c0, 0

	// Invalidates TLBs
	MOV R0, #0
	MCR p15, 0, R0, c8, c7, 0 /* Invalidates TLBs */
	MCR p15, 0, R0, c7, c5, 0 /* Invalidates instruction cache */
	MCR p15, 0, R0, c7, c5, 6 /* Invalidates branch predictor array */

	// Invalidates data cache
	MRC p15, 1, R0, c0, c0, 1 /* Reads CLIDR */
	ANDS R3, R0, #0x7000000
	MOV R3, R3, lsr #23 /* Gets cache level */
	BEQ PORT_RESETHANDLER_CORE1_INVALIDATEDCACHE_EXIT
	// Iterates caches, indexes and ways
	MOV R10, #0
PORT_RESETHANDLER_CORE1_INVALIDATEDCACHE_CACHELOOP:
	ADD R2, R10, R10, lsr #1
	MOV R1, R0, lsr R2
	AND R1, R1, #7 /* Masks cache type */
	CMP R1, #2
	BLT PORT_RESETHANDLER_CORE1_INVALIDATEDCACHE_SKIP /* No cache or only instruction caches exist at this level */
	MCR p15, 2, R10, c0, c0, 0 /* Selects cache size */
	ISB
	MRC p15, 1, R1, c0, c0, 0 /* Reads cache size ID */
	AND R2, R1, #7 /* Masks line length */
	ADD R2, R2, #4 /* Increases the line length offset by log2(16 bytes) */
	LDR R4, =0x3ff
	ANDS R4, R4, R1, lsr #3 /* Reads the maximum number of the way size */
	CLZ R5, R4 /* Reads the bit position of the way size increment */
	LDR R7, =0x7fff
	ANDS R7, R7, R1, lsr #13 /* Reads the maximum number of the index size */
PORT_RESETHANDLER_CORE1_INVALIDATEDCACHE_INDEXLOOP:
	MOV R9, R4
PORT_RESETHANDLER_CORE1_INVALIDATEDCACHE_WAYLOOP:
	ORR R11, R10, R9, lsl R5 /* Combines way and cache numbers */
	ORR R11, R11, R7, lsl R2 /* Combines the index number */
	MCR p15, 0, R11, c7, c6, 2 /* Invalidates set/way */
	SUBS R9, R9, #1 /* Decrements way number */
	BGE PORT_RESETHANDLER_CORE1_INVALIDATEDCACHE_WAYLOOP
	SUBS R7, R7, #1 /* Decrements index */
	BGE PORT_RESETHANDLER_CORE1_INVALIDATEDCACHE_INDEXLOOP
PORT_RESETHANDLER_CORE1_INVALIDATEDCACHE_SKIP:
	ADD R10, R10, #2 /* Increments cache number */
	CMP R3, R10
	BGT PORT_RESETHANDLER_CORE1_INVALIDATEDCACHE_CACHELOOP
PORT_RESETHANDLER_CORE1_INVALIDATEDCACHE_EXIT:
	MOV R10, #0
	MCR p15, 2, R10, c0, c0, 0 /* Switches to cache level 0 */
	DSB
	ISB

	// Reads stack pointer
	LDR R0, _STACK_TOP_CORE1
	// Sets up stack for undefined mode
	CPS #MODE_UND
	MOV SP, R0
	MRS R1, CPSR
	BIC R1, R1, #0x200 /* Little endian */
	MSR SPSR_fsxc, R1
	SUB R0, R0, #MODE_STACKSIZE_UND
	// Sets up stack for abort mode
	CPS #MODE_ABT
	MOV SP, R0
	MRS R1, CPSR
	BIC R1, R1, #0x200 /* Little endian */
	MSR SPSR_fsxc, R1
	SUB R0, R0, #MODE_STACKSIZE_ABT
	// Sets up stack for IRQ mode
	CPS #MODE_IRQ
	MOV SP, R0
	MRS R1, CPSR
	BIC R1, R1, #0x200 /* Little endian */
	MSR SPSR_fsxc, R1
	SUB R0, R0, #MODE_STACKSIZE_IRQ
	// Sets up stack for SVC mode
	CPS #MODE_SVC
	MOV SP, R0
	MRS R1, CPSR
	BIC R1, R1, #0x200 /* Little endian */
	MSR SPSR_fsxc, R1
	SUB R0, R0, #MODE_STACKSIZE_SVC
	// Sets up stack for SYS/FIQ/USR mode
	CPS #MODE_SYS
	MOV SP, R0
	MRS R1, CPSR
	BIC R1, R1, #0x200 /* Little endian */
	MSR SPSR_fsxc, R1
	SUB R0, R0, #MODE_STACKSIZE_SYSFIQUSR
	// Verifies stack bottom
	LDR R1, _STACK_BOTTOM_CORE1
	CMP R0, R1
PORT_RESETHANDLER_CORE1_STACKERRORLOOP:
	BNE PORT_RESETHANDLER_CORE1_STACKERRORLOOP

	// Enables NEON/VFP coprocessor
	MRC P15, #0, R0, C1, C0, #2 // R0 = Coprocessor Access Control Register (CACR)
	ORR R0, R0, #(0xF << 20) // Enables full access for CP10 and CP11
	MCR P15, #0, R0, C1, C0, #2 // Access control register = R0
	MOV R0, #0
	MCR P15, #0, R0, C7, C5, #4 // Flushes prefetch buffer
	FMRX R0, FPEXC
	ORR R0, R0, #0x40000000
	FMXR FPEXC, R0 // Enables NEON/VFP

	// Enables branch prediction
	MRC p15, 0, R0, c1, c0, 0
	ORR R0, R0, #0x0800
	MCR p15, 0, R0, c1, c0, 0
	ISB

	// Enables instruction cache
	MRC p15, 0, R0, c1, c0, 0
	ORR R0, R0, #0x1000
	MCR p15, 0, R0, c1, c0, 0
	ISB

	// Enables data cache
	MRC p15, 0, R0, c1, c0, 0
	ORR R0, R0, #0x0004
	MCR p15, 0, R0, c1, c0, 0
	DSB

	// Enables Dside prefetch and L2 prefetch hint
	MRC p15, 0, R0, c1, c0, 1
	ORR R0, R0, #0x6
	MCR p15, 0, R0, c1, c0, 1

	// Enables asynchronous abort exception
	MRS R0, CPSR
	BIC R0, R0, #0x100
	MSR CPSR_xsf, R0

	// Initializes CP15 registers
	MOV R0, #0
	MCR p15, #0, R0, c6, c0, #0 // DFAR
	MCR p15, #0, R0, c5, c0, #0 // DFSR
	MCR p15, #0, R0, c6, c0, #2 // IFAR
	MCR p15, #0, R0, c5, c0, #1 // IFSR
	MCR p15, #0, R0, c9, c13, #2 // PMXEVCNTR
	MCR p15, #0, R0, c13, c0, #2 // TPIDRURW
	MCR p15, #0, R0, c13, c0, #3// TPIDRURO

PORT_RESETHANDLER_CORE1_WAITFORRELEASE:
	// All but Core #0 wait to be released by Core #0 after system startup
	LDR R0, =_PORT_RELEASECORES
	LDR R0, [R0]
	LDR R0, [R0]
	CMP R0, #0
	BNE PORT_RESETHANDLER_CORE1_RELEASED
	DSB
	WFE
	B PORT_RESETHANDLER_CORE1_WAITFORRELEASE

PORT_RESETHANDLER_CORE1_RELEASED:

	// When released by Core #0 after system
	// startup, Core #1 proceeds to its entry point
	LDR R0, =_ENTRYPOINT_CORE1
	LDR R0, [R0]
	LDR R0, [R0]
	BX R0

	// Enters infinite loop
	SUB PC, PC, #8

// Variables and functions addresses
_PORT_VECTORTABLE_CORE1: .word PORT_VECTORTABLE_CORE1
_STACK_TOP_CORE1: .word _core1_module_stack_start
_STACK_BOTTOM_CORE1: .word _core1_module_stack_end
_PORT_RELEASECORES: .word PORT_RELEASECORES
_ENTRYPOINT_CORE1: .word ENTRYPOINT_CORE1

	.end
