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
#include <stdio.h>
#include <stdlib.h>
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

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART1_UART_Init(void);
/* USER CODE BEGIN PFP */
_Bool GetPress(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, GPIO_PinState PinState);
void draw_model_1(int lor);
void draw_model_2(int lor);
void draw_model_3(int lor);
void draw_model_4(int lor);
void draw_model_5(int lor);
void draw_model_6(int lor);
void draw_model_7(int lor);
int judge(int x_1, int y_1, int x_2, int y_2);
static void showStatus(int levelNum, int scoreNum, int next_shape1, int next_shape2);
void fill_record(int x_1, int y_1, int x_2, int y_2, int color);
void draw(int shape, int  lor);

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int xoffset = 0;
int if_bottom = 0;
int arrays[16][10];
int temp_arr[16][10];
int rotation = 0;
int drop = 0;
int speed = 2;
int count_row = 0;
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
    LCD_Init();
    /* USER CODE END SysInit */

    /* Initialize all configured peripherals */
    MX_GPIO_Init();
    MX_USART1_UART_Init();
    /* USER CODE BEGIN 2 */
    //  uint32_t y1 = 0;
    //  uint32_t y2 = 20;
    int initial = rand()% 7;
    int next_shape1 = rand()% 7;
    int next_shape2 = rand()% 7;
    int levelNum = 1;
    int scoreNum = 0;




    //  char yy[10];
    /* USER CODE END 2 */

    /* Infinite loop */
    /* USER CODE BEGIN WHILE */
    LCD_ShowString(205, 20, 30, 15, 12, "Next");
    LCD_ShowString(205, 180, 30, 15, 12, "Level");
    LCD_ShowString(205, 250, 30, 15, 12, "Score");
    LCD_Fill(201, 0, 203, 320, GRAY);
    char buffer[10];
    int count_model = 0;
    while (1)
    {
        int lor = 0;
        /* USER CODE END WHILE */
        /* USER CODE BEGIN 3 */

        BACK_COLOR = WHITE;

        // draw the status label
        scoreNum = count_row*3 + count_model;

        // 1 to 2
        if (scoreNum > 20 && levelNum == 1) {
        	speed = 4;
        	levelNum = 2;
        }

        // 2 to 3
        if (scoreNum > 50 && levelNum == 2) {
        	speed = 10;
        	levelNum = 3;
        }

        // hell
        if (scoreNum > 100 && levelNum == 3) {
        	speed = 10;
        	levelNum = 3;
        }
        showStatus(levelNum, scoreNum, next_shape1, next_shape2);



        draw( initial, lor);

        // right
        if (GetPress(KEY0_GPIO_Port, KEY0_Pin, GPIO_PIN_RESET) == 1)
        {
            HAL_GPIO_TogglePin(LED0_GPIO_Port, LED0_Pin);
            //RIGHT
            lor = 1;
            draw( initial, lor);
        }
        // rotate
        if (GetPress(KEY1_GPIO_Port, KEY1_Pin, GPIO_PIN_RESET) == 1)
        {
            HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
            //left
            lor = 2;
            rotation += 1;
            draw( initial, lor);
        }

        // left
        if (GetPress(KEY_WK_GPIO_Port, KEY_WK_Pin, GPIO_PIN_SET) == 1)
        {
            HAL_GPIO_TogglePin(LED0_GPIO_Port, LED0_Pin);
            HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
            lor = -1;
            draw( initial, lor);
        }



        HAL_Delay(40);

        // down
        if (if_bottom == 0)
        {
        	lor = 0;
        }

        // reach the bottom
        else
        {
        	if(check_over() == 0){
                check_array();
                draw_again();
                //到底之后，出现下一块
                if_bottom = 0;
                drop = 0;
                rotation = 0;
                xoffset = 0;
                count_model += 1;

                // randomly generate next shape
                initial = next_shape1;
                next_shape1 = next_shape2;
                next_shape2 = rand()% 7;
        	}else{
        		POINT_COLOR = RED;
        		LCD_ShowString(100, 160, 50, 40, 24, "Game over!!!!!!!!");
        		break;
        	}
            //到底之后，先遍历二维数组



        }
        /* USER CODE END 3 */
    }
}
/**
  * @brief System Clock Configuration
  * @retval None
  */
int check_over(){
	for(int i = 0 ; i < 10 ; i++){
		if(arrays[0][i] != 0){
			return 1;
		}
	}
	return 0;
}
void draw_again(){
	for ( int i = 0 ;  i < 16 ; i++){
		for( int j = 0 ; j < 10 ; j++){
			int x1 = j * 20;
			int y1 = i *20;
			int x2 = j * 20  + 20;
			int y2 =  i *20 + 20;
			switch(arrays[i][j]){
				case 0 :
					LCD_Fill(x1 , y1 ,x2 ,y2 , WHITE);
					break;
				case 1 :
					LCD_Fill(x1 , y1 ,x2 ,y2 , YELLOW);
					break;

				case 2 :
					LCD_Fill(x1 , y1 ,x2 ,y2 , BLUE);
					break;

				case 3 :
					LCD_Fill(x1 , y1 ,x2 ,y2 , GREEN);
					break;

				case 4 :
					LCD_Fill(x1 , y1 ,x2 ,y2 , RED);
					break;

				case 5:
					LCD_Fill(x1 , y1 ,x2 ,y2 , CYAN);
					break;

				case 6 :
					LCD_Fill(x1 , y1 ,x2 ,y2 ,MAGENTA);
					break;

				case 7:
					LCD_Fill(x1 , y1 ,x2 ,y2 ,LIGHTBLUE);
					break;
			}
		}
	}
}

