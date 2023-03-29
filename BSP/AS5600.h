#ifndef AS5600
#define AS5600

#include "main.h"
#include "i2c.h"
#include "IQmathLib.h"
#include "FOC_Ctrl.h"
#include "i2c.h"

#define  RAW_Angle_Hi    0x0C
#define  RAW_Angle_Lo    0x0D
#define  AS5600_Address  0x36


extern void AS5600_Get_Angle(void);
extern uint8_t Temp_Angle_Data[2];
extern void AS5600_Get_RAWData(void);




#endif