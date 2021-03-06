// ARINC653 includes
#include "arinc653_core.h"

// Partition sampling port identifier getter
void GET_PARTITION_SAMPLING_PORT_ID(PARTITION_ID_TYPE PARTITION_ID, SAMPLING_PORT_NAME_TYPE SAMPLING_PORT_NAME, SAMPLING_PORT_ID_TYPE *SAMPLING_PORT_ID, RETURN_CODE_TYPE *RETURN_CODE) {
	MODULE_INFORMATION_TYPE *MODULE_INFORMATION;
	PARTITION_INFORMATION_TYPE *PARTITION_INFORMATION;
	SAMPLING_PORT_ID_TYPE SAMPLINGPORT_IDENTIFIER;

	// Enters core
	ENTER_CORE();

	// Gets module information
	MODULE_INFORMATION = _CORE_MODULE_INFORMATION;

	// Verifies partition identifier
	if (PARTITION_ID >= MODULE_INFORMATION->PARTITION_COUNT) {

		// Sets return code
		*RETURN_CODE = INVALID_PARAM;

		// Exits core
		EXIT_CORE();
		return;
	}

	// Gets partition information
	PARTITION_INFORMATION = &MODULE_INFORMATION->PARTITION_INFORMATION[PARTITION_ID];

	// Iterates sampling ports
	for (SAMPLINGPORT_IDENTIFIER = 0; SAMPLINGPORT_IDENTIFIER < PARTITION_INFORMATION->SAMPLINGPORT_COUNT; SAMPLINGPORT_IDENTIFIER++) {

		// Compares names
		if (COMMON_COMPARESTRINGS(SAMPLING_PORT_NAME, PARTITION_INFORMATION->SAMPLINGPORT_INFORMATION[SAMPLINGPORT_IDENTIFIER].NAME, MAX_NAME_LENGTH)) {

			// Sets sampling port identifier
			*SAMPLING_PORT_ID = SAMPLINGPORT_IDENTIFIER;

			// Sets return code
			*RETURN_CODE = NO_ERROR;

			// Exits core
			EXIT_CORE();
			return;
		}
	}

	// Sets return code
	*RETURN_CODE = INVALID_CONFIG;

	// Exits core
	EXIT_CORE();
}

// Write partition sampling message method
void WRITE_PARTITION_SAMPLING_MESSAGE(PARTITION_ID_TYPE PARTITION_ID, SAMPLING_PORT_ID_TYPE SAMPLING_PORT_ID, MESSAGE_ADDR_TYPE MESSAGE_ADDR, MESSAGE_SIZE_TYPE LENGTH, RETURN_CODE_TYPE *RETURN_CODE) {
	MODULE_INFORMATION_TYPE *MODULE_INFORMATION;
	PARTITION_INFORMATION_TYPE *PARTITION_INFORMATION;
	SAMPLINGPORT_INFORMATION_TYPE *SAMPLINGPORT_INFORMATION;
	INDEX_TYPE CHARACTER_INDEX;

	// Enters core
	ENTER_CORE();

	// Gets module information
	MODULE_INFORMATION = _CORE_MODULE_INFORMATION;

	// Verifies partition identifier
	if (PARTITION_ID >= MODULE_INFORMATION->PARTITION_COUNT) {

		// Sets return code
		*RETURN_CODE = INVALID_PARAM;

		// Exits core
		EXIT_CORE();
		return;
	}

	// Gets partition information
	PARTITION_INFORMATION = &MODULE_INFORMATION->PARTITION_INFORMATION[PARTITION_ID];

	// Verifies sampling port identifier
	if (SAMPLING_PORT_ID >= PARTITION_INFORMATION->SAMPLINGPORT_COUNT) {

		// Sets return code
		*RETURN_CODE = INVALID_PARAM;

		// Exits core
		EXIT_CORE();
		return;
	}

	// Gets sampling port information
	SAMPLINGPORT_INFORMATION = &PARTITION_INFORMATION->SAMPLINGPORT_INFORMATION[SAMPLING_PORT_ID];

	// Verifies length
	if (LENGTH > SAMPLINGPORT_INFORMATION->MAX_MESSAGE_SIZE) {

		// Sets return code
		*RETURN_CODE = INVALID_CONFIG;

		// Exits core
		EXIT_CORE();
		return;
	}

	// Verifies port direction
	if (SAMPLINGPORT_INFORMATION->PORT_DIRECTION != DESTINATION) {

		// Sets return code
		*RETURN_CODE = INVALID_MODE;

		// Exits core
		EXIT_CORE();
		return;
	}

	// Sets sampling port empty indicator
	SAMPLINGPORT_INFORMATION->EMPTY_INDICATOR = OCCUPIED;

	// Overwrites sampling port's message with the given message
	for (CHARACTER_INDEX = 0; CHARACTER_INDEX < LENGTH; CHARACTER_INDEX++) {
		SAMPLINGPORT_INFORMATION->MESSAGE[CHARACTER_INDEX] = MESSAGE_ADDR[CHARACTER_INDEX];
	}
	SAMPLINGPORT_INFORMATION->LENGTH = LENGTH;

	// Sets last write time
	SAMPLINGPORT_INFORMATION->LAST_WRITE = CLOCK_GETSYSTEMTIME();

	// Sets return code
	*RETURN_CODE = NO_ERROR;

	// Exits core
	EXIT_CORE();
}