void draw(int shape, int  lor) {
	switch(shape) {
		case 0:
			draw_model_1(lor);
			break;
		case 1:
			draw_model_2(lor);
			break;
		case 2:
			draw_model_3(lor);
			break;
		case 3:
			draw_model_4(lor);
			break;
		case 4:
			draw_model_5(lor);
			break;
		case 5:
			draw_model_6(lor);
			break;
		case 6:
			draw_model_7(lor);
			break;
	}
}
int judge(int x_1, int y_1, int x_2, int y_2)
{
    if (x_2 > 200 || x_1 < 0 || y_1 < 0 || y_2 > 320)
    {
        return 0;
    }
    int x1 = x_1 / 20;
    int y1 = y_1 / 20;
    int x2 = x_2 / 20;
    int y2 = y_2 / 20;

    //如果刚好在在方格里
    if (y_2 % 20 == 0)
    {
        int i = 0;
        int j = 0;
        int f1 = 1;
        int f2 = 1;
        for (j = x1; j < x2; j++)
        {
            for (i = y1; i < y2; i++)
            {
                if (arrays[i][j] > 0)
                {
                    f1 = 0;
                }
            }
        }
        for (int i = x1; i < x2; i++)
        {
            if (arrays[i][y2] > 0)
            {
                f2 = 0;
            }
        }
        if (f1 == 1 && f2 == 1)
        {
            return 1;
        }
    }

    //如果不在在方格里
    else
    {
        int i = 0;
        int j = 0;

        y2 = y2 + 1;
        for (j = x1; j < x2; j++)
        {
            for (i = y1; i < y2; i++)
            {
                if (arrays[i][j] > 0)
                {
                    return 0;
                }
            }
        }
    }

    return 1;
}
void check_array(void)
{
    int i = 0;
    int j = 0;
    int temp_row = -1;

    for (i = 0; i < 16; i++)
    {
        int if_full = 1;
        for (j = 0; j < 10; j++)
        {

            if (arrays[i][j] == 0)
            {
                //  printf("%d\t",17);
                if_full = 0;
                break;
            }
        }
        if (if_full == 0)
        {
            //printf("%d\t",18);
            continue;
        }
        else
        {
        	count_row += 1;
            temp_row = i;
            printf("temp_row: %d\t", temp_row);

            int k = 0;
            int o = 0;
            for (k = 0; k < 16; k++)
            {
                for (o = 0; o < 10; o++)
                {
                    if (k <= temp_row)
                    {
                        if (k >= 1)
                        {
                            temp_arr[k][o] = arrays[k - 1][o];
                        }
                    }
                    else
                    {
                        temp_arr[k][o] = arrays[k][o];
                    }
                }
            }
            printf("test: %d\t", temp_arr[0][0]);
            k = 0;
            o = 0;
            for (k = 0; k < 16; k++)
            {
                for (o = 0; o < 10; o++)
                {
                    arrays[k][o] = temp_arr[k][o];
                }
            }
            //	 printArrays();
            // printf("\n");
            check_array();
        }
    }
    return 0;
}
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

    char levelMsg[5];
    char scoreMsg[5];
    //char shapeLable[5] = "Next";
    // char levelLable[6] = "Level";
    //  char scoreLable[6] = "Score";
    // draw the status bar

    // draw the strings in the status bar

    // draw the number of levels and scores
    sprintf(levelMsg, "%d", levelNum);
    sprintf(scoreMsg, "%d", scoreNum);
    LCD_ShowString(205, 210, 15, 15, 12, levelMsg);
    LCD_ShowString(205, 280, 15, 15, 12, scoreMsg);
    LCD_Fill(204,70,240,160,WHITE);
    switch (next_shape1)
    {
    /*
  ... ... ... ...
  ... ... ... ...

  */
    case 0:
        LCD_Fill(205, 75, 235, 85, YELLOW);
        break;

    /*
  ... ...
  ... ...
  ... ...
  ... ...

  */
    case 1:
        LCD_Fill(210, 70, 230, 90,BLUE );
        break;

    /*
  ... ...
  ... ...
      ... ...
      ... ...

  */
    case 2:
        LCD_Fill(205, 70, 225, 80, GREEN);
        LCD_Fill(215, 80, 235, 90, GREEN);
        break;

    /*
      ... ...
      ... ...
  ... ...
  ... ...

  */
    case 3:
        LCD_Fill(215, 70, 235, 80, RED);
        LCD_Fill(205, 80, 225, 90, RED);
        break;

    /*
  ...
  ...
  ... ... ...
  ... ... ...

  */
    case 4:
        LCD_Fill(205, 70, 215, 80, CYAN);
        LCD_Fill(205, 80, 235, 90, CYAN);
        break;

    /*
          ...
          ...
  ... ... ...
  ... ... ...

  */
    case 5:
        LCD_Fill(225, 70, 235, 80, MAGENTA);
        LCD_Fill(205, 80, 235, 90, MAGENTA);
        break;

    /*
      ...
      ...
  ... ... ...
  ... ... ...

  */
    case 6:
        LCD_Fill(205+10, 70, 215+10, 80, LIGHTBLUE);
        LCD_Fill(195+10, 80, 225+10, 90, LIGHTBLUE);
        break;
    }

    // draw the shapes of the coming nodes 2 center:(220, 120)
    int distance = 40;
    switch (next_shape2)
    {
    /*
  ... ... ... ...
  ... ... ... ...

  */
    case 0:
        LCD_Fill(205, 75 + distance, 235, 85 + distance, YELLOW);
        break;

    /*
  ... ...
  ... ...
  ... ...
  ... ...

  */
    case 1:
        LCD_Fill(210, 70 + distance, 230, 90 + distance,BLUE);
        break;

    /*
  ... ...
  ... ...
      ... ...
      ... ...

  */
    case 2:
        LCD_Fill(205, 70 + distance, 225, 80 + distance, GREEN);
        LCD_Fill(215, 80 + distance, 235, 90 + distance, GREEN);
        break;

    /*
      ... ...
      ... ...
  ... ...
  ... ...

  */
    case 3:
        LCD_Fill(215, 70 + distance, 235, 80 + distance, RED);
        LCD_Fill(205, 80 + distance, 225, 90 + distance, RED);
        break;

    /*
  ...
  ...
  ... ... ...
  ... ... ...

  */
    case 4:
        LCD_Fill(205, 70 + distance, 215, 80 + distance,CYAN);
        LCD_Fill(205, 80 + distance, 235, 90 + distance, CYAN);
        break;

    /*
          ...
          ...
  ... ... ...
  ... ... ...

  */
    case 5:
        LCD_Fill(225, 70 + distance, 235, 80 + distance, MAGENTA);
        LCD_Fill(205, 80 + distance, 235, 90 + distance, MAGENTA);
        break;

    /*
      ...
      ...
  ... ... ...
  ... ... ...

  */
    case 6:
        LCD_Fill(205+10, 70 + distance, 215+10, 80 + distance, LIGHTBLUE);
        LCD_Fill(195+10, 80 + distance, 225+10, 90 + distance, LIGHTBLUE);
        break;
    }
}

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

