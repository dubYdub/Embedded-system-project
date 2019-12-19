/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "lcd.h"
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
UART_HandleTypeDef huart1;
/* USER CODE BEGIN PV */
int xoffset = 0;
int if_bottom = 0;
int record[16][10];
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART1_UART_Init(void);
/* USER CODE BEGIN PFP */
static void showStatus(int levelNum, int scoreNum, int next_shape1, int next_shape2);
_Bool GetPress(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, GPIO_PinState PinState);
void draw_model_1(int rotation, int drop, int clear);
void draw_model_2(int rotation, int drop, int clear);
void draw_model_3(int rotation, int drop, int clear);
void draw_model_4(int rotation, int drop, int clear);
void draw_model_5(int rotation, int drop, int clear);
void draw_model_6(int rotation, int drop, int clear);
void draw_model_7(int rotation, int drop, int clear);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

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
    LCD_Init();
    /* USER CODE END Init */

    /* Configure the system clock */
    SystemClock_Config();
    /* USER CODE BEGIN SysInit */
    /* USER CODE END SysInit */

    /* Initialize all configured peripherals */
    MX_GPIO_Init();
    MX_USART1_UART_Init();

    /* USER CODE BEGIN 2 */
    int next_shape1 = 0;
    int next_shape2 = 0;
    int levelNum = 0;
    int scoreNum = 0;
    int rotation = 0;
    int drop = 0;
    int speed = 1;
    // Draw the strings in the status bar.
    LCD_ShowString(205, 20, 30, 15, 12, "Next");
    LCD_ShowString(205, 180, 30, 15, 12, "Level");
    LCD_ShowString(205, 250, 30, 15, 12, "Score");
    // Draw the status bar.
    LCD_Fill(201, 0, 203, 320, GRAY);
    /* USER CODE END 2 */

    /* Infinite loop */
    /* USER CODE BEGIN WHILE */
    while (1)
    {
        /* USER CODE END WHILE */

        /* USER CODE BEGIN 3 */
        BACK_COLOR = WHITE;
        showStatus(levelNum, scoreNum, next_shape1, next_shape2);
        draw_model_5(rotation, drop, 0);
        // Shape moves to the right.
        if (GetPress(KEY0_GPIO_Port, KEY0_Pin, GPIO_PIN_RESET) == 1)
        {
            HAL_GPIO_TogglePin(LED0_GPIO_Port, LED0_Pin);
            draw_model_5(rotation, drop, 1);
            xoffset += 20;
            draw_model_5(rotation, drop, 0);
        }
        // Shape rotates.
        if (GetPress(KEY1_GPIO_Port, KEY1_Pin, GPIO_PIN_RESET) == 1)
        {
            HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
            draw_model_5(rotation, drop, 1);
            rotation += 1;
            draw_model_5(rotation, drop, 0);
        }
        // Shape moves to the left.
        if (GetPress(KEY_WK_GPIO_Port, KEY_WK_Pin, GPIO_PIN_SET) == 1)
        {
            HAL_GPIO_TogglePin(LED0_GPIO_Port, LED0_Pin);
            HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
            draw_model_5(rotation, drop, 1);
            xoffset -= 20;
            draw_model_5(rotation, drop, 0);
        }
        HAL_Delay(40);
        if (if_bottom == 0)
        {
            draw_model_5(rotation, drop, 1);
            drop += speed;
        }
        else
        {
            // Once this shape reach the bottom, the next shape drops.
            if_bottom = 0;
            drop = 0;
            rotation = 0;
            xoffset = 0;
        }
        /* USER CODE END 3 */
    }
}
/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    /** Initializes the CPU, AHB and APB busses clocks
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
    /** Initializes the CPU, AHB and APB busses clocks
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

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

    /* USER CODE BEGIN USART1_Init 0 */

    /* USER CODE END USART1_Init 0 */

    /* USER CODE BEGIN USART1_Init 1 */

    /* USER CODE END USART1_Init 1 */
    huart1.Instance = USART1;
    huart1.Init.BaudRate = 115200;
    huart1.Init.WordLength = UART_WORDLENGTH_8B;
    huart1.Init.StopBits = UART_STOPBITS_1;
    huart1.Init.Parity = UART_PARITY_NONE;
    huart1.Init.Mode = UART_MODE_TX_RX;
    huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart1.Init.OverSampling = UART_OVERSAMPLING_16;
    if (HAL_UART_Init(&huart1) != HAL_OK)
    {
        Error_Handler();
    }
    /* USER CODE BEGIN USART1_Init 2 */

    /* USER CODE END USART1_Init 2 */
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    /* GPIO Ports Clock Enable */
    __HAL_RCC_GPIOD_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin, GPIO_PIN_SET);

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET);

    /*Configure GPIO pin : KEY_WK_Pin */
    GPIO_InitStruct.Pin = KEY_WK_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    HAL_GPIO_Init(KEY_WK_GPIO_Port, &GPIO_InitStruct);

    /*Configure GPIO pin : KEY0_Pin */
    GPIO_InitStruct.Pin = KEY0_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(KEY0_GPIO_Port, &GPIO_InitStruct);

    /*Configure GPIO pin : LED0_Pin */
    GPIO_InitStruct.Pin = LED0_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(LED0_GPIO_Port, &GPIO_InitStruct);

    /*Configure GPIO pin : KEY1_Pin */
    GPIO_InitStruct.Pin = KEY1_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(KEY1_GPIO_Port, &GPIO_InitStruct);

    /*Configure GPIO pin : LED1_Pin */
    GPIO_InitStruct.Pin = LED1_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(LED1_GPIO_Port, &GPIO_InitStruct);
}

