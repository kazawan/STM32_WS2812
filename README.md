# STM32_WS2812

v0.2.0

ws_driver新增
```c
//todo 新的灯珠控制
uint8_t ws_getPixelBrightness(uint16_t n);
void ws_setPixelColor_brightness(uint16_t n,uint32_t RGBColor,uint8_t brightness);
uint8_t ws_pixel_fadeOut(uint16_t n,uint8_t wait);
uint8_t ws_pixel_fadeIn(uint16_t n,uint8_t wait);
uint8_t ws_pixel_fadeInOut(uint16_t n,uint8_t wait);
```
