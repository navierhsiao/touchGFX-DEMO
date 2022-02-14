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
extern "C" void touchgfx_taskEntry();

static uint32_t frameBuf0 = (uint32_t)(LCD_FRAME_BUFFER); //Beginning of SDRAM
// static uint32_t frameBuf1 = (uint32_t)(LCD_FRAME_BUFFER_2);

static TouchGFXDMA dma;
static STM32TouchController tc;
static STM32H7Instrumentation mcuInstr;
static LCD24bpp lcd;
static STM32H7HAL hal(dma, lcd, tc, 480, 854);

void touchgfx_init()
{
    Bitmap::registerBitmapDatabase(BitmapDatabase::getInstance(), BitmapDatabase::getInstanceSize());
    TypedText::registerTexts(&texts);
    Texts::setLanguage(0);

    FontManager::setFontProvider(&fontProvider);

    FrontendHeap& heap = FrontendHeap::getInstance();
    /*
     * we need to obtain the reference above to initialize the frontend heap.
     */
    (void)heap;

    /*
     * Initialize TouchGFX
     */
    hal.initialize();

    hal.lockDMAToFrontPorch(false);

    mcuInstr.init();
    hal.setMCUInstrumentation(&mcuInstr);
    hal.enableMCULoadCalculation(true);
}


void touchgfx_taskEntry()
{
    /*
     * Main event loop. Will wait for VSYNC signal, and then process next frame. Call
     * this function from your GUI task.
     *
     * Note This function never returns
     */
    // touchgfx::HAL::getInstance()->taskEntry();
    hal.taskEntry();
}