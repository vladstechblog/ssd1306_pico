#ifndef D_I2CWriteSpy_H
#define D_I2CWriteSpy_H
#include "hardware/i2c.h"
#include "SSD1306.h"

#define BUFFER_VALUE_MISMATCH -100
#define BUFFER_LENGTH_MISMATCH -101

/**********************************************************
 *
 * I2CWriteSpy is responsible for ...
 *
 **********************************************************/

void I2CWriteSpy_Create(void);
void I2CWriteSpy_Destroy(void);
command_t* I2CWriteSpy_GetBufferPtr(void);
void I2CWriteSpy_SetExpectedBytes(command_t, command_t);
int I2CWriteSpy_GetInvocationCounter(void);
#endif  /* D_I2CWriteSpy_H */
