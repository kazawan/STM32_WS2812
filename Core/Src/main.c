/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2024 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "dma.h"
#include "spi.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "ws_driver.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
enum
{
  idle,
  pressing,
  pressed,
  released
} KEY_STATE;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
// WS2812B_TypeDef ws[ws_num];
// extern PIXEL_COLOR[PIXEL_NUMS];
extern PIXEL_TypeDef PIXEL[PIXEL_NUMS];
int key_state[2] = {idle, idle};
int time[2] = {0, 0};
int brightness[2] = {255, 255};

void show_handler(uint8_t *ws_buffer)
{
  HAL_SPI_Transmit_DMA(&hspi1, ws_buffer, PIXEL_NUMS * PIXEL_BITS_NUMS);
  HAL_Delay(1);
}

//! example for fade in and fade out effect
// void key_handler(int key_state, int key_num)
// {
//   switch (key_state)
//   {
//   case idle:
//     ws_brightness_resetAll(0);
//     ws_setAllPixelColor(ws_color(0, 0, 0));
//     break;
//   case pressed:
//     ws_fadeInAll(10, ws_color(127, 127, 0));

//     break;
//   case released:

//     ws_fadeOutAll(50);
//     break;
//   default:
//     break;
//   }
// }

// int key_state = idle;

// void time_handler()
// {
//   key_state++;
//   if (key_state > released)
//   {
//     key_state = idle;
//   }
// }

//! -----------------

//! example water light effect
// int water_light_state = idle;
// int count = 0;
// int start_pixel = 0;
// int key_press = idle;

// void key_press_timer()
// {
//   key_press++;
//   if (key_press > released)
//   {
//     key_press = idle;
//   }
// }

// void key_press_handler()
// {
//   switch (key_press)
//   {
//   case idle:
//     count = 0;
//     for (int i = 0; i < 8; i++)
//     {
//       ws_pixel_state_reset(i, 0);
//       ws_setPixelColor(i, ws_color(0, 0, 0));
//     }

//     break;
//   case pressing:

//     if (HAL_GetTick() - time[1] > 30)
//     {
//       time[1] = HAL_GetTick();
//       count++;
//       if (count > 8)
//       {
//         count = 8;
//       }
//       for (int i = 0; i < count; i++)
//       {
//         ws_fadeIn(i, 10, ws_color(0, 255, 255));
//       }
//     }

//     // key_press = idle;
//     break;
//   case pressed:
//     ws_brightness_resetAll(255);

//     count = 0;
//     break;
//   case released:
//     if (HAL_GetTick() - time[0] > 30)
//     {
//       time[0] = HAL_GetTick();
//       for (int i = 0; i < count; i++)
//       {
//         ws_fadeOut(i, 10);
//       }
//       count++;
//       if (count > 8)
//       {
//         count = 8;
//       }
//     }
//     // ws_fadeOutAll(10);
//     break;
//   default:
//     break;
//   }
// }

//! ----------------------------

//! key fader example
int key_fader_state = idle;
int key_fader_state_2 = idle;
int key_timer = 0;
int key_timer_2 = 0;
int start = 2;
int start_2 = 6;
int len = 0;
int len_2 = 0;
int target = 4;

