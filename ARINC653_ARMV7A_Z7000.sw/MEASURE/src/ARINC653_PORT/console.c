// Includes
#include "console.h"

// XUartPs instance identifier array
static unsigned char XUartPsDeviceId[CONSOLE_INSTANCECOUNT] = { XPAR_XUARTPS_0_DEVICE_ID, XPAR_XUARTPS_1_DEVICE_ID };

// XUartPs configuration data pointer
static XUartPs_Config *XUartPsConfigPtr[CONSOLE_INSTANCECOUNT];

// XUartPs instance data
static XUartPs XUartPsInst[CONSOLE_INSTANCECOUNT];

// XUartPs instance data format
static XUartPsFormat XUartPsInstFormat[CONSOLE_INSTANCECOUNT];

// Number printing digits
static const char * const PRINTN_DIGITS_LOWER = "0123456789abcdefghijklmnopqrstuvwxyz";
static const char * const PRINTN_DIGITS_UPPER = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";

// Startup method
unsigned char CONSOLE_STARTUP(unsigned char INSTANCE) {

	// Checks instance
	if (INSTANCE >= CONSOLE_INSTANCECOUNT) {
		return 0;
	}

	// Looks up the UART controller's configuration
	XUartPsConfigPtr[INSTANCE] = XUartPs_LookupConfig(XUartPsDeviceId[INSTANCE]);
	if (XUartPsConfigPtr[INSTANCE] == NULL) {
		return 0;
	}

	// Resets the UART controller
	XUartPs_ResetHw(XUartPsConfigPtr[INSTANCE]->BaseAddress);

	// Initializes the UART controller's configuration
	if (XUartPs_CfgInitialize(&XUartPsInst[INSTANCE], XUartPsConfigPtr[INSTANCE], XUartPsConfigPtr[INSTANCE]->BaseAddress) != XST_SUCCESS) {
		return 0;
	}

	// Sets the UART controller's operation mode
	XUartPs_SetOperMode(&XUartPsInst[INSTANCE], XUARTPS_OPER_MODE_NORMAL);

	// Sets the UART controller's data format
	XUartPsInstFormat[INSTANCE].BaudRate = 115200;
	XUartPsInstFormat[INSTANCE].DataBits = XUARTPS_FORMAT_8_BITS;
	XUartPsInstFormat[INSTANCE].Parity = XUARTPS_FORMAT_NO_PARITY;
	XUartPsInstFormat[INSTANCE].StopBits = XUARTPS_FORMAT_1_STOP_BIT;
	if (XUartPs_SetDataFormat(&XUartPsInst[INSTANCE], &XUartPsInstFormat[INSTANCE]) != XST_SUCCESS) {
		return 0;
	}
	return 1;
}

// Buffer writing method
unsigned int CONSOLE_WRITE_BUFFER(unsigned char INSTANCE, unsigned char *BUFFER, unsigned int LENGTH) {
	LENGTH = XUartPs_Send(&XUartPsInst[INSTANCE], BUFFER, LENGTH);
	while (!XUartPs_IsTransmitEmpty(&XUartPsInst[INSTANCE]));
	return LENGTH;
}

// Buffer reading method
unsigned int CONSOLE_READ_BUFFER(unsigned char INSTANCE, unsigned char *BUFFER, unsigned int LENGTH) {
	return XUartPs_Recv(&XUartPsInst[INSTANCE], BUFFER, LENGTH);
}

// String printing method similar to 'puts'.
void CONSOLE_PUTS(unsigned char INSTANCE, const char *STRING, unsigned int LENGTH) {
	CONSOLE_WRITE_BUFFER(INSTANCE, (unsigned char *) STRING, LENGTH);
}

// Number printing method
unsigned int CONSOLE_PRINTN(unsigned char INSTANCE, int VALUE, unsigned char BASE, unsigned char SIGNED, unsigned char UPPERCASE, unsigned char PADDING_CHARACTER, int PADDING_LENGTH) {
	unsigned char NEGATIVE;
	unsigned int DIMENSION, INDEX;
	unsigned char BUFFER[32];

	// Standardizes value and stores negative indicator
	NEGATIVE = (SIGNED && (VALUE < 0) ? 1 : 0);
	if (NEGATIVE) {
		VALUE = -VALUE;
	}

	// Calculates dimension according to the number's base
	DIMENSION = 1;
	while (((DIMENSION * BASE) <= VALUE) && (((DIMENSION * BASE) / BASE) == DIMENSION)) {
		DIMENSION *= BASE;
		if (PADDING_LENGTH > 0) {
			PADDING_LENGTH--;
		}
	}

	// Eliminates a padding character for the signal
	if (NEGATIVE && (PADDING_LENGTH > 0)) {
		PADDING_LENGTH--;
	}

	// Initializes index
	INDEX = 0;

	// For zero-padded negative numbers, the first character is the signal
	if(NEGATIVE && (PADDING_CHARACTER == '0')) {
		BUFFER[INDEX++] = '-';
		NEGATIVE = 0;
	}

	// Appends padding characters
	if ((PADDING_LENGTH > 1) && (PADDING_LENGTH < 32)) {
		while (PADDING_LENGTH > 0) {
			BUFFER[INDEX++] = PADDING_CHARACTER;
			PADDING_LENGTH--;
		}
	}

	// Appends signal for negative numbers
	if (NEGATIVE) {
		BUFFER[INDEX++] = '-';
	}

	// Converts the value to string
	while (DIMENSION != 0) {
		BUFFER[INDEX++] = (UPPERCASE ? PRINTN_DIGITS_UPPER[(VALUE / DIMENSION) % BASE] : PRINTN_DIGITS_LOWER[(VALUE / DIMENSION) % BASE]);
		DIMENSION /= BASE;
	}

	// Writes the string
	return CONSOLE_WRITE_BUFFER(INSTANCE, BUFFER, INDEX);
}

