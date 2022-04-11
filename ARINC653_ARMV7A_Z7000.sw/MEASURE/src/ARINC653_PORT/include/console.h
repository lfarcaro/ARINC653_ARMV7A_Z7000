#ifndef CONSOLE_H_
#define CONSOLE_H_

// Includes
#include "xuartps.h"

// Startup method
unsigned char CONSOLE_STARTUP(void);

// Buffer writing method
unsigned int CONSOLE_WRITE_BUFFER(unsigned char *BUFFER, unsigned int LENGTH);

// Buffer reading method
unsigned int CONSOLE_READ_BUFFER(unsigned char *BUFFER, unsigned int LENGTH);

// String printing method
void CONSOLE_PUTS(const char *STRING, unsigned int LENGTH);

// Number printing method
unsigned int CONSOLE_PRINTN(int VALUE, unsigned char BASE, unsigned char SIGNED, unsigned char UPPERCASE, unsigned char PADDING_CHARACTER, int PADDING_LENGTH);

// Formatted printing method
void CONSOLE_PRINTF(const char *STRING, ...);

#endif