// Read partition sampling message method
void READ_PARTITION_SAMPLING_MESSAGE(PARTITION_ID_TYPE PARTITION_ID, SAMPLING_PORT_ID_TYPE SAMPLING_PORT_ID, MESSAGE_ADDR_TYPE MESSAGE_ADDR, MESSAGE_SIZE_TYPE *LENGTH, BOOLEAN_TYPE PEEK, RETURN_CODE_TYPE *RETURN_CODE) {
	MODULE_INFORMATION_TYPE *MODULE_INFORMATION;
	PARTITION_INFORMATION_TYPE *PARTITION_INFORMATION;
	SAMPLINGPORT_INFORMATION_TYPE *SAMPLINGPORT_INFORMATION;
	INDEX_TYPE CHARACTER_INDEX;

	// Enters core
	ENTER_CORE();

	// Gets module information
	MODULE_INFORMATION = _CORE_MODULE_INFORMATION;

	// Verifies partition identifier
	if (PARTITION_ID >= MODULE_INFORMATION->PARTITION_COUNT) {

		// Sets return code
		*RETURN_CODE = INVALID_PARAM;

		// Exits core
		EXIT_CORE();
		return;
	}

	// Gets partition information
	PARTITION_INFORMATION = &MODULE_INFORMATION->PARTITION_INFORMATION[PARTITION_ID];

	// Verifies sampling port identifier
	if (SAMPLING_PORT_ID >= PARTITION_INFORMATION->SAMPLINGPORT_COUNT) {

		// Sets return code
		*RETURN_CODE = INVALID_PARAM;

		// Exits core
		EXIT_CORE();
		return;
	}

	// Gets sampling port information
	SAMPLINGPORT_INFORMATION = &PARTITION_INFORMATION->SAMPLINGPORT_INFORMATION[SAMPLING_PORT_ID];

	// Verifies port direction
	if (SAMPLINGPORT_INFORMATION->PORT_DIRECTION != SOURCE) {

		// Sets return code
		*RETURN_CODE = INVALID_MODE;

		// Exits core
		EXIT_CORE();
		return;
	}

	// Verifies sampling port emptiness indicator
	if (SAMPLINGPORT_INFORMATION->EMPTY_INDICATOR == EMPTY) {

		// Sets return code
		*RETURN_CODE = NOT_AVAILABLE;

		// Exits core
		EXIT_CORE();
		return;
	}

	// Verifies sampling port update
	if ((SAMPLINGPORT_INFORMATION->LAST_WRITE != INFINITE_TIME_VALUE) && (SAMPLINGPORT_INFORMATION->LAST_READ != INFINITE_TIME_VALUE) && (SAMPLINGPORT_INFORMATION->LAST_WRITE <= SAMPLINGPORT_INFORMATION->LAST_READ)) {

		// Sets return code
		*RETURN_CODE = NO_ACTION;

		// Exits core
		EXIT_CORE();
		return;
	}

	// Copies sampling port's message
	for (CHARACTER_INDEX = 0; CHARACTER_INDEX < SAMPLINGPORT_INFORMATION->LENGTH; CHARACTER_INDEX++) {
		MESSAGE_ADDR[CHARACTER_INDEX] = SAMPLINGPORT_INFORMATION->MESSAGE[CHARACTER_INDEX];
	}

	// Sets length
	*LENGTH = SAMPLINGPORT_INFORMATION->LENGTH;

	// Verifies peek flag
	if (!PEEK) {

		// Sets last read time
		SAMPLINGPORT_INFORMATION->LAST_READ = CLOCK_GETSYSTEMTIME();
	}

	// Sets return code
	*RETURN_CODE = NO_ERROR;

	// Exits core
	EXIT_CORE();
}

