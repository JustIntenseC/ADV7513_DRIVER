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
  // uint8_t read_val = 0;
  // HAL_StatusTypeDef status = HAL_I2C_Mem_Read(&hi2c1, ADV7513_ADDR, 0x41, I2C_MEMADD_SIZE_8BIT, &read_val, 1, HAL_MAX_DELAY);
  ADV7513_PowerInit();
  ADV7513_HDMI_Init();
  // Проверка PLL lock (самый важный индикатор)
    uint8_t pll_status = 0;
    HAL_I2C_Mem_Read(&hi2c1, ADV7513_ADDR, 0x9E, I2C_MEMADD_SIZE_8BIT, &pll_status, 1, HAL_MAX_DELAY);
    if (pll_status & 0x10) {
        HAL_UART_Transmit(&huart1, (uint8_t*)"PLL LOCKED (40 MHz TMDS)!\n", 27, HAL_MAX_DELAY);
    } else {
        HAL_UART_Transmit(&huart1, (uint8_t*)"PLL NOT LOCKED!\n", 16, HAL_MAX_DELAY);
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

/* USER CODE BEGIN 4 */


// void ADV7513_PowerInit(void){

//   typedef struct{
//     uint8_t addr;
//     uint8_t val;
//     const char *name;
//   } reg_adv7513_t;

//   reg_adv7513_t regs[] = {
//     // {0xD6, 0b00000000, "HDP_HIGH"},
//     // {0x41, 0b00010000, "POWER UP"},

    
//     {PLL_0_ADDR, PLL_0_VAL, "PLL0"},
//     {PLL_1_ADDR, PLL_1_VAL, "PLL_1"},
//     {PLL_2_ADDR, PLL_2_VAL, "PLL_2"},
//     {PLL_3_ADDR, PLL_3_VAL, "PLL_3"},
//     {PLL_4_ADDR, PLL_4_VAL, "PLL_4"},
//     {PLL_5_ADDR, PLL_5_VAL, "PLL_5"},
//     {PLL_6_ADDR, PLL_6_VAL, "PLL_6"},
//     {PLL_7_ADDR, PLL_7_VAL, "PLL_7"},
//     {0x3B, 0x54, "PR_MANUAL_X4"},
//   };

//   for(int i = 0; i<(sizeof(regs)/sizeof(regs[0]));i++){
//     ADV7513_WriteVerifyReg(regs[i].addr, regs[i].val, regs[i].name);
//   }

//   HAL_UART_Transmit(&huart1, (uint8_t*)"All PLL Registers OK\n", 22, HAL_MAX_DELAY);
//   HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_8);
//   HAL_Delay(100);
//   HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_8);
//   HAL_Delay(100);
//   HAL_GPIO_TogglePin(GPIOA,GPIO_PIN_8);

// }
void ADV7513_PowerInit(void)
{
    typedef struct {
        uint8_t addr;
        uint8_t val;
        const char *name;
    } reg_adv7513_t;

    // 1. СНАЧАЛА ОБЯЗАТЕЛЬНО POWER-UP
    reg_adv7513_t power_regs[] = {
        {POWER_UP_ADDR, POWER_UP_VAL, "POWER_UP"}
    };
    for (int i = 0; i < sizeof(power_regs)/sizeof(power_regs[0]); i++) {
        ADV7513_WriteVerifyReg(power_regs[i].addr, power_regs[i].val, power_regs[i].name);
    }

    // 2. Теперь PLL + PR
    reg_adv7513_t regs[] = {
        {PLL_0_ADDR, PLL_0_VAL, "PLL0"},
        {PLL_1_ADDR, PLL_1_VAL, "PLL_1"},
        {PLL_2_ADDR, PLL_2_VAL, "PLL_2"},
        {PLL_3_ADDR, PLL_3_VAL, "PLL_3"},
        {PLL_4_ADDR, PLL_4_VAL, "PLL_4"},
        {PLL_5_ADDR, PLL_5_VAL, "PLL_5"},
        {PLL_6_ADDR, PLL_6_VAL, "PLL_6"},
        {PLL_7_ADDR, PLL_7_VAL, "PLL_7"},
        {0x3B, 0x54, "PR_MANUAL_X4"}          // ← правильно для ×4
    };

    for (int i = 0; i < sizeof(regs)/sizeof(regs[0]); i++) {
        ADV7513_WriteVerifyReg(regs[i].addr, regs[i].val, regs[i].name);
    }

    HAL_UART_Transmit(&huart1, (uint8_t*)"Power + PLL + PR x4 OK\n", 24, HAL_MAX_DELAY);

    // LED blink
    HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_8); HAL_Delay(100);
    HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_8); HAL_Delay(100);
    HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_8);
}