// Formatted printing method similar to 'printf'.
// Supports the following fields:
// %c - Character field
// %d - Signed decimal integer field
// %u - Unsigned decimal integer field
// %x - Hexadecimal value using lower case letters
// %X - Hexadecimal value using upper case letters
// %p - Pointer field in hexadecimal format
// %s - String field
// %% - The % character itself
// The % indicator can be immediately followed by a decimal number indicating the minimum length of the printed string,
// which is left-padded with spaces by default or with zeroes if the '0' character immediately succeeds the % indicator.
void CONSOLE_PRINTF(unsigned char INSTANCE, const char *FORMAT, ...) {
	unsigned char PADDING_CHARACTER;
	int PADDING_LENGTH;
	unsigned int VALUE, LENGTH;
	char *VALUE_STRING;
	va_list varargs;

	// Starts varargs processing
	va_start(varargs, FORMAT);

	// Null-terminated format string iteration loop
	while (*FORMAT) {

		// Gets the contiguous section length
		LENGTH = 0;
		while ((FORMAT[LENGTH] != '%') && (FORMAT[LENGTH] != 0)) {
			LENGTH++;
		}

		// Writes contiguous section
		CONSOLE_WRITE_BUFFER(INSTANCE, (unsigned char *) FORMAT, LENGTH);

		// Skips contiguous section
		FORMAT += LENGTH;

		// Verifies field indicator
		if (*FORMAT == '%') {

			// Skips field indicator
			FORMAT++;

			// Determines padding length and character
			PADDING_LENGTH = 0;
			if (*FORMAT == '0') {
				FORMAT++;
				PADDING_CHARACTER = '0';
			} else {
				PADDING_CHARACTER = ' ';
			}
			while ((*FORMAT >= '0') && (*FORMAT <= '9')) {
				PADDING_LENGTH = (PADDING_LENGTH * 10) + (*FORMAT++ - '0');
			}

			// Verifies the field character
			switch (*FORMAT++) {
				// Character field
				case 'c': {
					// Gets the varargs value
					VALUE = va_arg(varargs, unsigned int);
					// Writes padding characters
					while (PADDING_LENGTH-- > 1) {
						CONSOLE_WRITE_BUFFER(INSTANCE, &PADDING_CHARACTER, 1);
					}
					// Writes the character
					CONSOLE_WRITE_BUFFER(INSTANCE, (unsigned char *) &VALUE, 1);
					break;
				}
				// Decimal integer field
				case 'd': {
					// Gets the varargs value
					VALUE = va_arg(varargs, unsigned int);
					// Prints value as signed decimal
					CONSOLE_PRINTN(INSTANCE, VALUE, 10, TRUE, FALSE, PADDING_CHARACTER, PADDING_LENGTH);
					break;
				}
				// Unsigned decimal integer field
				case 'u': {
					// Gets the varargs value
					VALUE = va_arg(varargs, unsigned int);
					// Prints value as unsigned decimal
					CONSOLE_PRINTN(INSTANCE, VALUE, 10, FALSE, FALSE, PADDING_CHARACTER, PADDING_LENGTH);
					break;
				}
				// String field
				case 's': {
					// Gets the varargs value
					VALUE_STRING = va_arg(varargs, char *);
					// Determines the length of the string
					LENGTH = 0;
					while (VALUE_STRING[LENGTH] != 0) {
						LENGTH++;
					}
					// Writes padding characters
					while (PADDING_LENGTH-- > LENGTH) {
						CONSOLE_WRITE_BUFFER(INSTANCE, &PADDING_CHARACTER, 1);
					}
					// Writes the string
					CONSOLE_WRITE_BUFFER(INSTANCE, (unsigned char *) VALUE_STRING, LENGTH);
					break;
				}
				// Hexadecimal integer and pointer fields
				case 'x': {
					// Gets the varargs value
					VALUE = va_arg(varargs, unsigned int);
					// Prints value as unsigned lower-case hexadecimal
					CONSOLE_PRINTN(INSTANCE, VALUE, 16, FALSE, FALSE, PADDING_CHARACTER, PADDING_LENGTH);
					break;
				}
				case 'X': {
					// Gets the varargs value
					VALUE = va_arg(varargs, unsigned int);
					// Prints value as unsigned upper-case hexadecimal
					CONSOLE_PRINTN(INSTANCE, VALUE, 16, FALSE, TRUE, PADDING_CHARACTER, PADDING_LENGTH);
					break;
				}
				case 'p': {
					// Gets the varargs value
					VALUE = va_arg(varargs, unsigned int);
					// Prints value as unsigned upper-case hexadecimal
					CONSOLE_PRINTN(INSTANCE, VALUE, 16, FALSE, TRUE, PADDING_CHARACTER, PADDING_LENGTH);
					break;
				}
				// Field indicator escaping
				case '%': {
					// Writes the % character
					CONSOLE_WRITE_BUFFER(INSTANCE, (unsigned char *) (FORMAT - 1), 1);
					break;
				}
				// Invalid fields
				default: {
					// Writes error indicator
					CONSOLE_WRITE_BUFFER(INSTANCE, (unsigned char *) "<ERROR>", 7);
					break;
				}
			}
		}
	}

	// Finishes varargs processing
	va_end(varargs);
}