///// SEVEN MODES
void draw_model_1(int lor)
{

    int x1 = 0;
    int y1 = 0;
    int x2 = 0;
    int y2 = 0;

    // last state
    if (rotation % 2 == 0)
    {
        x1 = 60 + xoffset;
        y1 = 0 + drop;
        x2 = 140 + xoffset;
        y2 = 20 + drop;
    }
    else
    {
        x1 = 100 + xoffset;
        y1 = 0 + drop;
        x2 = 120 + xoffset;
        y2 = 80 + drop;
    }

    // erase the previous state



    // judge the next state

    // judge left
    if (lor == -1) {
		if(judge(x1-20, y1, x2-20, y2) == 1) {
			xoffset -= 20;
		}else{

		}
    }
    // judge right
     if (lor == 1) {
		if(judge(x1+20, y1, x2+20, y2) == 1) {
			xoffset += 20;
		}else{

		}
    }
    // judge drop
    if (lor == 0) {
    	// if next state is legal
    	if (judge(x1, y1+speed, x2, y2+speed) == 1) {
    		drop += speed;
    	// if next state is illegal
    	}else{

    		if_bottom = 1;
    		fill_record(x1, y1, x2, y2,1);
    	//	LCD_Fill(x1,y1,x2,y2,YELLOW);
    	//	return ;
    	}
    }
   // judge rotate
    if (lor == 2) {
    	//LCD_Fill(x1, y1-speed, x2, y2-speed, WHITE);

    	int last = rotation - 1;

    	if (last % 2 == 0)
    	{
    		LCD_Fill(60 + xoffset, 0 + drop, 140 + xoffset, 20 + drop, WHITE);
    	}

    	else
    	{
    		LCD_Fill(100 + xoffset, 0 + drop, 120 + xoffset, 80 + drop, WHITE);
    	}

    	if (judge(x1,y1,x2,y2) == 0) {
    		rotation -= 1;
    	}
    }

   // update the next state
	if (rotation % 2 == 0)
	  {
		  x1 = 60 + xoffset;
		  y1 = 0 + drop;
		  x2 = 140 + xoffset;
		  y2 = 20 + drop;
		 }
	 else
	 {
		  x1 = 100 + xoffset;
		  y1 = 0 + drop;
		  x2 = 120 + xoffset;
		  y2 = 80 + drop;
	 }


	// draw the block
	if (if_bottom == 0) {
		if ( lor == -1 && judge(x1, y1, x2, y2) == 1) {
			LCD_Fill(x1+20, y1, x2+20, y2, WHITE);
		} else
		if (lor == 1 && judge(x1, y1, x2, y2) == 1) {
			LCD_Fill(x1-20, y1, x2-20, y2, WHITE);
		}else if (lor == 0) {
			LCD_Fill(x1, y1-speed, x2, y2-speed, WHITE);
		}

		LCD_Fill(x1, y1, x2, y2, YELLOW);
	}
}
void draw_model_2(int lor)
{

    int x1 = 0;
    int y1 = 0;
    int x2 = 0;
    int y2 = 0;

    // last state
    if (rotation % 2 == 0)
    {
    	 x1 = 80 + xoffset;
		y1 = 0 + drop;
		x2 = 120 + xoffset;
		y2 = 40 + drop;
    }
    else
    {
        x1 = 80 + xoffset;
        y1 = 0 + drop;
        x2 = 120 + xoffset;
        y2 = 40 + drop;
    }

    // erase the previous state



    // judge the next state

    // judge left
    if (lor == -1) {
		if(judge(x1-20, y1, x2-20, y2) == 1) {
			xoffset -= 20;
		}else{

		}
    }
    // judge right
     if (lor == 1) {
		if(judge(x1+20, y1, x2+20, y2) == 1) {
			xoffset += 20;
		}else{

		}
    }
    // judge drop
    if (lor == 0) {
    	// if next state is legal
    	if (judge(x1, y1+speed, x2, y2+speed) == 1) {
    		drop += speed;
    	// if next state is illegal
    	}else{

    		if_bottom = 1;
    		fill_record(x1, y1, x2, y2,2);
    	//	LCD_Fill(x1,y1,x2,y2,YELLOW);
    	//	return ;
    	}
    }
   // judge rotate
    if (lor == 2) {
    	//LCD_Fill(x1, y1-speed, x2, y2-speed, WHITE);

    	int last = rotation - 1;

    	if (last % 2 == 0)
    	{
    		LCD_Fill(60 + xoffset, 20 + drop, 140 + xoffset, 40 + drop, WHITE);
    	}

    	else
    	{
    		LCD_Fill(100 + xoffset, 0 + drop, 120 + xoffset, 80 + drop, WHITE);
    	}

    	if (judge(x1,y1,x2,y2) == 0) {
    		rotation -= 1;
    	}
    }

   // update the next state
	if (rotation % 2 == 0)
	  {
		 x1 = 80 + xoffset;
		        y1 = 0 + drop;
		        x2 = 120 + xoffset;
		        y2 = 40 + drop;
		 }
	 else
	 {
		 x1 = 80 + xoffset;
		        y1 = 0 + drop;
		        x2 = 120 + xoffset;
		        y2 = 40 + drop;
	 }


	// draw the block
	if (if_bottom == 0) {
		if ( lor == -1 && judge(x1, y1, x2, y2) == 1) {
			LCD_Fill(x1+20, y1, x2+20, y2, WHITE);
		} else
		if (lor == 1 && judge(x1, y1, x2, y2) == 1) {
			LCD_Fill(x1-20, y1, x2-20, y2, WHITE);
		}else if (lor == 0) {
			LCD_Fill(x1, y1-speed, x2, y2-speed, WHITE);
		}

		LCD_Fill(x1, y1, x2, y2, BLUE);
	}
}
void draw_model_3(int lor)
{

    int x1 = 0;
    int y1 = 0;
    int x2 = 0;
    int y2 = 0;
    int x3 = 0;
    int y3 = 0;
    int x4 = 0;
    int y4 = 0;
    // last state
    if (rotation % 2 == 0)
    {
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
        x1 = 100 + xoffset;
        y1 = 20 + drop;
        x2 = 120 + xoffset;
        y2 = 60 + drop;
        x3 = 120 + xoffset;
		y3 = 0 + drop;
		x4 = 140 + xoffset;
		y4 = 40 + drop;
    }

    // erase the previous state



    // judge the next state

    // judge left
    if (lor == -1) {
		if(judge(x1-20, y1, x2-20, y2) == 1 && judge(x3-20, y3, x4-20, y4) == 1) {
			xoffset -= 20;
		}else{

		}
    }
    // judge right
     if (lor == 1) {
		if(judge(x1+20, y1, x2+20, y2) == 1 && judge(x3+20, y3, x4 +20, y4) == 1) {
			xoffset += 20;
		}else{

		}
    }
    // judge drop
    if (lor == 0) {
    	// if next state is legal
    	if (judge(x1, y1+speed, x2, y2+speed) == 1 && judge(x3, y3+speed, x4, y4+speed) == 1) {
    		drop += speed;
    	// if next state is illegal
    	}else{

    		if_bottom = 1;
    		fill_record(x1, y1, x2, y2,3);
    		fill_record(x3, y3, x4, y4,3);
    	//	LCD_Fill(x1,y1,x2,y2,YELLOW);
    	//	return ;
    	}
    }
   // judge rotate
    if (lor == 2) {
    	//LCD_Fill(x1, y1-speed, x2, y2-speed, WHITE);

    	int last = rotation - 1;

    	if (last % 2 == 0)
    	{
    		LCD_Fill(80 + xoffset, 0 + drop, 120 + xoffset, 20 + drop, WHITE);
    		LCD_Fill(100 + xoffset, 20 + drop, 140 + xoffset, 40 + drop, WHITE);
    	}

    	else
    	{
    		LCD_Fill(100 + xoffset, 20 + drop, 120 + xoffset, 60 + drop, WHITE);
    		LCD_Fill(120 + xoffset, 0 + drop, 140 + xoffset, 40 + drop, WHITE);
    	}

    	if (judge(x1,y1,x2,y2) == 0 || judge(x3,y3,x4,y4) == 0 ) {
    		rotation -= 1;
    	}
    }

   // update the next state
    if (rotation % 2 == 0)
    {
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
        x1 = 100 + xoffset;
        y1 = 20 + drop;
        x2 = 120 + xoffset;
        y2 = 60 + drop;
        x3 = 120 + xoffset;
		y3 = 0 + drop;
		x4 = 140 + xoffset;
		y4 = 40 + drop;
    }

	// draw the block
	if (if_bottom == 0) {
		if ( lor == -1 && judge(x1,y1,x2,y2) == 1 && judge(x3,y3,x4,y4) == 1 ) {
			LCD_Fill(x1+20, y1, x2+20, y2, WHITE);
			LCD_Fill(x3+20, y3, x4+20, y4, WHITE);
		} else
		if (lor == 1 && judge(x1,y1,x2,y2) == 1 && judge(x3,y3,x4,y4) == 1) {
			LCD_Fill(x1-20, y1, x2-20, y2, WHITE);
			LCD_Fill(x3-20, y3, x4-20, y4, WHITE);
		}else if (lor == 0) {
			LCD_Fill(x1, y1-speed, x2, y2-speed, WHITE);
			LCD_Fill(x3, y3-speed, x4, y4-speed, WHITE);
		}

		LCD_Fill(x1, y1, x2, y2, GREEN);
		LCD_Fill(x3, y3, x4, y4, GREEN);
	}
}