// void ADV7513_HDMI_Init(void)
// {
//     typedef struct {
//         uint8_t addr;
//         uint8_t val;
//         const char* name;
//     } reg_t;

//     reg_t regs[] = {
//         {0x3C, 0x00, "Video Identification Code"},
//         {VIDEO_INPUT_ADDR,   VIDEO_INPUT_VAL,   "VIDEO_INPUT"}, //rgb 24 bit
//         {OUTPUT_FORMAT_ADDR, OUTPUT_FORMAT_VAL, "OUTPUT_FORMAT"},
//         {POLARITY_ADDR,      POLARITY_VAL,      "POLARITY"},
//         {POWER_UP_ADDR,      POWER_UP_VAL,      "POWER_UP"},
//         {MODE_ADDR,          MODE_HDMI_ENABLE,  "MODE"},          // HDMI mode
//         {TMDS_OUTPUT_ADDR,   TMDS_OUTPUT_ENABLE,"TMDS_OUTPUT"},   // включает TMDS
//         {AVI_INFOFRAME_ADDR, AVI_INFOFRAME_VAL, "AVI_INFOFRAME"},
//         {AVI_ASPECT_ADDR, AVI_ASPECT_VAL, "AVI_ASPECT"},
//         {AUTO_CKSUM_ADDR, AUTO_CKSUM_VAL, "AUTO_CKSUM"},
//         {0x18, 0x00, "CSC_DISABLE"},
//         {0xA1, 0x00, "TMDS_POWER_ON"},
//     };

//     for(int i = 0; i < sizeof(regs)/sizeof(regs[0]); i++) {
//         ADV7513_WriteVerifyReg(regs[i].addr, regs[i].val, regs[i].name);
//     }

//     HAL_UART_Transmit(&huart1, (uint8_t*)"HDMI Init Complete\n", 20, HAL_MAX_DELAY);
//     HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_8);
//     HAL_Delay(100);
//     HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_8);
//     HAL_Delay(100);
//     HAL_GPIO_TogglePin(GPIOA,GPIO_PIN_8);
//     HAL_Delay(100);
//     HAL_GPIO_TogglePin(GPIOA,GPIO_PIN_8);
// }