int width = 4;
int height = 2;
void handle_key_fader()
{
  switch (key_fader_state)
  {
  case idle:

    break;
  case pressing:
    // 延时增加长度
    if (HAL_GetTick() - key_timer > 33)
    {
      key_timer = HAL_GetTick();
      len++;
      if (len > target)
      {
        len = target;
      }
    }
    // 每次增加的亮度
    uint8_t step = 255 / len;
   
    int layer = 0;
    if (start < 4)
    {
      layer = 0;
    }
    else
    {
      layer = 1;
    }
    int toRight = width * (layer + 1) - 1;
    int toLeft = width * (layer + 1) - width;
    // to left

    for (int i = 0; i < len; i++)
    {
      int pos = start - i;
      if (pos < toLeft) // 边缘检测
      {
        // return;
      }
      else
      {
      
          
        ws_setPixelColor(pos, ws_color(255, 0, 255));
        PIXEL[pos].brightness = i * step + 64 >= 255 ? 255 : i * step + 64;
      }
    }
    // to right

    for (int i = 0; i < len; i++)
    {

      int pos = start + i;
      if (pos > toRight) // 边缘检测
      {
        // return;
      }
      else
      {
        ws_setPixelColor(pos, ws_color(255, 0, 255));
        // PIXEL[pos].brightness = i * 5 + 60;
        PIXEL[pos].brightness = i * step + 64 >= 255 ? 255 : i * step + 64;
      }
    }


    break;
  case pressed:

    break;
  case released:
    len = 0;

    break;
  default:
    break;
  }
}

void handle_key_fader2()
{
  switch (key_fader_state_2)
  {
  case idle:

    break;
  case pressing:
    if (HAL_GetTick() - key_timer_2 > 33)
    {
      key_timer_2 = HAL_GetTick();
      len_2++;
      if (len_2 > target)
      {
        len_2 = target;
      }
    }

    uint8_t step = 255 / len_2;
    int layer2 = 0;
    if (start_2 < 4)
    {
      layer2 = 0;
    }
    else
    {
      layer2 = 1;
    }

    for (int i = 0; i < len_2; i++)
    {
      int pos = start_2 - i;
      if (pos < width * (layer2 + 1) - width)
      {
        // return;
      }
      else
      {
        ws_setPixelColor(pos, ws_color(255, 0, 255));

        PIXEL[pos].brightness = i * step + 64 >= 255 ? 255 : i * step + 64;
      }
    }
    // to right
    for (int i = 0; i < len_2; i++)
    {

      int pos = start_2 + i;
      if (pos > width * (layer2 + 1) - 1)
      {
        // return;
      }
      else
      {

        ws_setPixelColor(pos, ws_color(255, 0, 255));
        // PIXEL[pos].brightness = i * 5 + 60;
        PIXEL[pos].brightness = i * step + 64 >= 255 ? 255 : i * step + 64;
      }
    }

    // to left

    break;
  case pressed:

    break;
  case released:
    len_2 = 0;
    break;
  default:
    break;
  }
}
void key_fader_timer()
{
  key_fader_state++;
  if (key_fader_state > released)
  {
    key_fader_state = idle;
  }
}

void key_fader_timer2()
{
  key_fader_state_2++;
  if (key_fader_state_2 > released)
  {
    key_fader_state_2 = idle;
  }
}

//! ----------------------------

/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_SPI1_Init();
  /* USER CODE BEGIN 2 */

  ws_init();

  ws_clearAll();
  ws_show();
  ws_setBrightness(0);
  // int key_state_1 = idle;

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

    //! example for fade in and fade out effect
    // EVERY_N_MILLISECONDS(1000, time_handler);
    // key_handler(key_state, 0);
    //! --------------------------------------------

    //! example water light effect
    // EVERY_N_MILLISECONDS(500, key_press_timer);
    // key_press_handler();
    //! ----------------------------

    //! key fader example

    handle_key_fader();
    handle_key_fader2();
    // EVERY_N_MILLISECONDS(500, key_fader_timer);
    // EVERY_N_MILLISECONDS(1000, key_fader_timer2);

    //! ----------------------------

    for (int i = 0; i < 8; i++)
    {
      ws_effect_fadeToBlack(i, 5);
    }

    ws_pixel_to_buffer();
    ws_show();

    if (HAL_GetTick() - time[0] > 500)
    {
      key_fader_timer();
      time[0] = HAL_GetTick();
    }
    if (HAL_GetTick() - time[1] > 1000)
    {
      key_fader_timer2();
      time[1] = HAL_GetTick();
    }
  }

  /* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
   * in the RCC_OscInitTypeDef structure.
   */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
   */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
