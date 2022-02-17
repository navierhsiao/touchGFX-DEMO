#include <STM32H7HAL.hpp>

#include <touchgfx/hal/OSWrappers.hpp>
#include <gui/common/FrontendHeap.hpp>
#include <touchgfx/hal/GPIO.hpp>

#include "stm32h7xx.h"
#include "../../../BSP/system.h"

extern "C" lcd_objectTypeDef lcd_obj;

using namespace touchgfx;

void STM32H7HAL::initialize()
{
    HAL::initialize();
    registerEventListener(*(Application::getInstance()));
    //buffer2=0xD0000000+480*854*3(bitsize of RGB888=3)
    //anim buffer=buffer2+single buffersize
    setFrameBufferStartAddresses((void*)0xD0000000, (void*)0xD0190500, (void*)0xD0320A00);
    /*
     * Set whether the DMA transfers are locked to the TFT update cycle. If
     * locked, DMA transfer will not begin until the TFT controller has finished
     * updating the display. If not locked, DMA transfers will begin as soon as
     * possible. Default is true (DMA is locked with TFT).
     */
    lockDMAToFrontPorch(true);
}

uint16_t* STM32H7HAL::getTFTFrameBuffer() const
{
    return (uint16_t*)LTDC_Layer1->CFBAR;
}

void STM32H7HAL::setTFTFrameBuffer(uint16_t* adr)
{
    LTDC_Layer1->CFBAR = (uint32_t)adr;

    /* Reload immediate */
    LTDC->SRCR = (uint32_t)LTDC_SRCR_IMR;
}

bool STM32H7HAL::blockCopy(void* RESTRICT dest, const void* RESTRICT src, uint32_t numBytes)
{
    return HAL::blockCopy(dest, src, numBytes);
}

void STM32H7HAL::taskEntry()
{
    enableLCDControllerInterrupt();
    enableInterrupts();

    OSWrappers::waitForVSync();
    backPorchExited();
    for (;;)
    {
        OSWrappers::waitForVSync();
        backPorchExited();
    }
}

uint16_t STM32H7HAL::getTFTCurrentLine()
{
    // This function only requires an implementation if single buffering
    // on LTDC display is being used (REFRESH_STRATEGY_OPTIM_SINGLE_BUFFER_TFT_CTRL).

    // The CPSR register (bits 15:0) specify current line of TFT controller.
    uint16_t curr = (uint16_t)(LTDC->CPSR & 0xffff);
    uint16_t backPorchY = (uint16_t)(LTDC->BPCR & 0x7FF) + 1;

    // The semantics of the getTFTCurrentLine() function is to return a value
    // in the range of 0-totalheight. If we are still in back porch area, return 0.
    if (curr < backPorchY)
    {
        return 0;
    }
    else
    {
        return curr - backPorchY;
    }
}

void STM32H7HAL::configureInterrupts()
{
    // These two priorities MUST be EQUAL, and MUST be functionally lower than RTOS scheduler interrupts.
    NVIC_SetPriority(DMA2D_IRQn, 7);
    NVIC_SetPriority(LTDC_IRQn, 7);
}

static uint16_t lcd_int_active_line;
static uint16_t lcd_int_porch_line;

/* Enable LCD line interrupt, when entering video (active) area */
void STM32H7HAL::enableLCDControllerInterrupt()
{
    lcd_int_active_line = (LTDC->BPCR & 0x7FF) - 1;
    lcd_int_porch_line = (LTDC->AWCR & 0x7FF) - 1;

    HAL_LTDC_ProgramLineEvent(&lcd_obj.dsi_object.hltdc, lcd_int_active_line);
    __HAL_LTDC_ENABLE_IT(&lcd_obj.dsi_object.hltdc, LTDC_IT_LI|LTDC_IT_FU); /* Enable line and FIFO underrun interrupts */
}

void STM32H7HAL::disableInterrupts()
{
    NVIC_DisableIRQ(LTDC_IRQn);
    NVIC_DisableIRQ(DMA2D_IRQn);
    NVIC_DisableIRQ(LTDC_ER_IRQn);
}

void STM32H7HAL::enableInterrupts()
{
    NVIC_EnableIRQ(LTDC_IRQn);
    NVIC_EnableIRQ(DMA2D_IRQn);
    NVIC_EnableIRQ(LTDC_ER_IRQn);
    
    __HAL_DSI_CLEAR_FLAG(&lcd_obj.dsi_object.hdsi, DSI_IT_ER);
    __HAL_DSI_CLEAR_FLAG(&lcd_obj.dsi_object.hdsi, DSI_IT_TE);
    __HAL_DSI_ENABLE_IT(&lcd_obj.dsi_object.hdsi, DSI_IT_TE);
    __HAL_DSI_ENABLE_IT(&lcd_obj.dsi_object.hdsi, DSI_IT_ER);
}

void STM32H7HAL::flushFrameBuffer(const touchgfx::Rect& rect)
{
    // If the framebuffer is placed in cached memory (e.g. SRAM) then we need
    // the CPU to read back the latest pixels produced by the h/w accelerator
    // thus we have to both flush and invalidate the Dcache prior to letting
    // DMA2D accessing it. That's done using SCB_CleanInvalidateDCache().
    HAL::flushFrameBuffer(rect);
    SCB_CleanInvalidateDCache();
}

void STM32H7HAL::InvalidateCache()
{
    // If the framebuffer is placed in Write Through cached memory (e.g. SRAM) then
    // the DCache must be flushed prior to DMA2D accessing it. That's done
    // using the function SCB_CleanInvalidateDCache(). Remember to enable "CPU Cache" in the
    // "System Core" settings for "Cortex M7" in CubeMX in order for this function call to work.
    SCB_CleanInvalidateDCache();
}

void STM32H7HAL::FlushCache()
{
    // If the framebuffer is placed in Write Through cached memory (e.g. SRAM) then
    // the DCache must be flushed prior to DMA2D accessing it. That's done
    // using the function SCB_CleanInvalidateDCache(). Remember to enable "CPU Cache" in the
    // "System Core" settings for "Cortex M7" in CubeMX in order for this function call to work.
    SCB_CleanInvalidateDCache();
}

extern "C"
{
    void HAL_LTDC_LineEvenCallback(LTDC_HandleTypeDef *hltdc)
    {
        if (LTDC->LIPCR == lcd_int_active_line)
        {
            //entering active area
            HAL_LTDC_ProgramLineEvent(hltdc, lcd_int_porch_line);
            HAL::getInstance()->vSync();
            OSWrappers::signalVSync();
            // Swap frame buffers immediately instead of waiting for the task to be scheduled in.
            // Note: task will also swap when it wakes up, but that operation is guarded and will not have
            // any effect if already swapped.
            HAL::getInstance()->swapFrameBuffers();
            GPIO::set(GPIO::VSYNC_FREQ);
        }
        else
        {
            //exiting active area
            HAL_LTDC_ProgramLineEvent(hltdc, lcd_int_active_line);
            GPIO::clear(GPIO::VSYNC_FREQ);
            HAL::getInstance()->frontPorchEntered();
        }
    }

    portBASE_TYPE IdleTaskHook(void* p)
    {
        if ((int)p) //idle task sched out
        {
            touchgfx::HAL::getInstance()->setMCUActive(true);
        }
        else //idle task sched in
        {
            touchgfx::HAL::getInstance()->setMCUActive(false);
        }
        return pdTRUE;
    }
}