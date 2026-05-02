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
#include <stdio.h>
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define ADV7513_ADDR  0x72

//Fixed Registers That Must Be Set


#define PLL_0_ADDR 0x98
#define PLL_0_VAL  0x03

#define PLL_1_ADDR 0x9A
#define PLL_1_VAL  0xE0

#define PLL_2_ADDR 0x9C
#define PLL_2_VAL  0x30

#define PLL_3_ADDR 0x9D
#define PLL_3_VAL  0x01

#define PLL_4_ADDR 0xA2
#define PLL_4_VAL  0xA4

#define PLL_5_ADDR 0xA3
#define PLL_5_VAL  0xA4

#define PLL_6_ADDR 0xE0
#define PLL_6_VAL  0xD0

#define PLL_7_ADDR 0xF9
#define PLL_7_VAL  0x00

// RGB888 -> HDMI INIT 

#define VIDEO_INPUT_ADDR   0x15
#define VIDEO_INPUT_VAL    0x00

#define OUTPUT_FORMAT_ADDR 0x16
#define OUTPUT_FORMAT_VAL  0x30

#define POLARITY_ADDR      0x17
#define POLARITY_VAL       0x00  

#define POWER_UP_ADDR      0x41	
#define POWER_UP_VAL       0x10

#define TMDS_OUTPUT_ADDR   0x40	
#define TMDS_OUTPUT_ENABLE 0x80

#define MODE_ADDR          0xAF	
#define MODE_DVI_ENABLE    0x00   
#define MODE_HDMI_ENABLE   0x02  

#define AVI_INFOFRAME_ADDR 0x55
#define AVI_INFOFRAME_VAL  0x10   // RGB 4:4:4

#define AVI_ASPECT_ADDR    0x56
#define AVI_ASPECT_VAL     0x20   // 16:9
#define AUTO_CKSUM_ADDR    0x4A
#define AUTO_CKSUM_VAL     0x80   // Auto Checksum Enable


// --- Определения для ADV7513 ---
#define ADV7513_MAIN_ADDR        0x72  // Основной I2C адрес ADV7513
#define ADV7513_EDID_MEM_ADDR    0x7E  // Адрес внутренней памяти EDID (по умолчанию)
#define EDID_BLOCK_SIZE          256   // Размер одного сегмента в байтах

// --- Регистры ADV7513 (Main Map) ---
#define REG_EDID_SEGMENT         0xC4  // Регистр выбора сегмента EDID
#define REG_EDID_RDY_FLAG        0x96  // Регистр флага готовности EDID
#define REG_EDID_MEM_ADDR        0x43  // Регистр адреса памяти EDID

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;

UART_HandleTypeDef huart1;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
static void MX_USART1_UART_Init(void);
void ADV7513_Full_Diagnostics(void);
void ADV7513_Read_Interrupts(void);
HAL_StatusTypeDef ADV7513_ReadEDIDSegment(I2C_HandleTypeDef *hi2c_dev, uint8_t segment, uint8_t *edid_buffer, uint32_t timeout_ms);
/* USER CODE BEGIN PFP */

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

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */
  HAL_Delay(200);
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_I2C1_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
  uint8_t edid_data[EDID_BLOCK_SIZE];
  ADV7513_PowerInit();
  ADV7513_HDMI_Init();

  // ADV7513_WriteVerifyReg(0xD6, 0x80, "HPD ON");
  // ADV7513_WriteVerifyReg(0x41, 0x00, "POWER TX ON");
  // uint8_t read_val = 0;
  // HAL_StatusTypeDef status = HAL_I2C_Mem_Read(&hi2c1, ADV7513_ADDR, 0x3E, I2C_MEMADD_SIZE_8BIT, &read_val, 1, HAL_MAX_DELAY);
  // // if(read_val== 0){
  // //   Error_Handler();
  // // }

  // status = HAL_I2C_Mem_Read(&hi2c1, ADV7513_ADDR, 0x17, I2C_MEMADD_SIZE_8BIT, &read_val, 1, HAL_MAX_DELAY);
  // if(read_val==0){
  //   Error_Handler();
  // }


  ADV7513_ReadEDIDSegment(&hi2c1,0, edid_data, HAL_MAX_DELAY);


  ADV7513_Full_Diagnostics();
  /* USER CODE END 2 */

  /* Infinite loop */
    while (1)
  {
     /* USER CODE BEGIN WHILE */
     
      // ADV7513_Read_Interrupts();
      __WFI();



    /* USER CODE END WHILE */

    


  }

  /* USER CODE BEGIN 3 */


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
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1|RCC_PERIPHCLK_I2C1;
  PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK1;
  PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_HSI;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
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
  hi2c1.Init.Timing = 0x00201D2B;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Analogue filter
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Digital filter
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

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
  huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
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
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4|GPIO_PIN_8, GPIO_PIN_RESET);

  /*Configure GPIO pins : PA4 PA8 */
  GPIO_InitStruct.Pin = GPIO_PIN_4|GPIO_PIN_8;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}