void draw_model_4(int lor)
{

    int x1 = 0;
    int y1 = 0;
    int x2 = 0;
    int y2 = 0;
    int x3 = 0;
    int y3 = 0;
    int x4 = 0;
    int y4 = 0;
    // last state
    if (rotation % 2 == 0)
    {
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
        x1 = 80 + xoffset;
        y1 = 0 + drop;
        x2 = 100 + xoffset;
        y2 = 40 + drop;
        x3 = 100 + xoffset;
		y3 = 20 + drop;
		x4 = 120 + xoffset;
		y4 = 60 + drop;
    }

    // erase the previous state



    // judge the next state

    // judge left
    if (lor == -1) {
		if(judge(x1-20, y1, x2-20, y2) == 1 && judge(x3-20, y3, x4-20, y4) == 1) {
			xoffset -= 20;
		}else{

		}
    }
    // judge right
     if (lor == 1) {
		if(judge(x1+20, y1, x2+20, y2) == 1 && judge(x3+20, y3, x4 +20, y4) == 1) {
			xoffset += 20;
		}else{

		}
    }
    // judge drop
    if (lor == 0) {
    	// if next state is legal
    	if (judge(x1, y1+speed, x2, y2+speed) == 1 && judge(x3, y3+speed, x4, y4+speed) == 1) {
    		drop += speed;
    	// if next state is illegal
    	}else{

    		if_bottom = 1;
    		fill_record(x1, y1, x2, y2,4);
    		fill_record(x3, y3, x4, y4,4);
    	//	LCD_Fill(x1,y1,x2,y2,YELLOW);
    	//	return ;
    	}
    }
   // judge rotate
    if (lor == 2) {
    	//LCD_Fill(x1, y1-speed, x2, y2-speed, WHITE);

    	int last = rotation - 1;

    	if (last % 2 == 0)
    	{
    		LCD_Fill(100 + xoffset, 0 + drop, 140 + xoffset, 20 + drop, WHITE);
    		LCD_Fill(80 + xoffset, 20 + drop, 120 + xoffset, 40 + drop, WHITE);
    	}

    	else
    	{
    		LCD_Fill(80 + xoffset, 0 + drop, 100 + xoffset, 40 + drop, WHITE);
    		LCD_Fill(100 + xoffset, 20 + drop, 120 + xoffset, 60 + drop, WHITE);
    	}

    	if (judge(x1,y1,x2,y2) == 0 || judge(x3,y3,x4,y4) == 0 ) {
    		rotation -= 1;
    	}
    }

   // update the next state
    if (rotation % 2 == 0)
    {
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
        x1 = 80 + xoffset;
        y1 = 0 + drop;
        x2 = 100 + xoffset;
        y2 = 40 + drop;
        x3 = 100 + xoffset;
		y3 = 20 + drop;
		x4 = 120 + xoffset;
		y4 = 60 + drop;
    }
	// draw the block
	if (if_bottom == 0) {
		if ( lor == -1 && judge(x1,y1,x2,y2) == 1 && judge(x3,y3,x4,y4) == 1 ) {
			LCD_Fill(x1+20, y1, x2+20, y2, WHITE);
			LCD_Fill(x3+20, y3, x4+20, y4, WHITE);
		} else
		if (lor == 1 && judge(x1,y1,x2,y2) == 1 && judge(x3,y3,x4,y4) == 1) {
			LCD_Fill(x1-20, y1, x2-20, y2, WHITE);
			LCD_Fill(x3-20, y3, x4-20, y4, WHITE);
		}else if (lor == 0) {
			LCD_Fill(x1, y1-speed, x2, y2-speed, WHITE);
			LCD_Fill(x3, y3-speed, x4, y4-speed, WHITE);
		}

		LCD_Fill(x1, y1, x2, y2, RED);
		LCD_Fill(x3, y3, x4, y4, RED);
	}
}
void draw_model_5(int lor)
{

    int x1 = 0;
    int y1 = 0;
    int x2 = 0;
    int y2 = 0;
    int x3 = 0;
    int y3 = 0;
    int x4 = 0;
    int y4 = 0;
    // last state
    if (rotation % 4 == 0)
    {
        x1 = 80 + xoffset;
        y1 = 0 + drop;
        x2 = 100 + xoffset;
        y2 = 20 + drop;
        x3 = 80 + xoffset;
		y3 = 20 + drop;
		x4 = 140 + xoffset;
		y4 = 40 + drop;
    }
    else if(rotation % 4 == 1)
    {
        x1 = 100 + xoffset;
        y1 = 0 + drop;
        x2 = 120 + xoffset;
        y2 = 20 + drop;
        x3 = 80+ xoffset;
		y3 = 0 + drop;
		x4 = 100 + xoffset;
		y4 = 60 + drop;
    }
    else if(rotation % 4 == 2)
    {
        x1 = 80 + xoffset;
        y1 = 0 + drop;
        x2 = 140 + xoffset;
        y2 = 20 + drop;
        x3 = 120 + xoffset;
		y3 = 20 + drop;
		x4 = 140 + xoffset;
		y4 = 40 + drop;
    }
    else
    {
        x1 = 80 + xoffset;
        y1 = 40 + drop;
        x2 = 100 + xoffset;
        y2 = 60 + drop;
        x3 = 100 + xoffset;
		y3 = 0 + drop;
		x4 = 120 + xoffset;
		y4 = 60 + drop;
    }

    // erase the previous state



    // judge the next state

    // judge left
    if (lor == -1) {
		if(judge(x1-20, y1, x2-20, y2) == 1 && judge(x3-20, y3, x4-20, y4) == 1) {
			xoffset -= 20;
		}else{

		}
    }
    // judge right
     if (lor == 1) {
		if(judge(x1+20, y1, x2+20, y2) == 1 && judge(x3+20, y3, x4 +20, y4) == 1) {
			xoffset += 20;
		}else{

		}
    }
    // judge drop
    if (lor == 0) {
    	// if next state is legal
    	if (judge(x1, y1+speed, x2, y2+speed) == 1 && judge(x3, y3+speed, x4, y4+speed) == 1) {
    		drop += speed;
    	// if next state is illegal
    	}else{

    		if_bottom = 1;
    		fill_record(x1, y1, x2, y2,5);
    		fill_record(x3, y3, x4, y4,5);
    	//	LCD_Fill(x1,y1,x2,y2,YELLOW);
    	//	return ;
    	}
    }
   // judge rotate
    if (lor == 2) {
    	//LCD_Fill(x1, y1-speed, x2, y2-speed, WHITE);

    	int last = rotation - 1;

    	if (last % 4 == 0)
    	{
    		LCD_Fill(80 + xoffset, 0 + drop, 100 + xoffset, 20 + drop, WHITE);
    		LCD_Fill(80 + xoffset, 20 + drop, 140 + xoffset, 40 + drop, WHITE);
    	}

    	else if(last% 4 == 1)
    	{
    		LCD_Fill(100 + xoffset, 0 + drop, 120 + xoffset, 20 + drop, WHITE);
    		LCD_Fill(80 + xoffset, 0 + drop, 100 + xoffset, 60 + drop, WHITE);
    	}
    	else if(last% 4 == 2){
    		LCD_Fill(80 + xoffset, 0 + drop, 140 + xoffset, 20 + drop, WHITE);
    		LCD_Fill(120 + xoffset, 20 + drop, 140 + xoffset, 40 + drop, WHITE);
    	}
    	else{
    		LCD_Fill(80 + xoffset, 40 + drop, 100 + xoffset, 60 + drop, WHITE);
    		LCD_Fill(100 + xoffset, 0 + drop, 120 + xoffset, 60 + drop, WHITE);
    	}

    	if (judge(x1,y1,x2,y2) == 0 || judge(x3,y3,x4,y4) == 0 ) {
    		rotation -= 1;
    	}
    }

   // update the next state
    if (rotation % 4 == 0)
    {
        x1 = 80 + xoffset;
        y1 = 0 + drop;
        x2 = 100 + xoffset;
        y2 = 20 + drop;
        x3 = 80 + xoffset;
		y3 = 20 + drop;
		x4 = 140 + xoffset;
		y4 = 40 + drop;
    }
    else if(rotation % 4 == 1)
    {
        x1 = 100 + xoffset;
        y1 = 0 + drop;
        x2 = 120 + xoffset;
        y2 = 20 + drop;
        x3 = 80+ xoffset;
		y3 = 0 + drop;
		x4 = 100 + xoffset;
		y4 = 60 + drop;
    }
    else if(rotation % 4 == 2)
    {
        x1 = 80 + xoffset;
        y1 = 0 + drop;
        x2 = 140 + xoffset;
        y2 = 20 + drop;
        x3 = 120 + xoffset;
		y3 = 20 + drop;
		x4 = 140 + xoffset;
		y4 = 40 + drop;
    }
    else
    {
        x1 = 80 + xoffset;
        y1 = 40 + drop;
        x2 = 100 + xoffset;
        y2 = 60 + drop;
        x3 = 100 + xoffset;
		y3 = 0 + drop;
		x4 = 120 + xoffset;
		y4 = 60 + drop;
    }

	// draw the block
	if (if_bottom == 0) {
		if ( lor == -1 && judge(x1,y1,x2,y2) == 1 && judge(x3,y3,x4,y4) == 1 ) {
			LCD_Fill(x1+20, y1, x2+20, y2, WHITE);
			LCD_Fill(x3+20, y3, x4+20, y4, WHITE);
		} else
		if (lor == 1 && judge(x1,y1,x2,y2) == 1 && judge(x3,y3,x4,y4) == 1) {
			LCD_Fill(x1-20, y1, x2-20, y2, WHITE);
			LCD_Fill(x3-20, y3, x4-20, y4, WHITE);
		}else if (lor == 0) {
			LCD_Fill(x1, y1-speed, x2, y2-speed, WHITE);
			LCD_Fill(x3, y3-speed, x4, y4-speed, WHITE);
		}

		LCD_Fill(x1, y1, x2, y2, CYAN);
		LCD_Fill(x3, y3, x4, y4, CYAN);
	}
}

