	// Imports C declarations
	#include "xparameters_ps.h"
	#include "xl2cc.h"
	// References global symbols
	.extern _bss_start
	.extern _bss_end

	.section ".boot"
	.code 32

// Reset handler method
	.align 4
	.global PORT_RESETHANDLER
PORT_RESETHANDLER:
	// Disables interrupts
	CPSID if

	// Reads CPU ID
	MRC p15, 0, R0, c0, c0, 5
	AND R0, R0, #0xF
	CMP R0, #0
PORT_RESETHANDLER_HANGOTHERCPUS:
	BNE PORT_RESETHANDLER_HANGOTHERCPUS

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

	// Clears BSS
	LDR R0, _BSS_START
	LDR R1, _BSS_END
	MOV R2, #0
PORT_RESETHANDLER_CLEARBSSLOOP:
	STR R2, [R0], #4
	CMP R0, R1
	BLT PORT_RESETHANDLER_CLEARBSSLOOP

	// Invalidates the Snoop Control Unit (SCU) registers
	LDR R0, =0xF8F0000C
	LDR R1, =0x0000FFFF
	STR R1, [R0]

	// Disables L2 caches
	LDR R0, =(XPS_L2CC_BASEADDR + XPS_L2CC_CNTRL_OFFSET)
	MOV R1, #0
	STR R1, [R0]
	// Enables all prefetching, cache replacement policy, parity enable, event monitor bus, shared attribute ignored and way size 64 KB
	LDR R0, =(XPS_L2CC_BASEADDR + XPS_L2CC_AUX_CNTRL_OFFSET)
	LDR R1, [R0]
	LDR R2, =0x72760000
	ORR R1, R1, R2
	STR R1, [R0]
	// Sets latencies for the tag
	LDR R0, =(XPS_L2CC_BASEADDR + XPS_L2CC_TAG_RAM_CNTRL_OFFSET)
	LDR R1, =0x0111
	STR R1, [R0]
	// Sets latencies for the data
	LDR R0, =(XPS_L2CC_BASEADDR + XPS_L2CC_DATA_RAM_CNTRL_OFFSET)
	LDR R1, =0x0121
	STR R1, [R0]
	// Forces invalidation
	LDR R0, =(XPS_L2CC_BASEADDR + XPS_L2CC_CACHE_INVLD_WAY_OFFSET)
	LDR R2, =0xFFFF
	STR R2, [R0]
	// Polls synchronization register
	LDR R0, =(XPS_L2CC_BASEADDR + XPS_L2CC_CACHE_SYNC_OFFSET)
PORT_RESETHANDLER_L2CCSYNCLOOP:
	LDR R1, [R0]
	CMP R1, #0
	BNE PORT_RESETHANDLER_L2CCSYNCLOOP
	// Clears pending interrupts
	LDR R0, =(XPS_L2CC_BASEADDR + XPS_L2CC_ISR_OFFSET)
	LDR R1, [R0]
	LDR R0, =(XPS_L2CC_BASEADDR + XPS_L2CC_IAR_OFFSET)
	STR R1, [R0]
	// Unlocks SLCR
	LDR R0, =(XPS_SYS_CTRL_BASEADDR + 0x00000008) /* XPSS_SLCR_UNLOCK_OFFSET */
	LDR R1, =0xDF0D
	STR R1, [R0]
	// Configures the SLCR L2C RAM control register
	LDR R0, =(XPS_SYS_CTRL_BASEADDR + 0x00000A1C) /* XPSS_SLCR_L2C_RAM_OFFSET - See AR# 54190 */
	LDR R1, =0x00020202
	STR R1, [R0]
	// Locks SLCR
	LDR R0, =(XPS_SYS_CTRL_BASEADDR + 0x00000004) /* XPSS_SLCR_LOCK_OFFSET */
	LDR R1, =0x767B
	STR R1, [R0]
	// Enables L2 caches
	LDR R0, =(XPS_L2CC_BASEADDR + XPS_L2CC_CNTRL_OFFSET)
	LDR R1, [R0]
	MOV R2, #0x0001
	ORR R1, R1, R2
	STR R1, [R0]

	// Enables the Snoop Control Unit (SCU)
	LDR R0, =0xF8F00000
	LDR R1, [R0]
	ORR R1, R1, #0x1
	STR R1, [R0]

	// Enables SMP and Cache/TLB maintenance broadcast in ACTLR
	MRC p15, 0, R0, c1, c0, 1
	ORR R0, R0, #0x40 /* SMP */
	ORR R0, R0, #0x01 /* Cache/TLB maintenance broadcast */
	MCR p15, 0, R0, c1, c0, 1

	# Sets other cores' reset handlers and releases them
	LDR R0, =0xFFFFFFF0
	LDR R1, =PORT_RESETHANDLER_CORE1
	STR R1, [R0]
	DSB
	SEV

	# Proceeds to Core #0 reset handler
	B PORT_RESETHANDLER_CORE0

	// Enters infinite loop
	SUB PC, PC, #8

// Variables and functions addresses
_BSS_START: .word _bss_start
_BSS_END: .word _bss_end

	.end
