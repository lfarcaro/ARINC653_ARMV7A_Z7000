// Module include
#include "module2_module.h"

// Configuration include
#include "configuration.h"

// SNIPPET_START GLOBAL_INCLUDES
// Test include
#include "test.h"
// SNIPPET_END

// -------------------- MODULE DESCRIPTION START --------------------
// SNIPPET_START MODULE2_MODULE_DESCRIPTION
// SNIPPET_END
// -------------------- MODULE DESCRIPTION END --------------------

// SNIPPET_START MODULE2_MODULE_INCLUDES
// SNIPPET_END

// Core #1 entry point
void (*ENTRYPOINT_CORE1)(void) = MODULE2_MODULE_DEFAULTPARTITION;

// Core #1 module configuration
MODULE_CONFIGURATION_TYPE *MODULE_CONFIGURATION_CORE1 = &MODULE2_MODULE_CONFIGURATION;

// PARTITION2 partition attributes
static PARTITION_ATTRIBUTE_TYPE PARTITION2_PARTITION_ATTRIBUTE = {
/*NAME*/(PARTITION_NAME_TYPE) "PARTITION2",
/*ENTRY_POINT*/(SYSTEM_ADDRESS_TYPE) &PARTITION2_DEFAULTPROCESS,
/*STACK_SIZE*/(STACK_SIZE_TYPE) 256,
/*SYSTEMPARTITION*/(BOOLEAN_TYPE) false };

// PARTITION2 partition identifier
static PARTITION_ID_TYPE PARTITION2_PARTITION_ID;

// SNIPPET_START MODULE2_MODULE_GLOBAL_DEFINITIONS
// SNIPPET_END

// SNIPPET_START MODULE2_MODULE_GLOBAL_VARIABLES
// SNIPPET_END

// SNIPPET_START MODULE2_MODULE_FUNCTIONS
// SNIPPET_END

// MODULE2 module default partition
void MODULE2_MODULE_DEFAULTPARTITION(void) {
	RETURN_CODE_TYPE RETURN_CODE;
	// SNIPPET_START MODULE2_MODULE_DEFAULTPARTITION_VARIABLES
	// SNIPPET_END

	// Starts module up
	STARTUP_MODULE(&RETURN_CODE);
	if (RETURN_CODE != NO_ERROR) {
		// SNIPPET_START STARTUP_MODULE_ERROR_HANDLING_CODE
		// SNIPPET_END
	}

	// Creates PARTITION2 partition
	CREATE_PARTITION(&PARTITION2_PARTITION_ATTRIBUTE, &PARTITION2_PARTITION_ID, &RETURN_CODE);
	if (RETURN_CODE != NO_ERROR) {
		// SNIPPET_START CREATE_PARTITION_ERROR_HANDLING_CODE
		// SNIPPET_END
	}

	// SNIPPET_START MODULE2_MODULE_DEFAULTPARTITION_INITIALIZATION
	// SNIPPET_END

	// Sets module mode
	SET_MODULE_MODE(NORMAL, &RETURN_CODE);
	if (RETURN_CODE != NO_ERROR) {
		// SNIPPET_START SET_MODULE_MODE_ERROR_HANDLING_CODE
		// SNIPPET_END
	}

	// Start of idle partition

	// SNIPPET_START MODULE2_MODULE_IDLEPARTITION_INITIALIZATION
	// SNIPPET_END

	// Main loop
	while (true) {

		// SNIPPET_START MODULE2_MODULE_IDLEPARTITION_CODE
		// Sets LEDs pattern
		TEST_LED2_OFF();
		// SNIPPET_END
	}
}