void ADV7513_HDMI_Init(void)
{
    typedef struct {
        uint8_t addr;
        uint8_t val;
        const char* name;
    } reg_t;

    reg_t regs[] = {
        {0x15, 0x00, "VIDEO_INPUT"},          // RGB 4:4:4 24-bit
        {0x16, 0x30, "OUTPUT_FORMAT"},
        {0x17, 0x00, "POLARITY"},
        // {0x18, 0x00, "CSC_DISABLE"},          // ОБЯЗАТЕЛЬНО
        {0x3C, 0x00, "VIC (custom)"},
        {0xAF, 0x02, "HDMI_MODE"},
        {0x40, 0x80, "GC_PACKET_EN"},
        {0x55, 0x10, "AVI_INFOFRAME"},
        {0x56, 0x20, "AVI_ASPECT"},           // можно оставить 0x20 (или 0x28 — оба работают)
        {0x4A, 0x80, "AUTO_CKSUM"},
        {0xA1, 0x00, "TMDS_POWER_ON"}         // TMDS clock + data ON
    };

    for (int i = 0; i < sizeof(regs)/sizeof(regs[0]); i++) {
        ADV7513_WriteVerifyReg(regs[i].addr, regs[i].val, regs[i].name);
    }

    HAL_UART_Transmit(&huart1, (uint8_t*)"HDMI Init + TMDS ON Complete\n", 30, HAL_MAX_DELAY);

    // LED blink
    HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_8); HAL_Delay(100);
    HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_8); HAL_Delay(100);
    HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_8); HAL_Delay(100);
    HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_8);
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
// void ADV7513_PowerInit(void)
// {
//     HAL_StatusTypeDef status = HAL_OK;
//     uint8_t write_val = 0;
//     uint8_t read_val = 0;
    
//     // ========== 1. PLL_0 (0x98) = 0x03 ==========
//     write_val = PLL_0_VAL;  // 0x03
//     status = HAL_I2C_Mem_Write(&hi2c1, ADV7513_ADDR, PLL_0_ADDR, 
//                                 I2C_MEMADD_SIZE_8BIT, &write_val, 1, HAL_MAX_DELAY);
//     if(status != HAL_OK) { HAL_UART_Transmit(&huart1, (uint8_t*)"PLL_0 Write ERROR\n", 19, HAL_MAX_DELAY); Error_Handler(); }
    
//     status = HAL_I2C_Mem_Read(&hi2c1, ADV7513_ADDR, PLL_0_ADDR, 
//                                I2C_MEMADD_SIZE_8BIT, &read_val, 1, HAL_MAX_DELAY);
//     if(status != HAL_OK) { HAL_UART_Transmit(&huart1, (uint8_t*)"PLL_0 Read ERROR\n", 18, HAL_MAX_DELAY); Error_Handler(); }
    
//     if(read_val != PLL_0_VAL) { HAL_UART_Transmit(&huart1, (uint8_t*)"PLL_0 Verify FAIL\n", 19, HAL_MAX_DELAY); Error_Handler(); }
//     else { HAL_UART_Transmit(&huart1, (uint8_t*)"PLL_0 => OK\n", 13, HAL_MAX_DELAY); }
    
//     // ========== 2. PLL_1 (0x9A) = 0xE0 ==========
//     write_val = PLL_1_VAL;  // 0xE0
//     status = HAL_I2C_Mem_Write(&hi2c1, ADV7513_ADDR, PLL_1_ADDR, 
//                                 I2C_MEMADD_SIZE_8BIT, &write_val, 1, HAL_MAX_DELAY);
//     if(status != HAL_OK) { HAL_UART_Transmit(&huart1, (uint8_t*)"PLL_1 Write ERROR\n", 19, HAL_MAX_DELAY); Error_Handler(); }
    
//     status = HAL_I2C_Mem_Read(&hi2c1, ADV7513_ADDR, PLL_1_ADDR, 
//                                I2C_MEMADD_SIZE_8BIT, &read_val, 1, HAL_MAX_DELAY);
//     if(status != HAL_OK) { HAL_UART_Transmit(&huart1, (uint8_t*)"PLL_1 Read ERROR\n", 18, HAL_MAX_DELAY); Error_Handler(); }
    
//     if(read_val != PLL_1_VAL) { HAL_UART_Transmit(&huart1, (uint8_t*)"PLL_1 Verify FAIL\n", 19, HAL_MAX_DELAY); Error_Handler(); }
//     else { HAL_UART_Transmit(&huart1, (uint8_t*)"PLL_1 => OK\n", 13, HAL_MAX_DELAY); }
    
