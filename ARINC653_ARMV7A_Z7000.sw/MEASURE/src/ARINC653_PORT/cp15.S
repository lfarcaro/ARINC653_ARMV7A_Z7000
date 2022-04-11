	.global CP15CoreIDGet
	.global CP15Ttbr0Set
	.global CP15Ttbr1Set
	.global CP15TlbInvalidate
	.global CP15SetContextID
	.global CP15MMUDisable
	.global CP15MMUEnable
	.global CP15VectorBaseAddrSet
	.global CP15BranchPredictorInvalidate
	.global CP15BranchPredictionEnable
	.global CP15BranchPredictionDisable
	.global CP15DomainAccessClientSet
	.global CP15ControlFeatureDisable
	.global CP15ControlFeatureEnable
	.global CP15TtbCtlConfig

	.text
	.code 32

// Gets core ID
// r0 - Core ID
CP15CoreIDGet:
	MRC p15, 0, R0, c0, c0, 5
	AND R0, R0, #0xF
	BX lr

// Sets TTBR0 Register
// r0 - Translation Table Base Address
CP15Ttbr0Set:
	MCR p15, #0, r0, c2, c0, #0
	DMB
	BX lr

// Sets TTBR1 Register
// r0 - Translation Table Base Address
CP15Ttbr1Set:
	MCR p15, #0, r0, c2, c0, #1
	DMB
	BX lr

// This API Invalidates the TLB
CP15TlbInvalidate:
	MCR p15, #0, r0, c8, c7, #0 // r0 value will be ignored
	DSB
	BX lr

// This API Sets the current context identifier.
// r0 - Context identifier
CP15SetContextID:
	MCR p15, #0, r0, c13, c0, #1
	DSB
	BX lr

// This API Disables MMU.
CP15MMUDisable:
	MCR p15, #0, r0, c8, c7, #0 // Invalidate TLB
	MRC p15, #0, r0, c1, c0, #0
	BIC r0, r0, #1
	MCR p15, #0, r0, c1, c0, #0 // Clear MMU bit
	DSB
	BX lr

// This API Enables MMU.
CP15MMUEnable:
	LDR R0, =0x55555555
	MCR p15, 0, R0, c3, c0, 0 /* Enables permission bits check for all domains */
	DSB
	ISB
	MRC p15, #0, R0, c1, c0, #0
	ORR R0, R0, #0x001
	MCR p15, #0, R0, c1, c0, #0 // Set MMU Enable bit
	DSB
	ISB
	BX lr

// This API sets the interrupt vector table base address
// r0 - Interrput Vector Base Address
CP15VectorBaseAddrSet:
	MCR p15, #0, r0, c12, c0, #0
	DSB
	BX lr

// This API invalidates the branch predictor
CP15BranchPredictorInvalidate:
	MCR p15, #0, r0, c7, c5, #6
	ISB
	BX lr

// This API enables the branch predictor
CP15BranchPredictionEnable:
	MRC p15, #0, r0, c1, c0, #0
	ORR r0, r0, #0x00000800
	MCR p15, #0, r0, c1, c0, #0
	BX lr

// This API disables the branch predictor
CP15BranchPredictionDisable:
	MRC p15, #0, r0, c1, c0, #0
	BIC r0, r0, #0x00000800
	MCR p15, #0, r0, c1, c0, #0
	BX lr

// This API sets the domain access to 'client'
CP15DomainAccessClientSet:
	LDR r0, _CLIENTD
	MCR p15, #0, r0, c3, c0, #0
	DSB
	BX lr

// This API Disables specified features in CP15 control register
// r0 - Features to disable in CP15 control register:
// 	CP15_CONTROL_TEXREMAP - TEX remap flag
// 	CP15_CONTROL_ACCESSFLAG - Access flag Control
// 	CP15_CONTROL_ALIGN_CHCK - Alignment Fault Checking
// 	CP15_CONTROL_MMU - To enable MMU
CP15ControlFeatureDisable:
	MRC p15, #0, r1, c1, c0, #0
	BIC r0, r1, r0
	MCR p15, #0, r0, c1, c0, #0
	DSB
	BX lr

// This API Enables specified features in CP15 control register
// r0 - Features to enable in CP15 control register:
// 	CP15_CONTROL_TEXREMAP - TEX remap flag
// 	CP15_CONTROL_ACCESSFLAG - Access flag Control
// 	CP15_CONTROL_ALIGN_CHCK - Alignment Fault Checking
// 	CP15_CONTROL_MMU - To enable MMU
CP15ControlFeatureEnable:
	MRC p15, #0, r1, c1, c0, #0
	ORR r0, r1, r0
	MCR p15, #0, r0, c1, c0, #0
	DSB
	BX lr

// This API Configures the TTB control register
// r0 - N value
CP15TtbCtlConfig:
	AND r0, r0, #0x7
	MCR p15, #0, r0, c2, c0, #2
	DSB
	BX lr

// Variables and functions addresses
_CLIENTD: .word 0x55555555

	.end