// Create sampling port method
void CREATE_SAMPLING_PORT(SAMPLING_PORT_NAME_TYPE SAMPLING_PORT_NAME, MESSAGE_SIZE_TYPE MAX_MESSAGE_SIZE, PORT_DIRECTION_TYPE PORT_DIRECTION, SYSTEM_TIME_TYPE REFRESH_PERIOD, SAMPLING_PORT_ID_TYPE *SAMPLING_PORT_ID, RETURN_CODE_TYPE *RETURN_CODE) {
	MODULE_INFORMATION_TYPE *MODULE_INFORMATION;
	SAMPLING_PORT_ID_TYPE SAMPLING_PORT_IDENTIFIER;
	SAMPLINGPORT_INFORMATION_TYPE *SAMPLINGPORT_INFORMATION;

	// The following validations are not performed, assuming that the code is generated based on a valid configuration file:

	// 1) If no sampling port of the partition is named SAMPLING_PORT_NAME in the configuration, RETURN_CODE := INVALID_CONFIG
	// 2) If MAX_MESSAGE_SIZE is not equal to the value specified in the configuration data, RETURN_CODE := INVALID_CONFIG
	// 3) If PORT_DIRECTION is not equal to the value specified in the configuration data, RETURN_CODE := INVALID_CONFIG
	// 4) If REFRESH_PERIOD is not equal to the value specified in the configuration data, RETURN_CODE := INVALID_CONFIG

	// Enters core
	ENTER_CORE();

	// Gets module information
	MODULE_INFORMATION = _CORE_MODULE_INFORMATION;

	// Verifies current partition information
	if (_CURRENT_PARTITION_INFORMATION == null) {

		// Sets return code
		*RETURN_CODE = INVALID_MODE;

		// Exits core
		EXIT_CORE();
		return;
	}

	// Verifies operating mode
	if (_CURRENT_PARTITION_INFORMATION->OPERATING_MODE == NORMAL) {

		// Sets return code
		*RETURN_CODE = INVALID_MODE;

		// Exits core
		EXIT_CORE();
		return;
	}

	// Gets free identifier
	SAMPLING_PORT_IDENTIFIER = _CURRENT_PARTITION_INFORMATION->SAMPLINGPORT_COUNT;

	// Verifies sampling port count
	if (SAMPLING_PORT_IDENTIFIER >= _CURRENT_PARTITION_INFORMATION->PARTITION_CONFIGURATION->MAXIMUM_SAMPLINGPORT_COUNT) {

		// Sets return code
		*RETURN_CODE = INVALID_CONFIG;

		// Exits core
		EXIT_CORE();
		return;
	}

	// Gets sampling port identifier
	GET_SAMPLING_PORT_ID(SAMPLING_PORT_NAME, &SAMPLING_PORT_IDENTIFIER, RETURN_CODE);

	// Verifies sampling port identifier
	if (*RETURN_CODE == NO_ERROR) {

		// Sets return code
		*RETURN_CODE = NO_ACTION;

		// Exits core
		EXIT_CORE();
		return;
	}

	// Verifies maximum message size
	if ((MAX_MESSAGE_SIZE < PORT_INTERPARTITIONCOMMUNICATION_SAMPLINGPORT_MINIMUMMESSAGESIZE) || (MAX_MESSAGE_SIZE > PORT_INTERPARTITIONCOMMUNICATION_SAMPLINGPORT_MAXIMUMMESSAGESIZE)) {

		// Sets return code
		*RETURN_CODE = INVALID_CONFIG;

		// Exits core
		EXIT_CORE();
		return;
	}

	// Verifies port direction
	if ((PORT_DIRECTION != SOURCE) && (PORT_DIRECTION != DESTINATION)) {

		// Sets return code
		*RETURN_CODE = INVALID_CONFIG;

		// Exits core
		EXIT_CORE();
		return;
	}

	// Verifies refresh period
	if ((REFRESH_PERIOD < PORT_TICKWINDOW) && (REFRESH_PERIOD != INFINITE_TIME_VALUE)) {

		// Sets return code
		*RETURN_CODE = INVALID_CONFIG;

		// Exits core
		EXIT_CORE();
		return;
	}

	// Gets sampling port information
	SAMPLINGPORT_INFORMATION = &_CURRENT_PARTITION_INFORMATION->SAMPLINGPORT_INFORMATION[SAMPLING_PORT_IDENTIFIER];

	// Allocates memory for message - If pointer is not null, keeps it unchanged
	HEAP_ALLOCATE(&_CURRENT_PARTITION_INFORMATION->REC_HEAP, (heapPOINTER *) &SAMPLINGPORT_INFORMATION->MESSAGE, MAX_MESSAGE_SIZE, false);

	// Verifies message memory
	if (SAMPLINGPORT_INFORMATION->MESSAGE == null) {

		// Sets return code
		*RETURN_CODE = INVALID_CONFIG;

		// Exits core
		EXIT_CORE();
		return;
	}

	// Allocates memory for name - If pointer is not null, keeps it unchanged
	HEAP_ALLOCATE(&_CURRENT_PARTITION_INFORMATION->REC_HEAP, (heapPOINTER *) &SAMPLINGPORT_INFORMATION->NAME, MAX_NAME_LENGTH, false);

	// Verifies name memory
	if (SAMPLINGPORT_INFORMATION->NAME == null) {

		// Sets return code
		*RETURN_CODE = INVALID_CONFIG;

		// Exits core
		EXIT_CORE();
		return;
	}

	// Copies name
	if (!COMMON_COPYSTRING(SAMPLING_PORT_NAME, SAMPLINGPORT_INFORMATION->NAME, MAX_NAME_LENGTH)) {

		// Sets return code
		*RETURN_CODE = INVALID_CONFIG;

		// Exits core
		EXIT_CORE();
		return;
	}

	// Sets up sampling port
	SAMPLINGPORT_INFORMATION->IDENTIFIER = _CURRENT_PARTITION_INFORMATION->SAMPLINGPORT_COUNT;
	// SAMPLINGPORT_INFORMATION->NAME = SAMPLING_PORT_NAME; // Already copied
	SAMPLINGPORT_INFORMATION->MAX_MESSAGE_SIZE = MAX_MESSAGE_SIZE;
	SAMPLINGPORT_INFORMATION->PORT_DIRECTION = PORT_DIRECTION;
	SAMPLINGPORT_INFORMATION->REFRESH_PERIOD = REFRESH_PERIOD;

	// Counts created sampling port
	_CURRENT_PARTITION_INFORMATION->SAMPLINGPORT_COUNT++;

	// Sets sampling port identifier
	*SAMPLING_PORT_ID = SAMPLING_PORT_IDENTIFIER;

	// Sets return code
	*RETURN_CODE = NO_ERROR;

	// Exits core
	EXIT_CORE();
}

