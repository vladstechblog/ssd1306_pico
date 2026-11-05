#ifndef D_I2CWriteSpy_H
#define D_I2CWriteSpy_H
#include "hardware/i2c.h"
#include "SSD1306.h"

#define BUFFER_VALUE_MISMATCH -100
#define BUFFER_LENGTH_MISMATCH -101

void I2CWriteSpy_Create(void);
void I2CWriteSpy_Destroy(void);
void I2CWriteSpy_SetExpectedI2CPayloadBytes(command_t, command_t);
int I2CWriteSpy_GetInvocationCounter(void);
data_t* I2CWriteSpy_GetExpectedI2CPayloadPointer(void);
data_t* I2CWriteSpy_GetExpectedBuffer(void);
void I2CWriteSpy_SetExpectedBuffer(data_t*);
void I2CWriteSpy_SetExpectedBufferSize(size_t);
size_t I2CWriteSpy_GetExpectedBufferSize(void);

#endif  /* I2CWriteSpy_H */