void ADV7513_Read_Interrupts(void)
{
    uint8_t interruptReg = 0;
    
    if(HAL_I2C_Mem_Read(&hi2c1, ADV7513_ADDR, 0x96, I2C_MEMADD_SIZE_8BIT, &interruptReg, 1, HAL_MAX_DELAY) != HAL_OK)
    {
        Error_Handler();
    }
    
    // Проверка бита VSYNC (бит 5)
    if((interruptReg >> 5) & 1)   // исправлено: добавлены скобки и &1
    {
        ADV7513_WriteVerifyReg(0x96, 0x00, "CLEAR_VSYNC_INT");
    }
}



/* USER CODE BEGIN 4 */
void ADV7513_Full_Diagnostics(void)
{
    uint8_t reg;
    char buf[80];

    HAL_UART_Transmit(&huart1, (uint8_t*)"\n=== ADV7513 FULL DIAGNOSTICS ===\n", 34, HAL_MAX_DELAY);

    // 1. Power status
    HAL_I2C_Mem_Read(&hi2c1, ADV7513_ADDR, 0x41, I2C_MEMADD_SIZE_8BIT, &reg, 1, HAL_MAX_DELAY);
    sprintf(buf, "0x41 Power     : 0x%02X  (Power-Up = %d)\n", reg, (reg & 0x10) ? 1 : 0);
    HAL_UART_Transmit(&huart1, (uint8_t*)buf, strlen(buf), HAL_MAX_DELAY);

    // 2. HPD и RxSense (самое важное)
    HAL_I2C_Mem_Read(&hi2c1, ADV7513_ADDR, 0x42, I2C_MEMADD_SIZE_8BIT, &reg, 1, HAL_MAX_DELAY);
    sprintf(buf, "0x42 Status    : 0x%02X  HPD=%d  RxSense=%d\n", 
            reg, (reg>>6)&1, (reg>>5)&1);
    HAL_UART_Transmit(&huart1, (uint8_t*)buf, strlen(buf), HAL_MAX_DELAY);

    // 3. PLL Lock + Input Status
    HAL_I2C_Mem_Read(&hi2c1, ADV7513_ADDR, 0x9E, I2C_MEMADD_SIZE_8BIT, &reg, 1, HAL_MAX_DELAY);
    sprintf(buf, "0x9E PLL+Input : 0x%02X  PLL Locked=%d\n", reg, (reg>>4)&1);
    HAL_UART_Transmit(&huart1, (uint8_t*)buf, strlen(buf), HAL_MAX_DELAY);

    // 4. TMDS Status
    HAL_I2C_Mem_Read(&hi2c1, ADV7513_ADDR, 0xA0, I2C_MEMADD_SIZE_8BIT, &reg, 1, HAL_MAX_DELAY);
    sprintf(buf, "0xA0 TMDS Status: 0x%02X\n", reg);
    HAL_UART_Transmit(&huart1, (uint8_t*)buf, strlen(buf), HAL_MAX_DELAY);

    // 5. TMDS Power
    HAL_I2C_Mem_Read(&hi2c1, ADV7513_ADDR, 0xA1, I2C_MEMADD_SIZE_8BIT, &reg, 1, HAL_MAX_DELAY);
    sprintf(buf, "0xA1 TMDS Power : 0x%02X\n", reg);
    HAL_UART_Transmit(&huart1, (uint8_t*)buf, strlen(buf), HAL_MAX_DELAY);

    // 6. DDC Controller State (очень важно!)
    HAL_I2C_Mem_Read(&hi2c1, ADV7513_ADDR, 0xC8, I2C_MEMADD_SIZE_8BIT, &reg, 1, HAL_MAX_DELAY);
    uint8_t ddc = reg & 0x0F;
    const char* states[] = {
        "In Reset (No HPD)",
        "Reading EDID",
        "IDLE (EDID Ready)",
        "Initializing HDCP",
        "HDCP Enabled",
        "Initializing HDCP Repeater"
    };
    sprintf(buf, "0xC8 DDC State : 0x%02X  (%s)\n", reg, (ddc <= 5) ? states[ddc] : "Unknown");
    HAL_UART_Transmit(&huart1, (uint8_t*)buf, strlen(buf), HAL_MAX_DELAY);

    // 7. Detected VIC (очень важно!)
    HAL_I2C_Mem_Read(&hi2c1, ADV7513_ADDR, 0x3E, I2C_MEMADD_SIZE_8BIT, &reg, 1, HAL_MAX_DELAY);
    sprintf(buf, "0x3E Detected VIC: 0x%02X  (VIC = %d)\n", reg, (reg >> 2) & 0x3F);
    HAL_UART_Transmit(&huart1, (uint8_t*)buf, strlen(buf), HAL_MAX_DELAY);

    HAL_UART_Transmit(&huart1, (uint8_t*)"================================\n\n", 33, HAL_MAX_DELAY);
}


