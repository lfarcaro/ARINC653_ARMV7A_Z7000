// Includes
#include "pmu.h"

// Startup method
void PMU_STARTUP(void) {

	// Disables and configures counters
	// D=1 - Increment PMCCNTR every 64 cycles
	// C=1 - Cycle counter (PMCCNTR) reset
	// P=1 - Event counter (PMXEVCNTRx) reset
	// E=0 - All counters disabled
	PMU_WRITE_PMCR(0x0000000E);

	// Enable access in user mode
	PMU_WRITE_PMUSERENR(0x00000001);

	// Disables all interrupts
	PMU_WRITE_PMINTENCLR(0xFFFFFFFF);

	// Disables all counters
	PMU_WRITE_PMCNTENCLR(0xFFFFFFFF);

	// Clear all overflow statuses
	PMU_WRITE_PMOVSR(0xFFFFFFFF);

	// Check number of counters implemented
	if (((PMU_READ_PMCR() >> 11) & 0x1F) < 1) {
		while (1) {}
	}

	// Enables
	PMU_WRITE_PMCR(PMU_READ_PMCR() | 0x00000001);
}