//     // ========== 3. PLL_2 (0x9C) = 0x30 ==========
//     write_val = PLL_2_VAL;  // 0x30
//     status = HAL_I2C_Mem_Write(&hi2c1, ADV7513_ADDR, PLL_2_ADDR, 
//                                 I2C_MEMADD_SIZE_8BIT, &write_val, 1, HAL_MAX_DELAY);
//     if(status != HAL_OK) { HAL_UART_Transmit(&huart1, (uint8_t*)"PLL_2 Write ERROR\n", 19, HAL_MAX_DELAY); Error_Handler(); }
    
//     status = HAL_I2C_Mem_Read(&hi2c1, ADV7513_ADDR, PLL_2_ADDR, 
//                                I2C_MEMADD_SIZE_8BIT, &read_val, 1, HAL_MAX_DELAY);
//     if(status != HAL_OK) { HAL_UART_Transmit(&huart1, (uint8_t*)"PLL_2 Read ERROR\n", 18, HAL_MAX_DELAY); Error_Handler(); }
    
//     if(read_val != PLL_2_VAL) { HAL_UART_Transmit(&huart1, (uint8_t*)"PLL_2 Verify FAIL\n", 19, HAL_MAX_DELAY); Error_Handler(); }
//     else { HAL_UART_Transmit(&huart1, (uint8_t*)"PLL_2 => OK\n", 13, HAL_MAX_DELAY); }
    
//     // ========== 4. PLL_3 (0x9D) = 0x01 ==========
//     write_val = PLL_3_VAL;  // 0x01
//     status = HAL_I2C_Mem_Write(&hi2c1, ADV7513_ADDR, PLL_3_ADDR, 
//                                 I2C_MEMADD_SIZE_8BIT, &write_val, 1, HAL_MAX_DELAY);
//     if(status != HAL_OK) { HAL_UART_Transmit(&huart1, (uint8_t*)"PLL_3 Write ERROR\n", 19, HAL_MAX_DELAY); Error_Handler(); }
    
//     status = HAL_I2C_Mem_Read(&hi2c1, ADV7513_ADDR, PLL_3_ADDR, 
//                                I2C_MEMADD_SIZE_8BIT, &read_val, 1, HAL_MAX_DELAY);
//     if(status != HAL_OK) { HAL_UART_Transmit(&huart1, (uint8_t*)"PLL_3 Read ERROR\n", 18, HAL_MAX_DELAY); Error_Handler(); }
    
//     if(read_val != PLL_3_VAL) { HAL_UART_Transmit(&huart1, (uint8_t*)"PLL_3 Verify FAIL\n", 19, HAL_MAX_DELAY); Error_Handler(); }
//     else { HAL_UART_Transmit(&huart1, (uint8_t*)"PLL_3 => OK\n", 13, HAL_MAX_DELAY); }
    
//     // ========== 5. PLL_4 (0xA2) = 0xA4 ==========
//     write_val = PLL_4_VAL;  // 0xA4
//     status = HAL_I2C_Mem_Write(&hi2c1, ADV7513_ADDR, PLL_4_ADDR, 
//                                 I2C_MEMADD_SIZE_8BIT, &write_val, 1, HAL_MAX_DELAY);
//     if(status != HAL_OK) { HAL_UART_Transmit(&huart1, (uint8_t*)"PLL_4 Write ERROR\n", 19, HAL_MAX_DELAY); Error_Handler(); }
    
//     status = HAL_I2C_Mem_Read(&hi2c1, ADV7513_ADDR, PLL_4_ADDR, 
//                                I2C_MEMADD_SIZE_8BIT, &read_val, 1, HAL_MAX_DELAY);
//     if(status != HAL_OK) { HAL_UART_Transmit(&huart1, (uint8_t*)"PLL_4 Read ERROR\n", 18, HAL_MAX_DELAY); Error_Handler(); }
    
