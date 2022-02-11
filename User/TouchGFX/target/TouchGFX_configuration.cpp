#include <touchgfx/hal/GPIO.hpp>
#include <common/TouchGFXInit.hpp>
#include <touchgfx/hal/BoardConfiguration.hpp>
#include <platform/driver/lcd/LCD24bpp.hpp>

#include <stm32h7xx_hal.h>
#include "../../../BSP/system.h"

#include <STM32H7HAL.hpp>
#include <TouchGFXDMA.hpp>

#include <STM32TouchController.hpp>
#include <STM32H7Instrumentation.hpp>

extern "C" lcd_objectTypeDef lcd_obj;
extern "C" void touchgfx_init();
// extern "C" void touchgfx_taskEntry();

#define LAYER0_ADDRESS  (LCD_FRAME_BUFFER)

static uint32_t frameBuf0 = (uint32_t)(LAYER0_ADDRESS); //Beginning of SDRAM

TouchGFXDMA dma;
STM32TouchController tc;
STM32H7Instrumentation mcuInstr;
touchgfx::LCD24bpp lcd;
uint16_t bitDepth = 24;
// static TouchGFXHAL hal(dma, lcd, tc, 800, 480);

void touchgfx_init()
{
    uint32_t dispWidth=480;
    uint32_t dispHeight=854;

    HAL& hal = touchgfx_generic_init<STM32H7HAL>(dma, lcd, tc, dispWidth, dispHeight,
                                                 (uint16_t *)(frameBuf0 + dispWidth * dispHeight * 2 * 3),
                                                 4 * 1024 * 1024, 64);


    hal.setFrameBufferStartAddresses((uint16_t*)frameBuf0, NULL, NULL);
    hal.setTouchSampleRate(1);
    hal.setFingerSize(1);

    // By default frame rate compensation is off.
    // Enable frame rate compensation to smooth out animations in case there is periodic slow frame rates.
    hal.setFrameRateCompensation(false);

    // This platform can handle simultaneous DMA and TFT accesses to SDRAM, so disable lock to increase performance.
    hal.lockDMAToFrontPorch(false);

    mcuInstr.init();

    //Set MCU instrumentation and Load calculation
    hal.setMCUInstrumentation(&mcuInstr);
    hal.enableMCULoadCalculation(true);
}

// void touchgfx_taskEntry()
// {
//     /*
//      * Main event loop. Will wait for VSYNC signal, and then process next frame. Call
//      * this function from your GUI task.
//      *
//      * Note This function never returns
//      */
//     hal.taskEntry();
// }