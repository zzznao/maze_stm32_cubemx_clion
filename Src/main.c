/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
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
#include "tim.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "User/maze.h"
#include "User/motor.h"
#include "User/infrared.h"

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

/* USER CODE BEGIN PV */
motorVar left_var;
motorVar right_var;
positionVar pos_var;
uint8_t maze[DPI][DPI] = {0x01};//走过是1，障碍是1

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void para_init(char x, char y){
  left_var.ENC = 0;
  left_var.PWM = 0;
  left_var.ADD = 0;
  left_var.TGT = 0;

  right_var.ENC = 0;
  right_var.PWM = 0;
  right_var.ADD = 0;
  right_var.TGT = 0;

  pos_var.x = x;
  pos_var.x = y;
  pos_var.aim_x = x;
  pos_var.aim_y = y;
  pos_var.relDir = 0;

  maze[x][y] = 0x10;
}

void check(){
  check_edge(pos_var.x, pos_var.y, pos_var.relDir, (char **) maze);
  if(maze[pos_var.x+1][pos_var.y] == 0x00){
    pos_var.aim_x = pos_var.x+1;
    pos_var.aim_y = pos_var.y;
    maze[pos_var.aim_x][pos_var.aim_y] |= 0x10;
  }else if(maze[pos_var.x+1][pos_var.y] == 0x10){

  }
  if(maze[pos_var.x][pos_var.y-1] == 0x00){
    pos_var.aim_x = pos_var.x;
    pos_var.aim_y = pos_var.y-1;
    maze[pos_var.aim_x][pos_var.aim_y] |= 0x10;
  }else if(maze[pos_var.x][pos_var.y-1] == 0x10){

  }
  if(maze[pos_var.x-1][pos_var.y] == 0x00){
    pos_var.aim_x = pos_var.x-1;
    pos_var.aim_y = pos_var.y;
    maze[pos_var.aim_x][pos_var.aim_y] |= 0x10;
  }else if(maze[pos_var.x-1][pos_var.y] == 0x10){

  }
  if(maze[pos_var.x][pos_var.y+1] == 0x00){
    pos_var.aim_x = pos_var.x;
    pos_var.aim_y = pos_var.y+1;
    maze[pos_var.aim_x][pos_var.aim_y] |= 0x10;
  }else if(maze[pos_var.x][pos_var.y+1] == 0x10){

  }
}

void goToNext(){
  char absDir = checkAbsDir(&pos_var);
  pos_var.relDir = checkNextDir(pos_var.relDir, absDir);
  if(pos_var.relDir == 1){
    go_straight(&left_var, &right_var, 999);
  }else if(pos_var.relDir == 2) {
    go_left(&left_var, &right_var, 999);
    go_straight(&left_var, &right_var, 999);
  }else if(pos_var.relDir == 3){
    go_turn(&left_var, &right_var, 999);
    go_straight(&left_var, &right_var, 999);
  }else if(pos_var.relDir == 4){
    go_right(&left_var, &right_var, 999);
    go_straight(&left_var, &right_var, 999);
  }else{
    while(1);
  }
  pos_var.x = pos_var.aim_x;
  pos_var.y = pos_var.aim_y;
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
  char flag = 1;

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */
  para_init(1,1);

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_TIM3_Init();
  MX_TIM4_Init();
  MX_TIM6_Init();
  MX_TIM8_Init();
  /* USER CODE BEGIN 2 */

  HAL_TIM_Encoder_Start(&htim3, TIM_CHANNEL_1|TIM_CHANNEL_2);
  HAL_TIM_Encoder_Start(&htim4, TIM_CHANNEL_1|TIM_CHANNEL_2);
  HAL_TIM_PWM_Start(&htim8,TIM_CHANNEL_1);
  HAL_TIM_PWM_Start(&htim8,TIM_CHANNEL_2);
  HAL_TIM_PWM_Start(&htim8,TIM_CHANNEL_3);
  HAL_TIM_PWM_Start(&htim8,TIM_CHANNEL_4);
  HAL_TIM_Base_Start_IT(&htim6);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
    if(flag){
      check();
      goToNext();
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
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
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
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
  if (htim->Instance == htim6.Instance){ //10ms
    //motorControl
    check_ENC(&left_var, &right_var);
    plus_ADD(&left_var, &right_var);
    incremental_PI_A(&left_var);
    incremental_PI_B(&right_var);
    range_PWM(&left_var, &right_var, 6000);
    set_PWM(&left_var, &right_var);
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
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