void draw_model_6(int lor)
{

    int x1 = 0;
    int y1 = 0;
    int x2 = 0;
    int y2 = 0;
    int x3 = 0;
    int y3 = 0;
    int x4 = 0;
    int y4 = 0;
    // last state
    if (rotation % 4 == 0)
    {
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

        x1 = 80 + xoffset;
        y1 = 0 + drop;
        x2 = 100 + xoffset;
        y2 = 20 + drop;
        x3 = 100 + xoffset;
        y3 = 0 + drop;
        x4 = 120 + xoffset;
        y4 = 60 + drop;
    }

    // erase the previous state



    // judge the next state

    // judge left
    if (lor == -1) {
		if(judge(x1-20, y1, x2-20, y2) == 1 && judge(x3-20, y3, x4-20, y4) == 1) {
			xoffset -= 20;
		}else{

		}
    }
    // judge right
     if (lor == 1) {
		if(judge(x1+20, y1, x2+20, y2) == 1 && judge(x3+20, y3, x4 +20, y4) == 1) {
			xoffset += 20;
		}else{

		}
    }
    // judge drop
    if (lor == 0) {
    	// if next state is legal
    	if (judge(x1, y1+speed, x2, y2+speed) == 1 && judge(x3, y3+speed, x4, y4+speed) == 1) {
    		drop += speed;
    	// if next state is illegal
    	}else{

    		if_bottom = 1;
    		fill_record(x1, y1, x2, y2,6);
    		fill_record(x3, y3, x4, y4,6);
    	//	LCD_Fill(x1,y1,x2,y2,YELLOW);
    	//	return ;
    	}
    }
   // judge rotate
    if (lor == 2) {
    	//LCD_Fill(x1, y1-speed, x2, y2-speed, WHITE);

    	int last = rotation - 1;

    	if (last % 4 == 0)
    	{
    		LCD_Fill(120 + xoffset, 0 + drop, 140 + xoffset, 20 + drop, WHITE);
    		LCD_Fill(80 + xoffset, 20 + drop, 140 + xoffset, 40 + drop, WHITE);
    	}

    	else if(last% 4 == 1)
    	{
    		LCD_Fill(80 + xoffset, 0 + drop, 100 + xoffset, 60 + drop, WHITE);
    		LCD_Fill(100 + xoffset, 40 + drop, 120 + xoffset, 60 + drop, WHITE);
    	}
    	else if(last% 4 == 2){
    		LCD_Fill(80 + xoffset, 0 + drop, 140 + xoffset, 20 + drop, WHITE);
    		LCD_Fill(80 + xoffset, 20 + drop, 100 + xoffset, 40 + drop, WHITE);
    	}
    	else{
    		LCD_Fill(80 + xoffset, 0 + drop, 100 + xoffset, 20 + drop, WHITE);
    		LCD_Fill(100 + xoffset, 0 + drop, 120 + xoffset, 60 + drop, WHITE);
    	}

    	if (judge(x1,y1,x2,y2) == 0 || judge(x3,y3,x4,y4) == 0 ) {
    		rotation -= 1;
    	}
    }

    // last state
    if (rotation % 4 == 0)
    {
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

        x1 = 80 + xoffset;
        y1 = 0 + drop;
        x2 = 100 + xoffset;
        y2 = 20 + drop;
        x3 = 100 + xoffset;
        y3 = 0 + drop;
        x4 = 120 + xoffset;
        y4 = 60 + drop;
    }

	// draw the block
	if (if_bottom == 0) {
		if ( lor == -1 && judge(x1,y1,x2,y2) == 1 && judge(x3,y3,x4,y4) == 1 ) {
			LCD_Fill(x1+20, y1, x2+20, y2, WHITE);
			LCD_Fill(x3+20, y3, x4+20, y4, WHITE);
		} else
		if (lor == 1 && judge(x1,y1,x2,y2) == 1 && judge(x3,y3,x4,y4) == 1) {
			LCD_Fill(x1-20, y1, x2-20, y2, WHITE);
			LCD_Fill(x3-20, y3, x4-20, y4, WHITE);
		}else if (lor == 0) {
			LCD_Fill(x1, y1-speed, x2, y2-speed, WHITE);
			LCD_Fill(x3, y3-speed, x4, y4-speed, WHITE);
		}

		LCD_Fill(x1, y1, x2, y2, MAGENTA);
		LCD_Fill(x3, y3, x4, y4, MAGENTA);
	}
}

