#ifndef __LTDC_DSI_H
#define __LTDC_DSI_H

#define LCD_FRAME_BUFFER        0xD0000000

#define VSYNC           1
#define VBP             1
#define VFP             1
#define VACT            480
#define HSYNC           1
#define HBP             1
#define HFP             1
#define HACT            800

#define Xsize           800
#define Ysize           480

typedef struct ltdc_dsi_structDef
{
    DMA2D_HandleTypeDef hdma2d;
    DSI_HandleTypeDef   hdsi;
    LTDC_HandleTypeDef  hltdc;
    __IO int32_t        pend_buffer;

    void (*dsi_IO_write)    (struct ltdc_dsi_structDef *object,uint16_t chNbr, uint16_t reg, uint8_t* data, uint16_t size);
    void (*dsi_IO_read)     (struct ltdc_dsi_structDef *object,uint16_t chNbr, uint16_t reg, uint8_t* data, uint16_t size);
    void (*dsi_refresh)     (struct ltdc_dsi_structDef *object);
    void (*copy_buffer_M2M) (struct ltdc_dsi_structDef *object,uint32_t *src,uint16_t x,uint16_t y,uint16_t xsize,uint16_t ysize);
    void (*copy_buffer_R2M) (struct ltdc_dsi_structDef *object,uint32_t *dst,uint16_t xsize,uint16_t ysize,uint16_t offline,uint32_t color);
}ltdc_dsi_objectTypeDef;

void LTDC_DSI_object_Init(ltdc_dsi_objectTypeDef *object);

#endif