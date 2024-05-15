#include "ws_driver.h"
uint8_t PIXEL_BUFFER[PIXEL_NUMS][PIXEL_BITS_NUMS] = {0};
PIXEL_TypeDef PIXEL[PIXEL_NUMS] = {0};

void ws_init(void)
{
    for (int i = 0; i < PIXEL_NUMS; i++)
    {
        PIXEL[i].color = 0x000000;
        PIXEL[i].brightness = 255;
        PIXEL[i].timer = 0;
    }
}

uint32_t ws_color(uint8_t red, uint8_t green, uint8_t blue)
{
    return green << 16 | red << 8 | blue;
}

uint32_t ws_color_brighness(uint8_t red, uint8_t green, uint8_t blue, uint16_t brightness)
{
    uint8_t r, g, b;
    r = red * brightness / 255;
    g = green * brightness / 255;
    b = blue * brightness / 255;
    return ws_color(r, g, b);
}

uint32_t ws_wheel(uint8_t wheelPos)
{
    wheelPos = 255 - wheelPos;
    if (wheelPos < 85)
    {
        return ws_color(255 - wheelPos * 3, 0, wheelPos * 3);
    }
    if (wheelPos < 170)
    {
        wheelPos -= 85;
        return ws_color(0, wheelPos * 3, 255 - wheelPos * 3);
    }
    wheelPos -= 170;
    return ws_color(wheelPos * 3, 255 - wheelPos * 3, 0);
}

void ws_setPixelRGB(uint16_t n, uint8_t red, uint8_t green, uint8_t blue)
{
    uint8_t i;
    // PIXEL[n].color = ws_color(red, green, blue);
    if (n < PIXEL_NUMS)
    {
        for (i = 0; i < 24; ++i)
        {
            PIXEL_BUFFER[n][i] = (((ws_color(red, green, blue) << i) & 0X800000) ? CODE_H : CODE_L);
        }
    }
}

void ws_setPixelColor(uint16_t n, uint32_t GRBcolor)
{
    uint8_t i;
    PIXEL[n].color = GRBcolor;
    if (n < PIXEL_NUMS)
    {
        for (i = 0; i < 24; ++i)
        {
            PIXEL_BUFFER[n][i] = (((PIXEL[n].color << i) & 0X800000) ? CODE_H : CODE_L);
        }
    }
}

void ws_setAllPixelColor(uint32_t GRBcolor)
{
    for (int i = 0; i < PIXEL_NUMS; i++)
    {
        PIXEL[i].color = GRBcolor;
        for (int j = 0; j < 24; j++)
        {
            PIXEL_BUFFER[i][j] = (((PIXEL[i].color << j) & 0X800000) ? CODE_H : CODE_L);
        }
    }
}

void ws_show()
{
    uint8_t buff[PIXEL_NUMS * PIXEL_BITS_NUMS] = {0};
    for (int i = 0; i < PIXEL_NUMS; i++)
    {
        for (int j = 0; j < PIXEL_BITS_NUMS; j++)
        {
            buff[i * PIXEL_BITS_NUMS + j] = PIXEL_BUFFER[i][j];
        }
    }
    show_handler(buff);
}

void ws_clearAll(void)
{
    for (int i = 0; i < PIXEL_NUMS; i++)
    {
        for (int j = 0; j < PIXEL_BITS_NUMS; j++)
        {
            PIXEL_BUFFER[i][j] = CODE_L;
        }
    }
}

void ws_rainbow(uint8_t wait)
{
    uint16_t i, j;

    for (j = 0; j < 256; j++)
    {
        for (i = 0; i < PIXEL_NUMS; i++)
        {
            ws_setPixelColor(i, ws_wheel((i + j) & 255));
            // ws_set_color_hex(ws281x_wheel((i + j) & 255), ws_buffer, i);
        }
        ws_show();
        HAL_Delay(wait);
    }
}

void ws_fadeOut(uint16_t n, uint8_t wait)
{
    if (HAL_GetTick() - PIXEL[n].timer > wait)
    {
        uint8_t g = (PIXEL[n].color >> 16) & 0XFF;
        uint8_t r = (PIXEL[n].color >> 8) & 0XFF;
        uint8_t b = PIXEL[n].color & 0XFF;
        if (PIXEL[n].brightness > 0)
        {

            PIXEL[n].brightness -= 5;
            PIXEL[n].timer = HAL_GetTick();
            ws_setPixelColor(n, ws_color_brighness(r, g, b, PIXEL[n].brightness));
        }
        if (PIXEL[n].brightness < 0)
        {
            PIXEL[n].brightness = 0;
            PIXEL[n].color = ws_color_brighness(r, g, b, 0);
        }
    }
}

void ws_fadeOutAll(uint8_t wait)
{
    for (int i = 0; i < PIXEL_NUMS; i++)
    {
        ws_fadeOut(i, wait);
    }
}

void ws_pixel_state_reset(uint16_t n, uint16_t brightness)
{
    PIXEL[n].brightness = brightness;
    // PIXEL[n].color = 0x000000;
    PIXEL[n].timer = 0;
}

void ws_brightness_resetAll(uint8_t brightness)
{
    for (int i = 0; i < PIXEL_NUMS; i++)
    {
        ws_pixel_state_reset(i, brightness);
    }
}