// Write sampling message method
void WRITE_SAMPLING_MESSAGE(SAMPLING_PORT_ID_TYPE SAMPLING_PORT_ID, MESSAGE_ADDR_TYPE MESSAGE_ADDR, MESSAGE_SIZE_TYPE LENGTH, RETURN_CODE_TYPE *RETURN_CODE) {
	MODULE_INFORMATION_TYPE *MODULE_INFORMATION;
	SAMPLINGPORT_INFORMATION_TYPE *SAMPLINGPORT_INFORMATION;
	INDEX_TYPE CHARACTER_INDEX;

	// Enters core
	ENTER_CORE();

	// Gets module information
	MODULE_INFORMATION = _CORE_MODULE_INFORMATION;

	// Verifies current partition information
	if (_CURRENT_PARTITION_INFORMATION == null) {

		// Sets return code
		*RETURN_CODE = INVALID_MODE;

		// Exits core
		EXIT_CORE();
		return;
	}

	// Verifies sampling port identifier
	if (SAMPLING_PORT_ID >= _CURRENT_PARTITION_INFORMATION->SAMPLINGPORT_COUNT) {

		// Sets return code
		*RETURN_CODE = INVALID_PARAM;

		// Exits core
		EXIT_CORE();
		return;
	}

	// Gets sampling port information
	SAMPLINGPORT_INFORMATION = &_CURRENT_PARTITION_INFORMATION->SAMPLINGPORT_INFORMATION[SAMPLING_PORT_ID];

	// Verifies length
	if (LENGTH > SAMPLINGPORT_INFORMATION->MAX_MESSAGE_SIZE) {

		// Sets return code
		*RETURN_CODE = INVALID_CONFIG;

		// Exits core
		EXIT_CORE();
		return;
	}

	// Verifies port direction
	if (SAMPLINGPORT_INFORMATION->PORT_DIRECTION != SOURCE) {

		// Sets return code
		*RETURN_CODE = INVALID_MODE;

		// Exits core
		EXIT_CORE();
		return;
	}

	// Sets sampling port empty indicator
	SAMPLINGPORT_INFORMATION->EMPTY_INDICATOR = OCCUPIED;

	// Overwrites sampling port's message with the given message
	for (CHARACTER_INDEX = 0; CHARACTER_INDEX < LENGTH; CHARACTER_INDEX++) {
		SAMPLINGPORT_INFORMATION->MESSAGE[CHARACTER_INDEX] = MESSAGE_ADDR[CHARACTER_INDEX];
	}
	SAMPLINGPORT_INFORMATION->LENGTH = LENGTH;

	// Sets last write time
	SAMPLINGPORT_INFORMATION->LAST_WRITE = CLOCK_GETSYSTEMTIME();

	// Sets return code
	*RETURN_CODE = NO_ERROR;

	// Exits core
	EXIT_CORE();
}

