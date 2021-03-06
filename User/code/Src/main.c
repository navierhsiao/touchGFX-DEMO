#include "main.h"
#include "cmsis_os.h"
#include "fatfs.h"
#include "../../../BSP/system.h"
#include "app_touchgfx.h"

osThreadId_t defaultTaskHandle;
osThreadId_t touchTaskID;
osThreadId_t TouchGFXTaskHandle;

const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};

const osThreadAttr_t TouchGFXTask_attributes = {
  .name = "TouchGFXTask",
  .stack_size = 4048,
  .priority = ( tskIDLE_PRIORITY + 3 ),
};

void StartDefaultTask(void *argument);
void TouchGFX_Task(void *argument);

int main(void)
{
  Hardware_Init();
  MX_TouchGFX_Init();

  osKernelInitialize();
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);
  TouchGFXTaskHandle = osThreadNew(TouchGFX_Task, NULL, &TouchGFXTask_attributes);
  osKernelStart();

  while (1);
}

void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN 5 */
  /* Infinite loop */
  uint8_t count=0;
  for(;;)
  {
    LED1_TOGGLE;
    osDelay(500);
  }
  /* USER CODE END 5 */
}

__weak void TouchGFX_Task(void *argument)
{
  for(;;)
  {
    osDelay(1);
  }
}
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