//     if(read_val != PLL_4_VAL) { HAL_UART_Transmit(&huart1, (uint8_t*)"PLL_4 Verify FAIL\n", 19, HAL_MAX_DELAY); Error_Handler(); }
//     else { HAL_UART_Transmit(&huart1, (uint8_t*)"PLL_4 => OK\n", 13, HAL_MAX_DELAY); }
    
//     // ========== 6. PLL_5 (0xA3) = 0xA4 ==========
//     write_val = PLL_5_VAL;  // 0xA4
//     status = HAL_I2C_Mem_Write(&hi2c1, ADV7513_ADDR, PLL_5_ADDR, 
//                                 I2C_MEMADD_SIZE_8BIT, &write_val, 1, HAL_MAX_DELAY);
//     if(status != HAL_OK) { HAL_UART_Transmit(&huart1, (uint8_t*)"PLL_5 Write ERROR\n", 19, HAL_MAX_DELAY); Error_Handler(); }
    
//     status = HAL_I2C_Mem_Read(&hi2c1, ADV7513_ADDR, PLL_5_ADDR, 
//                                I2C_MEMADD_SIZE_8BIT, &read_val, 1, HAL_MAX_DELAY);
//     if(status != HAL_OK) { HAL_UART_Transmit(&huart1, (uint8_t*)"PLL_5 Read ERROR\n", 18, HAL_MAX_DELAY); Error_Handler(); }
    
//     if(read_val != PLL_5_VAL) { HAL_UART_Transmit(&huart1, (uint8_t*)"PLL_5 Verify FAIL\n", 19, HAL_MAX_DELAY); Error_Handler(); }
//     else { HAL_UART_Transmit(&huart1, (uint8_t*)"PLL_5 => OK\n", 13, HAL_MAX_DELAY); }
    
//     // ========== 7. PLL_6 (0xE0) = 0xD0 ==========
//     write_val = PLL_6_VAL;  // 0xD0
//     status = HAL_I2C_Mem_Write(&hi2c1, ADV7513_ADDR, PLL_6_ADDR, 
//                                 I2C_MEMADD_SIZE_8BIT, &write_val, 1, HAL_MAX_DELAY);
//     if(status != HAL_OK) { HAL_UART_Transmit(&huart1, (uint8_t*)"PLL_6 Write ERROR\n", 19, HAL_MAX_DELAY); Error_Handler(); }
    
//     status = HAL_I2C_Mem_Read(&hi2c1, ADV7513_ADDR, PLL_6_ADDR, 
//                                I2C_MEMADD_SIZE_8BIT, &read_val, 1, HAL_MAX_DELAY);
//     if(status != HAL_OK) { HAL_UART_Transmit(&huart1, (uint8_t*)"PLL_6 Read ERROR\n", 18, HAL_MAX_DELAY); Error_Handler(); }
    
//     if(read_val != PLL_6_VAL) { HAL_UART_Transmit(&huart1, (uint8_t*)"PLL_6 Verify FAIL\n", 19, HAL_MAX_DELAY); Error_Handler(); }
//     else { HAL_UART_Transmit(&huart1, (uint8_t*)"PLL_6 => OK\n", 13, HAL_MAX_DELAY); }
    
//     // ========== 8. PLL_7 (0xF9) = 0x00 ==========
//     // write_val = PLL_7_VAL;  // 0x00
//     // status = HAL_I2C_Mem_Write(&hi2c1, ADV7513_ADDR, PLL_7_ADDR, 
//     //                             I2C_MEMADD_SIZE_8BIT, &write_val, 1, HAL_MAX_DELAY);
//     // if(status != HAL_OK) { HAL_UART_Transmit(&huart1, (uint8_t*)"PLL_7 Write ERROR\n", 19, HAL_MAX_DELAY); Error_Handler(); }
    
//     // status = HAL_I2C_Mem_Read(&hi2c1, ADV7513_ADDR, PLL_7_ADDR, 
//     //                            I2C_MEMADD_SIZE_8BIT, &read_val, 1, HAL_MAX_DELAY);
//     // if(status != HAL_OK) { HAL_UART_Transmit(&huart1, (uint8_t*)"PLL_7 Read ERROR\n", 18, HAL_MAX_DELAY); Error_Handler(); }
    