void ADV7513_PowerInit(void)
{
    // 1. Power Up
    ADV7513_WriteVerifyReg(0x41, 0x10, "POWER_UP");
    HAL_Delay(200);

    // 2. Fixed PLL registers + High Frequency mode
    ADV7513_WriteVerifyReg(0x98, 0x03, "PLL0");
    ADV7513_WriteVerifyReg(0x9A, 0xE0, "PLL1");
    ADV7513_WriteVerifyReg(0x9C, 0x30, "PLL2");
    ADV7513_WriteVerifyReg(0x9D, 0x61, "PLL3_HighFreq");   // важно для 40 МГц
    ADV7513_WriteVerifyReg(0xA2, 0xA4, "PLL4");
    ADV7513_WriteVerifyReg(0xA3, 0xA4, "PLL5");
    ADV7513_WriteVerifyReg(0xE0, 0xD0, "PLL6");
    ADV7513_WriteVerifyReg(0xF9, 0x00, "PLL7");

    


    // ADV7513_WriteVerifyReg(0x15, 0x05, "RGB888-DDR-ON");
    // ADV7513_WriteVerifyReg(0xD0, 0x3C, "NO SYNC PULSE");
    uint8_t reg = 0;


    HAL_I2C_Mem_Read(&hi2c1, ADV7513_ADDR, 0x9E, I2C_MEMADD_SIZE_8BIT, &reg, 1, HAL_MAX_DELAY);
    if(!((reg>>4)&1)){
      Error_Handler();
    }
    HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_8);
}


void ADV7513_HDMI_Init(void)
{

    
    // // // ========== 3. Рекомендованные ADI регистры ==========
    ADV7513_WriteVerifyReg(0x99, 0x02, "ADI_RECOMMENDED");
    ADV7513_WriteVerifyReg(0xA5, 0x44, "ADI_RECOMMENDED");
    ADV7513_WriteVerifyReg(0xAB, 0x40, "ADI_RECOMMENDED");
    ADV7513_WriteVerifyReg(0xD1, 0xFF, "ADI_RECOMMENDED");
    ADV7513_WriteVerifyReg(0xFA, 0x7D, "ADI_RECOMMENDED");
    ADV7513_WriteVerifyReg(0x49, 0xA8, "must be set to default");
    ADV7513_WriteVerifyReg(0x4C, 0x00, "must be set to default");
    ADV7513_WriteVerifyReg(0x4B, 0x80, "Clear_AVMute");   // обязательно
    ADV7513_WriteVerifyReg(0xDE, 0x10, "TMDS_CLK_DRIVER_ON"); //default
    
    // // // ========== 4. Настройка видеоформата ==========
    ADV7513_WriteVerifyReg(0x15, 0x00, "RGB888_4:4:4");
    ADV7513_WriteVerifyReg(0x16, 0x60, "Output_Format_444_8bit");
    ADV7513_WriteVerifyReg(0x18, 0x46, "CSC_Disabled");    // исправлено
    // //точно верно
    // // // ========== 5. Полярности (зависит от вашего LTDC) ==========
    ADV7513_WriteVerifyReg(0x17, 0x60, "HS VS AL 4:3"); 
    // // // ========== 6. Включение TMDS выхода ==========
    ADV7513_WriteVerifyReg(0x40, 0x80, "TMDS_ENABLE");  // GC packet enabled   
    // // // ========== 7. Режим HDMI и AVI InfoFrame ==========

      ADV7513_WriteVerifyReg(0xAF, 0x02, "HDMI ON");
      ADV7513_WriteVerifyReg(0x44, 0x10," AVI PACKET ENABLE");
      ADV7513_WriteVerifyReg(0x55, 0x00, "AVI");


    // // ========== 8. Задержка такта (опционально) ==========
    // ADV7513_WriteVerifyReg(0xBA, 0x60, "CLOCK_DELAY_0ns");
    // ========== 9. Управление тактовым драйвером TMDS ==========
    // Регистр 0xDE, бит 3 = 0 для включения clock driver


    

}