/* USER CODE BEGIN 4 */
static void showStatus(int levelNum, int scoreNum, int next_shape1, int next_shape2)
{
    // Draw the number of levels and scores.
    char levelMsg[5];
    char scoreMsg[5];
    sprintf(levelMsg, "%d", levelNum);
    sprintf(scoreMsg, "%d", scoreNum);
    LCD_ShowString(205, 210, 15, 15, 12, levelMsg);
    LCD_ShowString(205, 280, 15, 15, 12, scoreMsg);
    // Draw the shapes of the coming nodes 2.
    switch (next_shape1)
    {
    /*
  ... ... ... ...
  ... ... ... ...

  */
    case 0:
        LCD_Fill(205, 75, 235, 85, BLUE);
        break;
    /*
  ... ...
  ... ...
  ... ...
  ... ...

  */
    case 1:
        LCD_Fill(210, 70, 230, 90, WHITE);
        break;
    /*
  ... ...
  ... ...
      ... ...
      ... ...

  */
    case 2:
        LCD_Fill(205, 70, 225, 80, WHITE);
        LCD_Fill(215, 80, 235, 90, WHITE);
        break;
    /*
      ... ...
      ... ...
  ... ...
  ... ...

  */
    case 3:
        LCD_Fill(215, 70, 235, 80, WHITE);
        LCD_Fill(205, 80, 225, 90, WHITE);
        break;
    /*
  ...
  ...
  ... ... ...
  ... ... ...

  */
    case 4:
        LCD_Fill(205, 70, 215, 80, WHITE);
        LCD_Fill(205, 80, 235, 90, WHITE);
        break;
    /*
          ...
          ...
  ... ... ...
  ... ... ...

  */
    case 5:
        LCD_Fill(225, 70, 235, 80, WHITE);
        LCD_Fill(205, 80, 235, 90, WHITE);
        break;
    /*
      ...
      ...
  ... ... ...
  ... ... ...

  */
    case 6:
        LCD_Fill(205, 70, 215, 80, WHITE);
        LCD_Fill(195, 80, 225, 90, WHITE);
        break;
    }

    // Draw the shapes of the coming nodes 2. Center:(220, 120)
    int distance = 40;
    switch (next_shape2)
    {
    /*
  ... ... ... ...
  ... ... ... ...

  */
    case 0:
        LCD_Fill(205, 75 + distance, 235, 85 + distance, BLUE);
        break;
    /*
  ... ...
  ... ...
  ... ...
  ... ...

  */
    case 1:
        LCD_Fill(210, 70 + distance, 230, 90 + distance, WHITE);
        break;
    /*
  ... ...
  ... ...
      ... ...
      ... ...

  */
    case 2:
        LCD_Fill(205, 70 + distance, 225, 80 + distance, WHITE);
        LCD_Fill(215, 80 + distance, 235, 90 + distance, WHITE);
        break;
    /*
      ... ...
      ... ...
  ... ...
  ... ...

  */
    case 3:
        LCD_Fill(215, 70 + distance, 235, 80 + distance, WHITE);
        LCD_Fill(205, 80 + distance, 225, 90 + distance, WHITE);
        break;
    /*
  ...
  ...
  ... ... ...
  ... ... ...

  */
    case 4:
        LCD_Fill(205, 70 + distance, 215, 80 + distance, WHITE);
        LCD_Fill(205, 80 + distance, 235, 90 + distance, WHITE);
        break;
    /*
          ...
          ...
  ... ... ...
  ... ... ...

  */
    case 5:
        LCD_Fill(225, 70 + distance, 235, 80 + distance, WHITE);
        LCD_Fill(205, 80 + distance, 235, 90 + distance, WHITE);
        break;
    /*
      ...
      ...
  ... ... ...
  ... ... ...

  */
    case 6:
        LCD_Fill(205, 70 + distance, 215, 80 + distance, WHITE);
        LCD_Fill(195, 80 + distance, 225, 90 + distance, WHITE);
        break;
    }
}