//     // if(read_val != PLL_7_VAL) { HAL_UART_Transmit(&huart1, (uint8_t*)"PLL_7 Verify FAIL\n", 19, HAL_MAX_DELAY); Error_Handler(); }
//     // else { HAL_UART_Transmit(&huart1, (uint8_t*)"PLL_7 => OK\n", 13, HAL_MAX_DELAY); }
    
//     HAL_UART_Transmit(&huart1, (uint8_t*)"All PLL Registers OK\n", 22, HAL_MAX_DELAY);
// }

// void ADV7513_HDMI_Init(void)
// {
//     HAL_StatusTypeDef status = HAL_OK;
//     uint8_t write_val = 0;
//     uint8_t read_val = 0;

//     // ========== 1. VIDEO_INPUT (0x15) = 0x00 (RGB 4:4:4) ==========
//     write_val = VIDEO_INPUT_VAL;  // 0x00
//     status = HAL_I2C_Mem_Write(&hi2c1, ADV7513_ADDR, VIDEO_INPUT_ADDR,
//                                 I2C_MEMADD_SIZE_8BIT, &write_val, 1, HAL_MAX_DELAY);
//     if(status != HAL_OK) { HAL_UART_Transmit(&huart1, (uint8_t*)"VIDEO_INPUT Write ERROR\n", 25, HAL_MAX_DELAY); Error_Handler(); }

//     status = HAL_I2C_Mem_Read(&hi2c1, ADV7513_ADDR, VIDEO_INPUT_ADDR,
//                                I2C_MEMADD_SIZE_8BIT, &read_val, 1, HAL_MAX_DELAY);
//     if(status != HAL_OK) { HAL_UART_Transmit(&huart1, (uint8_t*)"VIDEO_INPUT Read ERROR\n", 24, HAL_MAX_DELAY); Error_Handler(); }

//     if(read_val != VIDEO_INPUT_VAL) { HAL_UART_Transmit(&huart1, (uint8_t*)"VIDEO_INPUT Verify FAIL\n", 25, HAL_MAX_DELAY); Error_Handler(); }
//     else { HAL_UART_Transmit(&huart1, (uint8_t*)"VIDEO_INPUT => OK\n", 19, HAL_MAX_DELAY); }

//     // ========== 2. OUTPUT_FORMAT (0x16) = 0x30 (RGB888) ==========
//     write_val = OUTPUT_FORMAT_VAL;  // 0x30
//     status = HAL_I2C_Mem_Write(&hi2c1, ADV7513_ADDR, OUTPUT_FORMAT_ADDR,
//                                 I2C_MEMADD_SIZE_8BIT, &write_val, 1, HAL_MAX_DELAY);
//     if(status != HAL_OK) { HAL_UART_Transmit(&huart1, (uint8_t*)"OUTPUT_FORMAT Write ERROR\n", 27, HAL_MAX_DELAY); Error_Handler(); }

//     status = HAL_I2C_Mem_Read(&hi2c1, ADV7513_ADDR, OUTPUT_FORMAT_ADDR,
//                                I2C_MEMADD_SIZE_8BIT, &read_val, 1, HAL_MAX_DELAY);
//     if(status != HAL_OK) { HAL_UART_Transmit(&huart1, (uint8_t*)"OUTPUT_FORMAT Read ERROR\n", 26, HAL_MAX_DELAY); Error_Handler(); }

//     if(read_val != OUTPUT_FORMAT_VAL) { HAL_UART_Transmit(&huart1, (uint8_t*)"OUTPUT_FORMAT Verify FAIL\n", 27, HAL_MAX_DELAY); Error_Handler(); }
//     else { HAL_UART_Transmit(&huart1, (uint8_t*)"OUTPUT_FORMAT => OK\n", 21, HAL_MAX_DELAY); }

