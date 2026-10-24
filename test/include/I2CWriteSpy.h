#ifndef D_I2CWriteSpy_H
#define D_I2CWriteSpy_H
#include "hardware/i2c.h"
#include "SSD1306.h"

/**********************************************************
 *
 * I2CWriteSpy is responsible for ...
 *
 **********************************************************/

void I2CWriteSpy_Create(void);
void I2CWriteSpy_Destroy(void);
command_t* I2CWriteSpy_GetBufferPtr(void);

#endif  /* D_FakeI2CWriteSpy_H */
