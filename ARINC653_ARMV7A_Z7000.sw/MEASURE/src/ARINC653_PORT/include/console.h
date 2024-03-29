#ifndef CONSOLE_H_
#define CONSOLE_H_

// Includes
#include "xuartps.h"

#define CONSOLE_INSTANCECOUNT XPAR_XUARTPS_NUM_INSTANCES
#define CONSOLE_DEFAULTINSTANCE 0

// Startup method
unsigned char CONSOLE_STARTUP(unsigned char INSTANCE);

// Buffer writing method
unsigned int CONSOLE_WRITE_BUFFER(unsigned char INSTANCE, unsigned char *BUFFER, unsigned int LENGTH);

// Buffer reading method
unsigned int CONSOLE_READ_BUFFER(unsigned char INSTANCE, unsigned char *BUFFER, unsigned int LENGTH);

// String printing method
void CONSOLE_PUTS(unsigned char INSTANCE, const char *STRING, unsigned int LENGTH);

// Number printing method
unsigned int CONSOLE_PRINTN(unsigned char INSTANCE, int VALUE, unsigned char BASE, unsigned char SIGNED, unsigned char UPPERCASE, unsigned char PADDING_CHARACTER, int PADDING_LENGTH);

// Formatted printing method
void CONSOLE_PRINTF(unsigned char INSTANCE, const char *STRING, ...);

#endif