// Check whether the key is pressed.
_Bool GetPress(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, GPIO_PinState PinState)
{
    if (HAL_GPIO_ReadPin(GPIOx, GPIO_Pin) == PinState)
    {
        HAL_Delay(10);
        if (HAL_GPIO_ReadPin(GPIOx, GPIO_Pin) == PinState)
        {
            while (HAL_GPIO_ReadPin(GPIOx, GPIO_Pin) == PinState)
            {
                ;
            }
            return 1;
        }
        else
            return 0;
    }
    else
        return 0;
}

// Draw seven models。
void draw_model_1(int rotation, int drop, int clear)
{
    int x1 = 0;
    int y1 = 0;
    int x2 = 0;
    int y2 = 0;
    if (rotation % 2 == 0)
    {
        x1 = 60 + xoffset;
        if (x1 >= 120)
        {
            x1 = 120;
            xoffset = 60;
        }
        if (x1 <= 0)
        {
            x1 = 0;
            xoffset = -60;
        }
        y1 = 0 + drop;
        x2 = 140 + xoffset;
        y2 = 20 + drop;
    }
    else
    {
        x1 = 80 + xoffset;
        if (x1 >= 180)
        {
            x1 = 180;
            xoffset = 100;
        }
        if (x1 <= 0)
        {
            x1 = 0;
            xoffset = -80;
        }
        y1 = 0 + drop;
        x2 = 100 + xoffset;
        y2 = 80 + drop;
    }
    if (clear == 0)
    {
        LCD_Fill(x1, y1, x2, y2, YELLOW);
        if (y2 == 320)
        {
            if_bottom = 1;
        }
    }
    else
    {
        LCD_Fill(x1, y1, x2, y2, WHITE);
    }
}
void draw_model_2(int rotation, int drop, int clear)
{

    int x1 = 80 + xoffset;
    if (x1 >= 160)
    {
        x1 = 160;
        xoffset = 80;
    }
    if (x1 <= 0)
    {
        x1 = 0;
        xoffset = -80;
    }
    int y1 = 0 + drop;
    int x2 = 120 + xoffset;
    int y2 = 40 + drop;
    if (clear == 0)
    {
        LCD_Fill(x1, y1, x2, y2, BLUE);
        if (y2 == 320)
        {
            if_bottom = 1;
        }
    }
    else
    {
        LCD_Fill(x1, y1, x2, y2, WHITE);
    }
}
void draw_model_3(int rotation, int drop, int clear)
{
    int x1 = 0;
    int y1 = 0;
    int x2 = 0;
    int y2 = 0;
    int x3 = 0;
    int y3 = 0;
    int x4 = 0;
    int y4 = 0;
    if (rotation % 2 == 0)
    {
        if (80 + xoffset <= 0)
        {
            xoffset = -80;
        }
        if (100 + xoffset >= 160)
        {
            xoffset = 60;
        }
        x1 = 80 + xoffset;
        y1 = 0 + drop;
        x2 = 120 + xoffset;
        y2 = 20 + drop;
        x3 = 100 + xoffset;
        y3 = 20 + drop;
        x4 = 140 + xoffset;
        y4 = 40 + drop;
    }
    else
    {
        if (100 + xoffset <= 0)
        {
            xoffset = -100;
        }
        if (120 + xoffset >= 180)
        {
            xoffset = 60;
        }
        x1 = 100 + xoffset;
        x3 = 120 + xoffset;
        y1 = 20 + drop;
        x2 = 120 + xoffset;
        y2 = 60 + drop;
        y3 = 0 + drop;
        x4 = 140 + xoffset;
        y4 = 40 + drop;
    }
    if (clear == 0)
    {
        LCD_Fill(x1, y1, x2, y2, GREEN);
        LCD_Fill(x3, y3, x4, y4, GREEN);
        if (y2 == 320 || y4 == 320)
        {
            if_bottom = 1;
        }
    }
    else
    {
        LCD_Fill(x1, y1, x2, y2, WHITE);
        LCD_Fill(x3, y3, x4, y4, WHITE);
    }
}
void draw_model_4(int rotation, int drop, int clear)
{
    int x1 = 0;
    int y1 = 0;
    int x2 = 0;
    int y2 = 0;
    int x3 = 0;
    int y3 = 0;
    int x4 = 0;
    int y4 = 0;
    if (rotation % 2 == 0)
    {
        if (80 + xoffset <= 0)
        {
            xoffset = -80;
        }
        if (100 + xoffset >= 160)
        {
            xoffset = 60;
        }
        x1 = 100 + xoffset;
        y1 = 0 + drop;
        x2 = 140 + xoffset;
        y2 = 20 + drop;
        x3 = 80 + xoffset;
        y3 = 20 + drop;
        x4 = 120 + xoffset;
        y4 = 40 + drop;
    }
    else
    {
        if (80 + xoffset <= 0)
        {
            xoffset = -80;
        }
        if (100 + xoffset >= 180)
        {
            xoffset = 80;
        }
        x1 = 80 + xoffset;
        y1 = 0 + drop;
        x2 = 100 + xoffset;
        y2 = 40 + drop;
        x3 = 100 + xoffset;
        y3 = 20 + drop;
        x4 = 120 + xoffset;
        y4 = 60 + drop;
    }
    if (clear == 0)
    {
        LCD_Fill(x1, y1, x2, y2, RED);
        LCD_Fill(x3, y3, x4, y4, RED);
        if (y2 == 320 || y4 == 320)
        {
            if_bottom = 1;
        }
    }
    else
    {
        LCD_Fill(x1, y1, x2, y2, WHITE);
        LCD_Fill(x3, y3, x4, y4, WHITE);
    }
}
void draw_model_5(int rotation, int drop, int clear)
{
    int x1 = 0;
    int y1 = 0;
    int x2 = 0;
    int y2 = 0;
    int x3 = 0;
    int y3 = 0;
    int x4 = 0;
    int y4 = 0;
    if (rotation % 4 == 0)
    {
        if (80 + xoffset <= 0)
        {
            xoffset = -80;
        }
        if (80 + xoffset >= 140)
        {
            xoffset = 60;
        }
        x1 = 80 + xoffset;
        y1 = 0 + drop;
        x2 = 100 + xoffset;
        y2 = 20 + drop;
        x3 = 80 + xoffset;
        y3 = 20 + drop;
        x4 = 140 + xoffset;
        y4 = 40 + drop;
    }
    else if (rotation % 4 == 1)
    {
        if (100 + xoffset >= 180)
        {
            xoffset = 80;
        }
        if (80 + xoffset <= 0)
        {
            xoffset = -80;
        }
        x1 = 100 + xoffset;
        y1 = 0 + drop;
        x2 = 120 + xoffset;
        y2 = 20 + drop;
        x3 = 80 + xoffset;
        y3 = 0 + drop;
        x4 = 100 + xoffset;
        y4 = 60 + drop;
    }
    else if (rotation % 4 == 2)
    {
        if (80 + xoffset <= 0)
        {
            xoffset = -80;
        }
        if (120 + xoffset >= 180)
        {
            xoffset = 60;
        }
        x1 = 80 + xoffset;
        y1 = 0 + drop;
        x2 = 140 + xoffset;
        y2 = 20 + drop;
        x3 = 120 + xoffset;
        y3 = 20 + drop;
        x4 = 140 + xoffset;
        y4 = 40 + drop;
    }
    else if (rotation % 4 == 3)
    {
        if (80 + xoffset <= 0)
        {
            xoffset = -80;
        }
        if (100 + xoffset >= 180)
        {
            xoffset = 80;
        }
        x1 = 80 + xoffset;
        y1 = 40 + drop;
        x2 = 100 + xoffset;
        y2 = 60 + drop;
        x3 = 100 + xoffset;
        y3 = 0 + drop;
        x4 = 120 + xoffset;
        y4 = 60 + drop;
    }
    if (clear == 0)
    {
        LCD_Fill(x1, y1, x2, y2, CYAN);
        LCD_Fill(x3, y3, x4, y4, CYAN);
        if (y2 == 320 || y4 == 320)
        {
            if_bottom = 1;
        }
    }
    else
    {
        LCD_Fill(x1, y1, x2, y2, WHITE);
        LCD_Fill(x3, y3, x4, y4, WHITE);
    }
}
void draw_model_6(int rotation, int drop, int clear)
{
    int x1 = 0;
    int y1 = 0;
    int x2 = 0;
    int y2 = 0;
    int x3 = 0;
    int y3 = 0;
    int x4 = 0;
    int y4 = 0;
    if (rotation % 4 == 0)
    {
        if (120 + xoffset >= 180)
        {
            xoffset = 60;
        }
        if (80 + xoffset <= 0)
        {
            xoffset = -80;
        }
        x1 = 120 + xoffset;
        y1 = 0 + drop;
        x2 = 140 + xoffset;
        y2 = 20 + drop;
        x3 = 80 + xoffset;
        y3 = 20 + drop;
        x4 = 140 + xoffset;
        y4 = 40 + drop;
    }
    else if (rotation % 4 == 1)
    {
        if (80 + xoffset <= 0)
        {
            xoffset = -80;
        }
        if (100 + xoffset >= 180)
        {
            xoffset = 80;
        }
        x1 = 80 + xoffset;
        y1 = 0 + drop;
        x2 = 100 + xoffset;
        y2 = 60 + drop;
        x3 = 100 + xoffset;
        y3 = 40 + drop;
        x4 = 120 + xoffset;
        y4 = 60 + drop;
    }
    else if (rotation % 4 == 2)
    {
        if (80 + xoffset <= 0)
        {
            xoffset = -80;
        }
        if (80 + xoffset >= 140)
        {
            xoffset = 60;
        }
        x1 = 80 + xoffset;
        y1 = 0 + drop;
        x2 = 140 + xoffset;
        y2 = 20 + drop;
        x3 = 80 + xoffset;
        y3 = 20 + drop;
        x4 = 100 + xoffset;
        y4 = 40 + drop;
    }
    else if (rotation % 4 == 3)
    {
        if (80 + xoffset <= 0)
        {
            xoffset = -80;
        }
        if (100 + xoffset >= 180)
        {
            xoffset = 80;
        }
        x1 = 80 + xoffset;
        y1 = 0 + drop;
        x2 = 100 + xoffset;
        y2 = 20 + drop;
        x3 = 100 + xoffset;
        y3 = 0 + drop;
        x4 = 120 + xoffset;
        y4 = 60 + drop;
    }
    if (clear == 0)
    {
        LCD_Fill(x1, y1, x2, y2, GRAY);
        LCD_Fill(x3, y3, x4, y4, GRAY);
        if (y2 == 320 || y4 == 320)
        {
            if_bottom = 1;
        }
    }
    else
    {
        LCD_Fill(x1, y1, x2, y2, WHITE);
        LCD_Fill(x3, y3, x4, y4, WHITE);
    }
}
void draw_model_7(int rotation, int drop, int clear)
{
    int x1 = 0;
    int y1 = 0;
    int x2 = 0;
    int y2 = 0;
    int x3 = 0;
    int y3 = 0;
    int x4 = 0;
    int y4 = 0;
    if (rotation % 4 == 0)
    {
        if (80 + xoffset <= 0)
        {
            xoffset = -80;
        }
        if (80 + xoffset >= 140)
        {
            xoffset = 60;
        }
        x1 = 100 + xoffset;
        y1 = 0 + drop;
        x2 = 120 + xoffset;
        y2 = 20 + drop;
        x3 = 80 + xoffset;
        y3 = 20 + drop;
        x4 = 140 + xoffset;
        y4 = 40 + drop;
    }
    else if (rotation % 4 == 1)
    {
        if (80 + xoffset <= 0)
        {
            xoffset = -80;
        }
        if (100 + xoffset >= 180)
        {
            xoffset = 80;
        }
        x1 = 80 + xoffset;
        y1 = 0 + drop;
        x2 = 100 + xoffset;
        y2 = 60 + drop;
        x3 = 100 + xoffset;
        y3 = 20 + drop;
        x4 = 120 + xoffset;
        y4 = 40 + drop;
    }
    else if (rotation % 4 == 2)
    {
        if (80 + xoffset <= 0)
        {
            xoffset = -80;
        }
        if (80 + xoffset >= 140)
        {
            xoffset = 60;
        }
        x1 = 80 + xoffset;
        y1 = 0 + drop;
        x2 = 140 + xoffset;
        y2 = 20 + drop;
        x3 = 100 + xoffset;
        y3 = 20 + drop;
        x4 = 120 + xoffset;
        y4 = 40 + drop;
    }
    else if (rotation % 4 == 3)
    {
        if (80 + xoffset <= 0)
        {
            xoffset = -80;
        }
        if (100 + xoffset >= 180)
        {
            xoffset = 80;
        }
        x1 = 80 + xoffset;
        y1 = 20 + drop;
        x2 = 100 + xoffset;
        y2 = 40 + drop;
        x3 = 100 + xoffset;
        y3 = 0 + drop;
        x4 = 120 + xoffset;
        y4 = 60 + drop;
    }
    if (clear == 0)
    {
        LCD_Fill(x1, y1, x2, y2, BLACK);
        LCD_Fill(x3, y3, x4, y4, BLACK);
        if (y2 == 320 || y4 == 320)
        {
            if_bottom = 1;
        }
    }
    else
    {
        LCD_Fill(x1, y1, x2, y2, WHITE);
        LCD_Fill(x3, y3, x4, y4, WHITE);
    }
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
    /* USER CODE BEGIN Error_Handler_Debug */
    /* User can add his own implementation to report the HAL error return state */

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
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
    /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
