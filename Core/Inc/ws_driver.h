#ifndef WS_DRIVER_H
#define WS_DRIVER_H
#include "stm32f1xx_hal.h"
#include "main.h"



#define PIXEL_NUMS 8 // 灯珠数量
#define PIXEL_BITS_NUMS 24 // 灯珠位数
#define PIXEL_BITS_LENS PIXEL_NUMS * PIXEL_BITS_NUMS // 灯珠总位数



enum
{
    GREEN = 0XFF0000,
    RED = 0X00FF00,
    BLUE = 0X0000FF,
    WHITE = 0XFFFFFF,
    PURPLE = 0X00FFFF,
    YELLOW = 0XFFFF00,
    ZERO = 0X000000
} WS_COLOR;



enum
{
    CODE_H = 0XF,
    CODE_L = 0XC
} WS_CODE;

typedef struct
{
    uint32_t color;
    unsigned int timer;
    uint8_t brightness;

}PIXEL_TypeDef;

// 颜色转换
uint32_t ws_color(uint8_t red,uint8_t green,uint8_t blue);
uint32_t ws_color_brighness(uint8_t red,uint8_t green,uint8_t blue,uint16_t brightness);
uint32_t ws_wheel(uint8_t wheelPos);

// 灯珠控制
// void ws_setPixelRGB(uint16_t n ,uint8_t red,uint8_t green,uint8_t blue);
// void ws_setPixelColor(uint16_t n ,uint32_t GRBcolor);
// void ws_setAllPixelColor(uint32_t GRBcolor);

//todo 新的灯珠控制
// uint8_t ws_getPixelBrightness(uint16_t n);
// void ws_setPixelColor_brightness(uint16_t n,uint32_t RGBColor,uint8_t brightness);
// uint8_t ws_pixel_fadeOut(uint16_t n,uint8_t wait);
// uint8_t ws_pixel_fadeIn(uint16_t n,uint8_t wait);
// uint8_t ws_pixel_fadeInOut(uint16_t n,uint8_t wait);
// uint8_t ws_effect_slide_in(uint16_t n,uint8_t wait);
// uint8_t ws_effect_slide_out(uint16_t n,uint8_t wait);



// Effect...
// void ws_rainbow(uint8_t wait);
// void ws_fadeOut(uint16_t n , uint8_t wait);
// void ws_fadeOutAll(uint8_t wait);
// void ws_fadeIn(uint16_t n , uint8_t wait,uint32_t GRBcolor);
// void ws_fadeInAll(uint8_t wait,uint32_t GRBcolor);
// void ws_brightness_resetAll(uint8_t brightness);
// void ws_pixel_state_reset(uint16_t n,uint16_t brightness);
// void ws_fadeToBlack(uint16_t n , uint8_t fadeValue,uint32_t GRBcolor);

// 定时器
void EVERY_N_MILLISECONDS(int wait, void(*callback)());


// set pixel color
void ws_setPixelColor(uint16_t n, uint32_t GRBcolor);

// 初始化 & 显示
void ws_init(void);
void ws_setBrightness(uint8_t brightness);
void ws_clearAll(void);
void ws_pixel_to_buffer();
void ws_show(void);
_weak_ void show_handler(uint8_t *ws_buffer);


// fade in & fade out
void ws_effect_fadeIn(uint16_t n, uint8_t wait);
void ws_effect_fadeToBlack(uint16_t n, uint8_t wait);
void ws_effect_slide_in(uint16_t n,uint8_t lens, uint8_t wait);

#endif // WS_DRIVER_H