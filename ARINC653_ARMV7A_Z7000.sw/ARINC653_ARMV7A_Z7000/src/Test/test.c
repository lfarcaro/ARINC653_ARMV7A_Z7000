// General includes
#include "test.h"

// Hardware includes
#include "xgpiops.h"

// XGpioPs configuration data pointer
static XGpioPs_Config *XGpioPsConfigPtr;

// XGpioPs instance data
static XGpioPs XGpioPsInst;

// Test startup method
unsigned char TEST_STARTUP(void) {

	// Looks up the GPIO controller's configuration
	XGpioPsConfigPtr = XGpioPs_LookupConfig(XPAR_XGPIOPS_0_DEVICE_ID);
	if (XGpioPsConfigPtr == NULL) {
		return 0;
	}

	// Initializes the GPIO controller's configuration
	if (XGpioPs_CfgInitialize(&XGpioPsInst, XGpioPsConfigPtr, XGpioPsConfigPtr->BaseAddr) != XST_SUCCESS) {
		return 0;
	}

	// Configures the GPIO controller's pins' directions
	XGpioPs_SetDirectionPin(&XGpioPsInst, 54, 1);
	XGpioPs_SetDirectionPin(&XGpioPsInst, 55, 1);

	// Configures the GPIO controller's output pins
	XGpioPs_SetOutputEnablePin(&XGpioPsInst, 54, 1);
	XGpioPs_SetOutputEnablePin(&XGpioPsInst, 55, 1);

	// Accesses the GPIO controller's pins
	XGpioPs_WritePin(&XGpioPsInst, 54, 1);
	XGpioPs_WritePin(&XGpioPsInst, 55, 1);

	// Accesses the GPIO controller's pins
	XGpioPs_WritePin(&XGpioPsInst, 54, 0);
	XGpioPs_WritePin(&XGpioPsInst, 55, 0);

	// Accesses the GPIO controller's pins
	XGpioPs_WritePin(&XGpioPsInst, 54, 1);
	XGpioPs_WritePin(&XGpioPsInst, 55, 1);

	// Accesses the GPIO controller's pins
	TEST_LED1_ON();
	TEST_LED2_ON();

	// Accesses the GPIO controller's pins
	TEST_LED1_OFF();
	TEST_LED2_OFF();
	return 1;
}