//     // ========== 3. POLARITY (0x17) = 0x00 ==========
//     write_val = POLARITY_VAL;  // 0x00
//     status = HAL_I2C_Mem_Write(&hi2c1, ADV7513_ADDR, POLARITY_ADDR,
//                                 I2C_MEMADD_SIZE_8BIT, &write_val, 1, HAL_MAX_DELAY);
//     if(status != HAL_OK) { HAL_UART_Transmit(&huart1, (uint8_t*)"POLARITY Write ERROR\n", 22, HAL_MAX_DELAY); Error_Handler(); }

//     status = HAL_I2C_Mem_Read(&hi2c1, ADV7513_ADDR, POLARITY_ADDR,
//                                I2C_MEMADD_SIZE_8BIT, &read_val, 1, HAL_MAX_DELAY);
//     if(status != HAL_OK) { HAL_UART_Transmit(&huart1, (uint8_t*)"POLARITY Read ERROR\n", 21, HAL_MAX_DELAY); Error_Handler(); }

//     if(read_val != POLARITY_VAL) { HAL_UART_Transmit(&huart1, (uint8_t*)"POLARITY Verify FAIL\n", 22, HAL_MAX_DELAY); Error_Handler(); }
//     else { HAL_UART_Transmit(&huart1, (uint8_t*)"POLARITY => OK\n", 16, HAL_MAX_DELAY); }

//     // ========== 4. POWER_UP (0x41) = 0x10 ==========
//     write_val = POWER_UP_VAL;  // 0x10
//     status = HAL_I2C_Mem_Write(&hi2c1, ADV7513_ADDR, POWER_UP_ADDR,
//                                 I2C_MEMADD_SIZE_8BIT, &write_val, 1, HAL_MAX_DELAY);
//     if(status != HAL_OK) { HAL_UART_Transmit(&huart1, (uint8_t*)"POWER_UP Write ERROR\n", 22, HAL_MAX_DELAY); Error_Handler(); }

//     status = HAL_I2C_Mem_Read(&hi2c1, ADV7513_ADDR, POWER_UP_ADDR,
//                                I2C_MEMADD_SIZE_8BIT, &read_val, 1, HAL_MAX_DELAY);
//     if(status != HAL_OK) { HAL_UART_Transmit(&huart1, (uint8_t*)"POWER_UP Read ERROR\n", 21, HAL_MAX_DELAY); Error_Handler(); }

//     if(read_val != POWER_UP_VAL) { HAL_UART_Transmit(&huart1, (uint8_t*)"POWER_UP Verify FAIL\n", 22, HAL_MAX_DELAY); Error_Handler(); }
//     else { HAL_UART_Transmit(&huart1, (uint8_t*)"POWER_UP => OK\n", 16, HAL_MAX_DELAY); }

//     // ========== 5. MODE (0xAF) = HDMI enable (0x02) ==========
//     // Если нужно DVI, замените на MODE_DVI_ENABLE (0x00)
//     write_val = MODE_HDMI_ENABLE;  // 0x02
//     status = HAL_I2C_Mem_Write(&hi2c1, ADV7513_ADDR, MODE_ADDR,
//                                 I2C_MEMADD_SIZE_8BIT, &write_val, 1, HAL_MAX_DELAY);
//     if(status != HAL_OK) { HAL_UART_Transmit(&huart1, (uint8_t*)"MODE Write ERROR\n", 18, HAL_MAX_DELAY); Error_Handler(); }

//     status = HAL_I2C_Mem_Read(&hi2c1, ADV7513_ADDR, MODE_ADDR,
//                                I2C_MEMADD_SIZE_8BIT, &read_val, 1, HAL_MAX_DELAY);
//     if(status != HAL_OK) { HAL_UART_Transmit(&huart1, (uint8_t*)"MODE Read ERROR\n", 17, HAL_MAX_DELAY); Error_Handler(); }

