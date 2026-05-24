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
I2C_HandleTypeDef hi2c1;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */
#define TxBufferSize	8
#define RxBufferSize	4

int count = 0;

uint8_t aTxBuffer[TxBufferSize] = {0};
uint8_t aRxBuffer[RxBufferSize];
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
static void MX_USART2_UART_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void MostrarNumero(uint8_t valor){

	uint8_t display = 0;

	switch(valor) {
	    case 0: display = 0b1000000; break;
	    case 1: display = 0b1111001; break;
	    case 2: display = 0b0100100; break;
	    case 3: display = 0b0110000; break;
	    case 4: display = 0b0011001; break;
	    case 5: display = 0b0010010; break;
	    case 6: display = 0b0000010; break;
	    case 7: display = 0b1111000; break;
	    case 8: display = 0b0000000; break;
	    default: display = 0b1111111; break; // En ánodo común, 1 apaga todos los segmentos
	}
	//Revisar y encender bits
	HAL_GPIO_WritePin(DPLY_A_GPIO_Port,DPLY_A_Pin,(display & 0x01)?GPIO_PIN_SET:GPIO_PIN_RESET);
	HAL_GPIO_WritePin(DPLY_B_GPIO_Port,DPLY_B_Pin,(display & 0x02)?GPIO_PIN_SET:GPIO_PIN_RESET);
	HAL_GPIO_WritePin(DPLY_C_GPIO_Port,DPLY_C_Pin,(display & 0x04)?GPIO_PIN_SET:GPIO_PIN_RESET);
	HAL_GPIO_WritePin(DPLY_D_GPIO_Port,DPLY_D_Pin,(display & 0x08)?GPIO_PIN_SET:GPIO_PIN_RESET);
	HAL_GPIO_WritePin(DPLY_E_GPIO_Port,DPLY_E_Pin,(display & 0x10)?GPIO_PIN_SET:GPIO_PIN_RESET);
	HAL_GPIO_WritePin(DPLY_F_GPIO_Port,DPLY_F_Pin,(display & 0x20)?GPIO_PIN_SET:GPIO_PIN_RESET);
	HAL_GPIO_WritePin(DPLY_G_GPIO_Port,DPLY_G_Pin,(display & 0x40)?GPIO_PIN_SET:GPIO_PIN_RESET);
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
  MX_I2C1_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */
  aTxBuffer[0] = 1;
  aTxBuffer[4] = 1;
  aTxBuffer[5] = 1;
  aTxBuffer[6] = 1;

  MostrarNumero(9);
  if (HAL_I2C_EnableListen_IT(&hi2c1) != HAL_OK) {
	  Error_Handler();
  }

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
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

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
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
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 36;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

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
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, LD2_Pin|DPLY_A_Pin|DPLY_F_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, DPLY_B_Pin|DPLY_E_Pin|DPLY_C_Pin|DPLY_D_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(DPLY_G_GPIO_Port, DPLY_G_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : LD2_Pin DPLY_A_Pin DPLY_F_Pin */
  GPIO_InitStruct.Pin = LD2_Pin|DPLY_A_Pin|DPLY_F_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : DPLY_B_Pin DPLY_E_Pin DPLY_C_Pin DPLY_D_Pin */
  GPIO_InitStruct.Pin = DPLY_B_Pin|DPLY_E_Pin|DPLY_C_Pin|DPLY_D_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : DPLY_G_Pin */
  GPIO_InitStruct.Pin = DPLY_G_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(DPLY_G_GPIO_Port, &GPIO_InitStruct);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
//Cuando termine alguna lectura, volver a colocar listo la escucha a algun llamado
void HAL_I2C_ListenCpltCallback(I2C_HandleTypeDef *hi2c){
	HAL_I2C_EnableListen_IT(hi2c);
}

void HAL_I2C_AddrCallback(I2C_HandleTypeDef *hi2c, uint8_t TransferDirection, uint16_t AddrMatchCode){
	// para trasmitir datos del maestro al esclavo
	if (TransferDirection == I2C_DIRECTION_TRANSMIT) {
		if (HAL_I2C_Slave_Seq_Receive_IT(&hi2c1, (uint8_t*) aRxBuffer, 1, I2C_FIRST_AND_LAST_FRAME) != HAL_OK) {
			/* Transfer error in reception process */
			Error_Handler();
		}
	}
	else if (TransferDirection == I2C_DIRECTION_RECEIVE) {
		//para trasmitir datos del esclavo al maestro
		if (HAL_I2C_Slave_Seq_Transmit_IT(&hi2c1, (uint8_t*) aTxBuffer, 8, I2C_FIRST_AND_LAST_FRAME) != HAL_OK) {
			Error_Handler();
		}

	}
}

void HAL_I2C_SlaveRxCpltCallback(I2C_HandleTypeDef *I2cHandle) {
	MostrarNumero(aRxBuffer[0]);

    aRxBuffer[0] = 0x00;
    HAL_I2C_EnableListen_IT(I2cHandle);
}

void HAL_I2C_SlaveTxCpltCallback(I2C_HandleTypeDef *I2cHandle){
	//se podria colocar logica para saber si se transmitireon lso datos
}

void HAL_I2C_ErrorCallback(I2C_HandleTypeDef *I2cHandle) {
    /* Error_Handler() function is called when error occurs.
     * 1- When Slave doesn't acknowledge its address, Master restarts communication.
     * 2- When Master doesn't acknowledge the last data transferred, Slave doesn't care in this example.
     */
    if (HAL_I2C_GetError(I2cHandle) != HAL_I2C_ERROR_AF) {
        Error_Handler();
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