void draw_model_7(int lor)
{

    int x1 = 0;
    int y1 = 0;
    int x2 = 0;
    int y2 = 0;
    int x3 = 0;
    int y3 = 0;
    int x4 = 0;
    int y4 = 0;
    // last state
    if (rotation % 4 == 0)
   {
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
	   x1 = 80 + xoffset;
	   y1 = 20 + drop;
	   x2 = 100 + xoffset;
	   y2 = 40 + drop;
	   x3 = 100 + xoffset;
	   y3 = 0 + drop;
	   x4 = 120 + xoffset;
	   y4 = 60 + drop;
   }

    // erase the previous state



    // judge the next state

    // judge left
    if (lor == -1) {
		if(judge(x1-20, y1, x2-20, y2) == 1 && judge(x3-20, y3, x4-20, y4) == 1) {
			xoffset -= 20;
		}else{

		}
    }
    // judge right
     if (lor == 1) {
		if(judge(x1+20, y1, x2+20, y2) == 1 && judge(x3+20, y3, x4 +20, y4) == 1) {
			xoffset += 20;
		}else{

		}
    }
    // judge drop
    if (lor == 0) {
    	// if next state is legal
    	if (judge(x1, y1+speed, x2, y2+speed) == 1 && judge(x3, y3+speed, x4, y4+speed) == 1) {
    		drop += speed;
    	// if next state is illegal
    	}else{

    		if_bottom = 1;
    		fill_record(x1, y1, x2, y2,7);
    		fill_record(x3, y3, x4, y4,7);
    	//	LCD_Fill(x1,y1,x2,y2,YELLOW);
    	//	return ;
    	}
    }
   // judge rotate
    if (lor == 2) {
    	//LCD_Fill(x1, y1-speed, x2, y2-speed, WHITE);

    	int last = rotation - 1;

    	if (last % 4 == 0)
    	{
    		LCD_Fill(100 + xoffset, 0 + drop, 120 + xoffset, 20 + drop, WHITE);
    		LCD_Fill(80 + xoffset, 20 + drop, 140 + xoffset, 40 + drop, WHITE);
    	}

    	else if(last% 4 == 1)
    	{
    		LCD_Fill(80 + xoffset, 0 + drop, 100 + xoffset, 60 + drop, WHITE);
    		LCD_Fill(100 + xoffset, 20 + drop, 120 + xoffset, 40 + drop, WHITE);
    	}
    	else if(last% 4 == 2){
    		LCD_Fill(80 + xoffset, 0 + drop, 140 + xoffset, 20 + drop, WHITE);
    		LCD_Fill(100 + xoffset, 20 + drop, 120 + xoffset, 40 + drop, WHITE);
    	}
    	else{
    		LCD_Fill(80 + xoffset, 20 + drop, 100 + xoffset, 40 + drop, WHITE);
    		LCD_Fill(100 + xoffset, 0 + drop, 120 + xoffset, 60 + drop, WHITE);
    	}

    	if (judge(x1,y1,x2,y2) == 0 || judge(x3,y3,x4,y4) == 0 ) {
    		rotation -= 1;
    	}
    }

    // last state
    if (rotation % 4 == 0)
   {
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
	   x1 = 80 + xoffset;
	   y1 = 20 + drop;
	   x2 = 100 + xoffset;
	   y2 = 40 + drop;
	   x3 = 100 + xoffset;
	   y3 = 0 + drop;
	   x4 = 120 + xoffset;
	   y4 = 60 + drop;
   }

	// draw the block
	if (if_bottom == 0) {
		if ( lor == -1 && judge(x1,y1,x2,y2) == 1 && judge(x3,y3,x4,y4) == 1 ) {
			LCD_Fill(x1+20, y1, x2+20, y2, WHITE);
			LCD_Fill(x3+20, y3, x4+20, y4, WHITE);
		} else
		if (lor == 1 && judge(x1,y1,x2,y2) == 1 && judge(x3,y3,x4,y4) == 1) {
			LCD_Fill(x1-20, y1, x2-20, y2, WHITE);
			LCD_Fill(x3-20, y3, x4-20, y4, WHITE);
		}else if (lor == 0) {
			LCD_Fill(x1, y1-speed, x2, y2-speed, WHITE);
			LCD_Fill(x3, y3-speed, x4, y4-speed, WHITE);
		}

		LCD_Fill(x1, y1, x2, y2, LIGHTBLUE);
		LCD_Fill(x3, y3, x4, y4, LIGHTBLUE);
	}
}



void fill_record(int x_1, int y_1, int x_2, int y_2, int color)
{
    int x1 = x_1 / 20;
    int x2 = x_2 / 20;
    int y1 = y_1 / 20;
    int y2 = y_2 / 20;

    if (x2 == x1 + 1)
    {
        int i = 0;
        for (i = y1; i < y2; i++)
        {
            arrays[i][x1] = color;
        }
    }
    else if (y2 == y1 + 1)
    {
        int i = 0;
        for (i = x1; i < x2; i++)
        {
            arrays[y1][i] = color;
        }
    }
    else if (x2 >= x1 + 2 && y2 >= y1 + 2)
    {
        int i = 0;
        int j = 0;

        for (i = x1; i < x2; i++)
        {
            for (j = y1; j < y2; j++)
            {
                arrays[j][i] = color;
            }
        }
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
