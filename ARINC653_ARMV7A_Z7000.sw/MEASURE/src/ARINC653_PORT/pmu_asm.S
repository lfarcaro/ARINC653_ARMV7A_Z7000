// PMCCNTR read method
	.align 4
	.global PMU_READ_PMCCNTR
PMU_READ_PMCCNTR: .func
	MRC p15, 0, R0, c9, c13, 0 /* Read PMCCNTR */
	BX LR
	.endfunc

// PMCCNTR write method
	.align 4
	.global PMU_WRITE_PMCCNTR
PMU_WRITE_PMCCNTR: .func
	MCR p15, 0, R0, c9, c13, 0 /* Write PMCCNTR */
	ISB
	BX LR
	.endfunc

// PMCEID0 read method
	.align 4
	.global PMU_READ_PMCEID0
PMU_READ_PMCEID0: .func
	MRC p15, 0, R0, c9, c12, 6 /* Read PMCEID0 */
	BX LR
	.endfunc

// PMCEID1 read method
	.align 4
	.global PMU_READ_PMCEID1
PMU_READ_PMCEID1: .func
	MRC p15, 0, R0, c9, c12, 7 /* Read PMCEID1 */
	BX LR
	.endfunc

// PMCNTENCLR read method
	.align 4
	.global PMU_READ_PMCNTENCLR
PMU_READ_PMCNTENCLR: .func
	MRC p15, 0, R0, c9, c12, 2 /* Read PMCNTENCLR */
	BX LR
	.endfunc

// PMCNTENCLR write method
	.align 4
	.global PMU_WRITE_PMCNTENCLR
PMU_WRITE_PMCNTENCLR: .func
	MCR p15, 0, R0, c9, c12, 2 /* Write PMCNTENCLR */
	ISB
	BX LR
	.endfunc

// PMCNTENSET read method
	.align 4
	.global PMU_READ_PMCNTENSET
PMU_READ_PMCNTENSET: .func
	MRC p15, 0, R0, c9, c12, 1 /* Read PMCNTENSET */
	BX LR
	.endfunc

// PMCNTENSET write method
	.align 4
	.global PMU_WRITE_PMCNTENSET
PMU_WRITE_PMCNTENSET: .func
	MCR p15, 0, R0, c9, c12, 1 /* Write PMCNTENSET */
	ISB
	BX LR
	.endfunc

// PMCR read method
	.align 4
	.global PMU_READ_PMCR
PMU_READ_PMCR: .func
	MRC p15, 0, R0, c9, c12, 0 /* Read PMCR */
	BX LR
	.endfunc

// PMCR write method
	.align 4
	.global PMU_WRITE_PMCR
PMU_WRITE_PMCR: .func
	MCR p15, 0, R0, c9, c12, 0 /* Write PMCR */
	ISB
	BX LR
	.endfunc

// PMINTENCLR read method
	.align 4
	.global PMU_READ_PMINTENCLR
PMU_READ_PMINTENCLR: .func
	MRC p15, 0, R0, c9, c14, 2 /* Read PMINTENCLR */
	BX LR
	.endfunc

// PMINTENCLR write method
	.align 4
	.global PMU_WRITE_PMINTENCLR
PMU_WRITE_PMINTENCLR: .func
	MCR p15, 0, R0, c9, c14, 2 /* Write PMINTENCLR */
	ISB
	BX LR
	.endfunc

// PMINTENSET read method
	.align 4
	.global PMU_READ_PMINTENSET
PMU_READ_PMINTENSET: .func
	MRC p15, 0, R0, c9, c14, 1 /* Read PMINTENSET */
	BX LR
	.endfunc

// PMINTENSET write method
	.align 4
	.global PMU_WRITE_PMINTENSET
PMU_WRITE_PMINTENSET: .func
	MCR p15, 0, R0, c9, c14, 1 /* Write PMINTENSET */
	ISB
	BX LR
	.endfunc

// PMOVSR read method
	.align 4
	.global PMU_READ_PMOVSR
PMU_READ_PMOVSR: .func
	MRC p15, 0, R0, c9, c12, 3 /* Read PMOVSR */
	BX LR
	.endfunc

// PMOVSR write method
	.align 4
	.global PMU_WRITE_PMOVSR
PMU_WRITE_PMOVSR: .func
	MCR p15, 0, R0, c9, c12, 3 /* Write PMOVSR */
	ISB
	BX LR
	.endfunc

// PMOVSSET read method
	.align 4
	.global PMU_READ_PMOVSSET
PMU_READ_PMOVSSET: .func
	MRC p15, 0, R0, c9, c14, 3 /* Read PMOVSSET */
	BX LR
	.endfunc

// PMOVSSET write method
	.align 4
	.global PMU_WRITE_PMOVSSET
PMU_WRITE_PMOVSSET: .func
	MCR p15, 0, R0, c9, c14, 3 /* Write PMOVSSET */
	ISB
	BX LR
	.endfunc

// PMSWINC write method
	.align 4
	.global PMU_WRITE_PMSWINC
PMU_WRITE_PMSWINC: .func
	MCR p15, 0, R0, c9, c12, 4 /* Write PMSWINC */
	ISB
	BX LR
	.endfunc

// PMUSERENR read method
	.align 4
	.global PMU_READ_PMUSERENR
PMU_READ_PMUSERENR: .func
	MRC p15, 0, R0, c9, c14, 0 /* Read PMUSERENR */
	BX LR
	.endfunc

// PMUSERENR write method
	.align 4
	.global PMU_WRITE_PMUSERENR
PMU_WRITE_PMUSERENR: .func
	MCR p15, 0, R0, c9, c14, 0 /* Write PMUSERENR */
	ISB
	BX LR
	.endfunc

// PMXEVCNTR read method
	.align 4
	.global PMU_READ_PMXEVCNTR
PMU_READ_PMXEVCNTR: .func
	MCR p15, 0, R0, c9, c12, 5 /* Write PMSELR */
	ISB
	MRC p15, 0, R0, c9, c13, 2 /* Read PMXEVCNTR */
	BX LR
	.endfunc

// PMXEVCNTR write method
	.align 4
	.global PMU_WRITE_PMXEVCNTR
PMU_WRITE_PMXEVCNTR: .func
	MCR p15, 0, R0, c9, c12, 5 /* Write PMSELR */
	ISB
	MCR p15, 0, R1, c9, c13, 2 /* Write PMXEVCNTR */
	ISB
	BX LR
	.endfunc

// PMXEVTYPER read method
	.align 4
	.global PMU_READ_PMXEVTYPER
PMU_READ_PMXEVTYPER: .func
	MCR p15, 0, R0, c9, c12, 5 /* Write PMSELR */
	ISB
	MRC p15, 0, R0, c9, c13, 1 /* Read PMXEVTYPER */
	BX LR
	.endfunc

// PMXEVTYPER write method
	.align 4
	.global PMU_WRITE_PMXEVTYPER
PMU_WRITE_PMXEVTYPER: .func
	MCR p15, 0, R0, c9, c12, 5 /* Write PMSELR */
	ISB
	MCR p15, 0, R1, c9, c13, 1 /* Write PMXEVTYPER */
	ISB
	BX LR
	.endfunc
