#ifndef __SYSTEM_H
#define __SYSTEM_H

#include "stm32h7xx_hal.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cmsis_os.h"

#include "Components/Inc/lcd.h"
#include "Components/Inc/touch.h"
#include "Components/Inc/is42s32800j.h"
#include "Components/Inc/mt25tl01g.h"

#include "Hardware/Inc/ltdc_dsi.h"
#include "Hardware/Inc/i2c.h"
#include "Hardware/Inc/qspi.h"
#include "Hardware/Inc/sdram.h"
#include "Hardware/Inc/uart.h"
#include "Hardware/Inc/tim_pwm.h"


#define AC_OK   HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_7);
#define CHG_OK  HAL_GPIO_ReadPin(GPIOG,GPIO_PIN_11);
#define PBTN    HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_8);

void Hardware_Init(void);
void turn_off_device();
void get_power_on_switch_State();
void Error_Handler(char *file, uint32_t line);

#endif