HAL_StatusTypeDef ADV7513_ReadEDIDSegment(I2C_HandleTypeDef *hi2c_dev, uint8_t segment, uint8_t *edid_buffer, uint32_t timeout_ms)
{
    uint8_t reg_data = 0;
    uint32_t tickstart = HAL_GetTick();

    // --- 1. Сброс флага готовности EDID ---
    // Читаем регистр 0x96, чтобы убедиться, что флаг готовности сброшен.
    if (HAL_I2C_Mem_Read(hi2c_dev, ADV7513_MAIN_ADDR, REG_EDID_RDY_FLAG, I2C_MEMADD_SIZE_8BIT, &reg_data, 1, HAL_MAX_DELAY) != HAL_OK) {
        return HAL_ERROR;
    }

    // --- 2. Запрос нового сегмента EDID ---
    // Записываем номер нужного сегмента в регистр 0xC4.
    // Это действие инициирует процесс чтения EDID мастером ADV7513.
    reg_data = segment;
    if (HAL_I2C_Mem_Write(hi2c_dev, ADV7513_MAIN_ADDR, REG_EDID_SEGMENT, I2C_MEMADD_SIZE_8BIT, &reg_data, 1, HAL_MAX_DELAY) != HAL_OK) {
        return HAL_ERROR;
    }

    // --- 3. Ожидание готовности данных (опрос флага EDID_RDY)---
    // Циклически проверяем бит 2 (EDID_RDY) в регистре 0x96.
    while ((HAL_GetTick() - tickstart) < timeout_ms) {
        if (HAL_I2C_Mem_Read(hi2c_dev, ADV7513_MAIN_ADDR, REG_EDID_RDY_FLAG, I2C_MEMADD_SIZE_8BIT, &reg_data, 1, HAL_MAX_DELAY) != HAL_OK) {
            return HAL_ERROR;
        }
        // Если флаг установлен, данные готовы
        if (reg_data & (1 << 2)) {
            break;
        }
    }

    // Проверка, не истекло ли время ожидания
    if ((HAL_GetTick() - tickstart) >= timeout_ms) {
        Error_Handler();
    }

    // --- 4. Чтение данных из EDID-памяти ADV7513 ---
    // Данные успешно загружены чипом. Теперь читаем их по адресу 0x7E.
    if (HAL_I2C_Mem_Read(hi2c_dev, ADV7513_EDID_MEM_ADDR, 0x00, I2C_MEMADD_SIZE_8BIT, edid_buffer, EDID_BLOCK_SIZE, HAL_MAX_DELAY) != HAL_OK) {
        Error_Handler();
    }


    return HAL_OK;
}



void ADV7513_WriteVerifyReg(uint8_t reg_addr, uint8_t reg_value, const char* reg_name)
{
    HAL_StatusTypeDef status;
    uint8_t write_val = reg_value;
    uint8_t read_val = 0;

    // Запись
    status = HAL_I2C_Mem_Write(&hi2c1, ADV7513_ADDR, reg_addr, I2C_MEMADD_SIZE_8BIT, &write_val, 1, HAL_MAX_DELAY);
    if(status != HAL_OK) {
        HAL_UART_Transmit(&huart1, (uint8_t*)reg_name, strlen(reg_name), HAL_MAX_DELAY);
        HAL_UART_Transmit(&huart1, (uint8_t*)" Write ERROR\n", 13, HAL_MAX_DELAY);
        Error_Handler();
    }

    // Чтение
    status = HAL_I2C_Mem_Read(&hi2c1, ADV7513_ADDR, reg_addr, I2C_MEMADD_SIZE_8BIT, &read_val, 1, HAL_MAX_DELAY);
    if(status != HAL_OK) {
        HAL_UART_Transmit(&huart1, (uint8_t*)reg_name, strlen(reg_name), HAL_MAX_DELAY);
        HAL_UART_Transmit(&huart1, (uint8_t*)" Read ERROR\n", 12, HAL_MAX_DELAY);
        Error_Handler();
    }

    // Проверка
    if(read_val != reg_value) {
        HAL_UART_Transmit(&huart1, (uint8_t*)reg_name, strlen(reg_name), HAL_MAX_DELAY);
        HAL_UART_Transmit(&huart1, (uint8_t*)" Verify FAIL\n", 13, HAL_MAX_DELAY);
        Error_Handler();
    } else {
        HAL_UART_Transmit(&huart1, (uint8_t*)reg_name, strlen(reg_name), HAL_MAX_DELAY);
        HAL_UART_Transmit(&huart1, (uint8_t*)" => OK\n", 7, HAL_MAX_DELAY);
    }
}



/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET);
  while (1)
  {
    // HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_8);
    // HAL_Delay(250);
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