//     if(read_val != MODE_HDMI_ENABLE) { HAL_UART_Transmit(&huart1, (uint8_t*)"MODE Verify FAIL\n", 18, HAL_MAX_DELAY); Error_Handler(); }
//     else { HAL_UART_Transmit(&huart1, (uint8_t*)"MODE => OK\n", 12, HAL_MAX_DELAY); }

//     // ========== 6. TMDS OUTPUT + GC PACKET (0x40) = 0x80 ==========
//     // Бит 7 включает TMDS выход и передачу General Control Packets
//     write_val = TMDS_OUTPUT_ENABLE;  // 0x80
//     status = HAL_I2C_Mem_Write(&hi2c1, ADV7513_ADDR, TMDS_OUTPUT_ADDR,
//                                 I2C_MEMADD_SIZE_8BIT, &write_val, 1, HAL_MAX_DELAY);
//     if(status != HAL_OK) { HAL_UART_Transmit(&huart1, (uint8_t*)"TMDS_OUTPUT Write ERROR\n", 25, HAL_MAX_DELAY); Error_Handler(); }

//     status = HAL_I2C_Mem_Read(&hi2c1, ADV7513_ADDR, TMDS_OUTPUT_ADDR,
//                                I2C_MEMADD_SIZE_8BIT, &read_val, 1, HAL_MAX_DELAY);
//     if(status != HAL_OK) { HAL_UART_Transmit(&huart1, (uint8_t*)"TMDS_OUTPUT Read ERROR\n", 24, HAL_MAX_DELAY); Error_Handler(); }

//     if(read_val != TMDS_OUTPUT_ENABLE) { HAL_UART_Transmit(&huart1, (uint8_t*)"TMDS_OUTPUT Verify FAIL\n", 25, HAL_MAX_DELAY); Error_Handler(); }
//     else { HAL_UART_Transmit(&huart1, (uint8_t*)"TMDS_OUTPUT => OK\n", 19, HAL_MAX_DELAY); }

//     // ========== 7. AVI InfoFrame (0x55) = 0x10 (RGB 4:4:4) ==========
//     // Это первый байт AVI InfoFrame (Version). В зависимости от разрешения могут потребоваться дополнительные байты.
//     write_val = AVI_INFOFRAME_VAL;  // 0x10
//     status = HAL_I2C_Mem_Write(&hi2c1, ADV7513_ADDR, AVI_INFOFRAME_ADDR,
//                                 I2C_MEMADD_SIZE_8BIT, &write_val, 1, HAL_MAX_DELAY);
//     if(status != HAL_OK) { HAL_UART_Transmit(&huart1, (uint8_t*)"AVI_INFOFRAME Write ERROR\n", 27, HAL_MAX_DELAY); Error_Handler(); }

//     status = HAL_I2C_Mem_Read(&hi2c1, ADV7513_ADDR, AVI_INFOFRAME_ADDR,
//                                I2C_MEMADD_SIZE_8BIT, &read_val, 1, HAL_MAX_DELAY);
//     if(status != HAL_OK) { HAL_UART_Transmit(&huart1, (uint8_t*)"AVI_INFOFRAME Read ERROR\n", 26, HAL_MAX_DELAY); Error_Handler(); }

//     if(read_val != AVI_INFOFRAME_VAL) { HAL_UART_Transmit(&huart1, (uint8_t*)"AVI_INFOFRAME Verify FAIL\n", 27, HAL_MAX_DELAY); Error_Handler(); }
//     else { HAL_UART_Transmit(&huart1, (uint8_t*)"AVI_INFOFRAME => OK\n", 21, HAL_MAX_DELAY); }

//     HAL_UART_Transmit(&huart1, (uint8_t*)"HDMI Init Complete\n", 20, HAL_MAX_DELAY);
// }
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
