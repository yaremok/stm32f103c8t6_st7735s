/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
SPI_HandleTypeDef hspi1;
DMA_HandleTypeDef hdma_spi1_tx;

/* USER CODE BEGIN PV */
extern int16_t _width;       								///< (oriented) display width
extern int16_t _height;      								///< (oriented) display height

uint64_t shift_ms = 1778284790123ULL; // Fri May 08 2026 23:59:50 GMT+0000
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_SPI1_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

int randInt(int a, int b)
{
  // случайное число в диапазоне [a; b]
  int r = a + rand() % (b - a + 1);
  return r;
}


char* time_str()
{
  static char str[9];
  
  uint32_t sys_ms = HAL_GetTick();
  uint64_t ms = sys_ms + shift_ms;

  uint64_t total_seconds = ms / 1000;

  uint32_t seconds = total_seconds % 60;
  uint32_t minutes = (total_seconds / 60) % 60;
  uint32_t hours   = (total_seconds / 3600) % 24;

  snprintf(str, sizeof(str),
             "%02lu:%02lu:%02lu",
             hours, minutes, seconds);

    return str;
}


static uint8_t sw_r = 0;
static uint8_t sw_g = 0;
static uint8_t sw_b = 0;
static int sw_dr = 5;
static int sw_dg = 3;
static int sw_db = 2;

uint16_t swingColor()
{
  if (sw_r + sw_dr > 255 || sw_r + sw_dr < 0)
  {
    sw_dr = -sw_dr;
  }
  sw_r += sw_dr;
  if (sw_g + sw_dg > 255 || sw_g + sw_dg < 0)
  {
    sw_dg = -sw_dg;
  }
  sw_g += sw_dg;
  if (sw_b + sw_db > 255 || sw_b + sw_db < 0)
  {
    sw_db = -sw_db;
  }
  sw_b += sw_db;
  uint16_t c = color565(sw_r, sw_g, sw_b);
  return c;
}
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
  Displ_Init(Displ_Orientat_0);		// initialize the display and set the initial display orientation (here is orientaton: 0°) - THIS FUNCTION MUST PRECEED ANY OTHER DISPLAY FUNCTION CALL.
  Displ_CLS(BLACK);			// after initialization (above) and before turning on backlight (below), you can draw the initial display appearance. (here I'm just clearing display with a black background)
  Displ_BackLight('I');  			// initialize backlight and turn it on at init level
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

  /**/
  srand(time(NULL));

  Displ_DrawBackground();

  int w = 106;
  int h = 36;
  int cx = w/2;
  int cy = h/2;
  int r = 5;
  int x0 = cx - w/2;
  int y0 = cy - h/2;
  int x1 = cx + w/2;
  int y1 = cy + h/2;
  int sx = 0;
  int sy = 0;
  int sx_min = 0;
  int sy_min = 0;
  int sx_max = 128 - 1 - w;
  int sy_max = 128 - 1 - h;
  int vx = 0;
  int vy = 0;
  int v_change_prob = 2;
  int v_max = 1;

  int o_x0 = x0;
  int o_y0 = y0;
  int n_x0 = x0;
  int n_y0 = y0;

  /*
  // screen size test
  int x1, y1, x2, y2,
      w = _width,
      h = _height,
      d = 2;

  uint16_t color = RED;

  x1 = 0;
  y1 = 0;
  x2 = w - 1;
  y2 = h - 1;
  color = RED;
  // diagonal line with arrows
  Displ_Line(x1, y1, x2, y2, color);
  Displ_Line(x1, y1, x1 + d, y1, color);
  Displ_Line(x1, y1, x1, y1 + d, color);
  Displ_Line(x2 - d, y2, x2, y2, color);
  Displ_Line(x2, y2 - d, x2, y2, color);

  Displ_Border(0, 0, w, h, 2, CYAN);
  */

  while (1)
  {
	  // Displ_PerfTest();

    // hide in old position
    // Displ_DrawBackgroundPart(o_x0, o_y0, w, h);

    // define new position
    int ch_r = randInt(0, 100);
    if (ch_r <= v_change_prob)
    {
      vx = randInt(-v_max, v_max);
      vy = randInt(-v_max, v_max);
    }
    int nsx = sx + vx;
    if (nsx >= sx_max || nsx <= sx_min)
    {
      vx = -vx;
      nsx = sx;
    }
    int nsy = sy + vy;
    if (nsy >= sy_max || nsy <= sy_min)
    {
      vy = -vy;
      nsy = sy;
    }
    sx = nsx;
    sy = nsy;
    n_x0 = x0 + sx;
    n_y0 = y0 + sy;

    // draw in new position
    //Displ_fillRoundRect(n_x0, n_y0, w, h, r, GREEN);
    //Displ_FillArea(n_x0, n_y0, w, h, GREEN);
    Displ_CString(
      n_x0 + r, 
      n_y0 + r, 
      x1 + sx - 1 - r, 
      y1 + sy - 1 - r, 
      time_str(), 
      Font16, 
      1, 
      BLACK, 
      WHITE
    );

    // glith fix
    Displ_Line(x1 + sx - 1 - r, n_y0 + r, x1 + sx - 1 - r, y1 + sy - 1 - r, WHITE);

    Displ_Border(n_x0, n_y0, w, h, r, swingColor());
    
    // fill new-old difference
    Displ_DrawBackgroundDifference(
        o_x0,
        o_y0,
        w,
        h,
        n_x0,
        n_y0
    );

    // remember new position
    o_x0 = n_x0;
    o_y0 = n_y0;

    /**/

    HAL_Delay(40);

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
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
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV8;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_1LINE;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_HARD_OUTPUT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Channel3_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel3_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel3_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, DISPL_RST_Pin|DISPL_LED_Pin|DISPL_DC_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : DISPL_RST_Pin DISPL_LED_Pin DISPL_DC_Pin */
  GPIO_InitStruct.Pin = DISPL_RST_Pin|DISPL_LED_Pin|DISPL_DC_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : BOOT1_Pin */
  GPIO_InitStruct.Pin = BOOT1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(BOOT1_GPIO_Port, &GPIO_InitStruct);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
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

#ifdef  USE_FULL_ASSERT
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