// Read sampling message method
void READ_SAMPLING_MESSAGE(SAMPLING_PORT_ID_TYPE SAMPLING_PORT_ID, MESSAGE_ADDR_TYPE MESSAGE_ADDR, MESSAGE_SIZE_TYPE *LENGTH, VALIDITY_TYPE *VALIDITY, RETURN_CODE_TYPE *RETURN_CODE) {
	MODULE_INFORMATION_TYPE *MODULE_INFORMATION;
	SAMPLINGPORT_INFORMATION_TYPE *SAMPLINGPORT_INFORMATION;
	INDEX_TYPE CHARACTER_INDEX;

	// Enters core
	ENTER_CORE();

	// Gets module information
	MODULE_INFORMATION = _CORE_MODULE_INFORMATION;

	// Verifies current partition information
	if (_CURRENT_PARTITION_INFORMATION == null) {

		// Sets return code
		*RETURN_CODE = INVALID_MODE;

		// Exits core
		EXIT_CORE();
		return;
	}

	// Verifies sampling port identifier
	if (SAMPLING_PORT_ID >= _CURRENT_PARTITION_INFORMATION->SAMPLINGPORT_COUNT) {

		// Sets return code
		*RETURN_CODE = INVALID_PARAM;

		// Exits core
		EXIT_CORE();
		return;
	}

	// Gets sampling port information
	SAMPLINGPORT_INFORMATION = &_CURRENT_PARTITION_INFORMATION->SAMPLINGPORT_INFORMATION[SAMPLING_PORT_ID];

	// Verifies port direction
	if (SAMPLINGPORT_INFORMATION->PORT_DIRECTION != DESTINATION) {

		// Sets return code
		*RETURN_CODE = INVALID_MODE;

		// Exits core
		EXIT_CORE();
		return;
	}

	// Verifies sampling port emptiness indicator
	if (SAMPLINGPORT_INFORMATION->EMPTY_INDICATOR == EMPTY) {

		// Sets validity
		*VALIDITY = INVALID;

		// Sets return code
		*RETURN_CODE = NO_ACTION;
	} else {

		// Copies sampling port's message
		for (CHARACTER_INDEX = 0; CHARACTER_INDEX < SAMPLINGPORT_INFORMATION->LENGTH; CHARACTER_INDEX++) {
			MESSAGE_ADDR[CHARACTER_INDEX] = SAMPLINGPORT_INFORMATION->MESSAGE[CHARACTER_INDEX];
		}

		// Sets length
		*LENGTH = SAMPLINGPORT_INFORMATION->LENGTH;

		// Sets last read time
		SAMPLINGPORT_INFORMATION->LAST_READ = CLOCK_GETSYSTEMTIME();

		// Verifies sampling port refresh period and last write time
		if ((SAMPLINGPORT_INFORMATION->REFRESH_PERIOD != INFINITE_TIME_VALUE) && (SAMPLINGPORT_INFORMATION->LAST_WRITE != INFINITE_TIME_VALUE)) {

			// Verifies last write time against refresh period
			if ((CLOCK_GETSYSTEMTIME() - SAMPLINGPORT_INFORMATION->LAST_WRITE) > SAMPLINGPORT_INFORMATION->REFRESH_PERIOD) {

				// Sets validity
				*VALIDITY = INVALID;
			} else {

				// Sets validity
				*VALIDITY = VALID;
			}
		} else {

			// Sets validity
			*VALIDITY = VALID;
		}

		// Sets return code
		*RETURN_CODE = NO_ERROR;
	}

	// Sets last validity
	SAMPLINGPORT_INFORMATION->LAST_VALIDITY = *VALIDITY;

	// Exits core
	EXIT_CORE();
}