void ws_fadeIn(uint16_t n, uint8_t wait, uint32_t GRBcolor)
{
    // static uint8_t brightness  = 0;
    if (HAL_GetTick() - PIXEL[n].timer > wait)
    {
        if (PIXEL[n].brightness < 255)
        {
            uint8_t g = (GRBcolor >> 16) & 0XFF;
            uint8_t r = (GRBcolor >> 8) & 0XFF;
            uint8_t b = GRBcolor & 0XFF;
            PIXEL[n].brightness += 5;
            // PIXEL[n].brightness = brightness + 5;
            PIXEL[n].timer = HAL_GetTick();
            ws_setPixelColor(n, ws_color_brighness(r, g, b, PIXEL[n].brightness));
        }
        if (PIXEL[n].brightness >= 255)
        {
            PIXEL[n].brightness = 255;
            PIXEL[n].color = GRBcolor;
        }
    }
}

void ws_fadeInAll(uint8_t wait, uint32_t GRBcolor)
{
    for (int i = 0; i < PIXEL_NUMS; i++)
    {
        PIXEL[i].color = GRBcolor;
        ws_fadeIn(i, wait, PIXEL[i].color);
    }
}

//! bug
void ws_fadeToBlack(uint16_t n, uint8_t fadeValue, uint32_t GRBcolor)
{
    PIXEL[n].color = GRBcolor;

    uint8_t g = (PIXEL[n].color >> 16) & 0XFF;
    uint8_t r = (PIXEL[n].color >> 8) & 0XFF;
    uint8_t b = PIXEL[n].color & 0XFF;
    if (PIXEL[n].brightness > 0)
    {
        PIXEL[n].brightness -= fadeValue;
        ws_setPixelColor(n, ws_color_brighness(r, g, b, PIXEL[n].brightness));
    }
    if (PIXEL[n].brightness < 0)
    {
        PIXEL[n].brightness = 0;
        PIXEL[n].color = ws_color_brighness(0, 0, 0, 0);
    }
}

void EVERY_N_MILLISECONDS(int wait, void (*callback)())
{
    static int last = 0;
    if (HAL_GetTick() - last > wait)
    {
        last = HAL_GetTick();
        callback();
    }
}

uint8_t ws_getPixelBrightness(uint16_t n)
{
    return PIXEL[n].brightness;
}

void ws_setPixelColor_brightness(uint16_t n, uint32_t RGBColor, uint8_t brightness)
{
    PIXEL[n].color = RGBColor;
    if (brightness > 255)
    {
        brightness = 255;
    }
    PIXEL[n].brightness = brightness;
    uint8_t g = (RGBColor >> 16) & 0XFF * brightness / 255;
    uint8_t r = (RGBColor >> 8) & 0XFF * brightness / 255;
    uint8_t b = RGBColor & 0XFF * brightness / 255;
    if (n < PIXEL_NUMS)
    {
        for (int i = 0; i < 24; i++)
        {
            PIXEL_BUFFER[n][i] = (((ws_color(r, g, b) << i) & 0X800000) ? CODE_H : CODE_L);
        }
    }
}

uint8_t ws_pixel_fadeOut(uint16_t n, uint8_t wait)
{
    // static uint8_t temp = 255;
    if (HAL_GetTick() - PIXEL[n].timer > wait)
    {
        if (PIXEL[n].brightness <= 0)
        {
            return 0;
        }
        uint8_t temp = PIXEL[n].brightness - 5;
        if (temp < 0)
        {
            temp = 0;
        }
        PIXEL[n].timer = HAL_GetTick();
        return temp;
    }
    return PIXEL[n].brightness;
}

uint8_t ws_pixel_fadeIn(uint16_t n, uint8_t wait)
{
    if (HAL_GetTick() - PIXEL[n].timer > wait)
    {
        if (PIXEL[n].brightness >= 255)
        {
            return 255;
        }
        uint8_t temp = PIXEL[n].brightness + 5;
        if (temp > 255)
        {
            temp = 255;
        }
        PIXEL[n].timer = HAL_GetTick();
        return temp;
    }
    return PIXEL[n].brightness;
}

uint8_t ws_pixel_fadeInOut(uint16_t n, uint8_t wait)
{
    static uint8_t trigger = 0;
    if (trigger == 0)
    {
        PIXEL[n].brightness = ws_pixel_fadeIn(n, wait);
        if (PIXEL[n].brightness == 255)
        {
            trigger = 1;
        }
    }
    else
    {
        PIXEL[n].brightness = ws_pixel_fadeOut(n, wait);
        if (PIXEL[n].brightness == 0)
        {
            trigger = 0;
        }
    }
}

uint8_t ws_effect_slide_in(uint16_t n, uint8_t wait)
{
    static uint8_t count = 0;
    
    if (HAL_GetTick() - PIXEL[n].timer > 30)
    {

        for (int i = 0; i < count; i++)
        {

            ws_setPixelColor_brightness(i, ws_color(0, 255, 255), ws_pixel_fadeIn(i, wait));
        }
        count++;
        if (count > 4)
        {
            count = 0;
        }

        PIXEL[n].timer = HAL_GetTick();
    }
}

uint8_t ws_effect_slide_out(uint16_t n, uint8_t wait)
{
    static uint8_t count = 0;
    // if (PIXEL[n ].brightness == 0)
    // {    
    //     // PIXEL[n].timer = HAL_GetTick(); 
    //     count = 0;
    //     return 0;
    // }
    if (HAL_GetTick() - PIXEL[n].timer > 30)
    {
        for (int i = 0; i < count; i++)
        {
            ws_setPixelColor_brightness(i, ws_color(0, 255, 255), ws_pixel_fadeOut(i, wait));
        }
        count++;
        if (count > 4)
        {
            count = 0;
        }

        PIXEL[n].timer = HAL_GetTick();
    }
}
