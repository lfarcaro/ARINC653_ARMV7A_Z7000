// Partition include
#include "partition1_partition.h"

// SNIPPET_START GLOBAL_INCLUDES
// Test include
#include "test.h"
#include "xscutimer.h"

#include "../Algorithms/algorithm_bsort1.h"
#include "../Algorithms/algorithm_dijkstra1.h"
#include "../Algorithms/algorithm_fibcall1.h"
#include "../Algorithms/algorithm_lms1.h"
// SNIPPET_END

// -------------------- PARTITION DESCRIPTION START --------------------
// SNIPPET_START PARTITION1_PARTITION_DESCRIPTION
// SNIPPET_END
// -------------------- PARTITION DESCRIPTION END --------------------

// SNIPPET_START PARTITION1_PARTITION_INCLUDES
// SNIPPET_END

// SNIPPET_START PARTITION1_PARTITION_GLOBAL_DEFINITIONS
// SNIPPET_END

// SNIPPET_START PARTITION1_PARTITION_GLOBAL_VARIABLES
// XScuTimer configuration data pointer
static XScuTimer_Config *XScuTimerConfigPtr;

// XScuTimer instance data
static XScuTimer XScuTimerInst;
// SNIPPET_END

// SNIPPET_START PARTITION1_PARTITION_FUNCTIONS
// SNIPPET_END

// PARTITION1 partition default process
void PARTITION1_DEFAULTPROCESS(void) {
	RETURN_CODE_TYPE RETURN_CODE;
	// SNIPPET_START PARTITION1_PARTITION_DEFAULTPROCESS_VARIABLES
	unsigned int VALUE;
	unsigned int COUNT1, COUNT2;
	// SNIPPET_END

	// SNIPPET_START PARTITION1_PARTITION_DEFAULTPROCESS_INITIALIZATION
	// SNIPPET_END

	// Sets partition mode
	SET_PARTITION_MODE(NORMAL, &RETURN_CODE);
	if (RETURN_CODE != NO_ERROR) {
		// SNIPPET_START SET_PARTITION_MODE_ERROR_HANDLING_CODE
		while (true) {
		}
		// SNIPPET_END
	}

	// Start of idle process

	// SNIPPET_START PARTITION1_PARTITION_IDLEPROCESS_INITIALIZATION
	// Looks up the private timer's configuration entry
	XScuTimerConfigPtr = XScuTimer_LookupConfig(XPAR_XSCUTIMER_0_DEVICE_ID);
	if (XScuTimerConfigPtr == NULL) {
		while (true) {}
	}

	// Initializes the timer's configuration
	if (XScuTimer_CfgInitialize(&XScuTimerInst, XScuTimerConfigPtr, XScuTimerConfigPtr->BaseAddr) != XST_SUCCESS) {

		// Attempts stopping the device
		XScuTimer_Stop(&XScuTimerInst);

		// Retries initializing the timer's configuration
		if (XScuTimer_CfgInitialize(&XScuTimerInst, XScuTimerConfigPtr, XScuTimerConfigPtr->BaseAddr) != XST_SUCCESS) {
			while (true) {}
		}
	}

	// Sets prescaler
	XScuTimer_SetPrescaler(&XScuTimerInst, 1);

	// Configures event type - From ARM DDI 0406C.d (ID040418) pages B6-1896 and D3-2364
	PMU_WRITE_PMXEVTYPER(0, 0x00000006);
	PMU_WRITE_PMXEVTYPER(1, 0x00000007);
	// SNIPPET_END

	// Main loop
	while (true) {

		// SNIPPET_START PARTITION1_PARTITION_IDLEPROCESS_CODE
		// Loads the timer's counter register
		XScuTimer_LoadTimer(&XScuTimerInst, 0xFFFFFFFFu);

		// Initializes algorithm
		ALGORITHM_BSORT1_INITIALIZE();
		ALGORITHM_DIJKSTRA1_INITIALIZE();
		ALGORITHM_FIBCALL1_INITIALIZE();
		ALGORITHM_LMS1_INITIALIZE();

		// Starts the timer
		//XScuTimer_Start(&XScuTimerInst);

		// Resets PMCCNTR and all event counters
		PMU_WRITE_PMCR(PMU_READ_PMCR() | 0x00000006);

		// Enables event counters
		PMU_WRITE_PMCNTENSET(0x80000003); // Enables PMCCNTR and PMXEVCNTR[0-1]

		// Executes algorithm
		ALGORITHM_BSORT1_EXECUTE();
		ALGORITHM_DIJKSTRA1_EXECUTE();
		ALGORITHM_FIBCALL1_EXECUTE();
		ALGORITHM_LMS1_EXECUTE();

		// Disables event counters
		PMU_WRITE_PMCNTENCLR(0x80000003); // Disables PMCCNTR and PMXEVCNTR[0-1]

		// Stops the timer
		//XScuTimer_Stop(&XScuTimerInst);

		// Reads event counter
		VALUE = PMU_READ_PMCCNTR(); // Reads cycle counter
		COUNT1 = PMU_READ_PMXEVCNTR(0); // Reads event counter
		COUNT2 = PMU_READ_PMXEVCNTR(1); // Reads event counter

		// Reads timer value
		//VALUE = 0xFFFFFFFFu - XScuTimer_GetCounterValue(&XScuTimerInst);

		// Prints value
		CONSOLE_PRINTF(0, "%d %d %d\n", VALUE, COUNT1, COUNT2);
		// SNIPPET_END
	}
}