// Sampling port identifier getter
void GET_SAMPLING_PORT_ID(SAMPLING_PORT_NAME_TYPE SAMPLING_PORT_NAME, SAMPLING_PORT_ID_TYPE *SAMPLING_PORT_ID, RETURN_CODE_TYPE *RETURN_CODE) {
	MODULE_INFORMATION_TYPE *MODULE_INFORMATION;

	// Enters core
	ENTER_CORE();

	// Gets module information
	MODULE_INFORMATION = _CORE_MODULE_INFORMATION;

	// Verifies current partition information
	if (_CURRENT_PARTITION_INFORMATION == null) {

		// Sets return code
		*RETURN_CODE = INVALID_MODE;

		// Exits core
		EXIT_CORE();
		return;
	}

	// Gets partition sampling port identifier
	GET_PARTITION_SAMPLING_PORT_ID(_CURRENT_PARTITION_INFORMATION->IDENTIFIER, SAMPLING_PORT_NAME, SAMPLING_PORT_ID, RETURN_CODE);

	// Exits core
	EXIT_CORE();
}

// Sampling port status getter
void GET_SAMPLING_PORT_STATUS(SAMPLING_PORT_ID_TYPE SAMPLING_PORT_ID, SAMPLING_PORT_STATUS_TYPE *SAMPLING_PORT_STATUS, RETURN_CODE_TYPE *RETURN_CODE) {
	MODULE_INFORMATION_TYPE *MODULE_INFORMATION;
	SAMPLINGPORT_INFORMATION_TYPE *SAMPLINGPORT_INFORMATION;

	// Enters core
	ENTER_CORE();

	// Gets module information
	MODULE_INFORMATION = _CORE_MODULE_INFORMATION;

	// Verifies current partition information
	if (_CURRENT_PARTITION_INFORMATION == null) {

		// Sets return code
		*RETURN_CODE = INVALID_MODE;

		// Exits core
		EXIT_CORE();
		return;
	}

	// Verifies sampling port identifier
	if (SAMPLING_PORT_ID >= _CURRENT_PARTITION_INFORMATION->SAMPLINGPORT_COUNT) {

		// Sets return code
		*RETURN_CODE = INVALID_PARAM;

		// Exits core
		EXIT_CORE();
		return;
	}

	// Gets sampling port information
	SAMPLINGPORT_INFORMATION = &_CURRENT_PARTITION_INFORMATION->SAMPLINGPORT_INFORMATION[SAMPLING_PORT_ID];

	// Copies status
	SAMPLING_PORT_STATUS->MAX_MESSAGE_SIZE = SAMPLINGPORT_INFORMATION->MAX_MESSAGE_SIZE;
	SAMPLING_PORT_STATUS->PORT_DIRECTION = SAMPLINGPORT_INFORMATION->PORT_DIRECTION;
	SAMPLING_PORT_STATUS->REFRESH_PERIOD = SAMPLINGPORT_INFORMATION->REFRESH_PERIOD;
	SAMPLING_PORT_STATUS->LAST_MSG_VALIDITY = SAMPLINGPORT_INFORMATION->LAST_VALIDITY;

	// Sets return code
	*RETURN_CODE = NO_ERROR;

	// Exits core
	EXIT_CORE();
}
