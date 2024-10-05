#include "stm32f4xx_hal.h"
#include <stdio.h>

// Define handles for ADC and UART
ADC_HandleTypeDef hadc1;
UART_HandleTypeDef huart2;

void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_ADC1_Init(void);
static void MX_USART2_UART_Init(void);

int main(void) {
    // Initialize the HAL library
    HAL_Init();

    // Configure the system clock
    SystemClock_Config();

    // Initialize all configured peripherals
    MX_GPIO_Init();
    MX_ADC1_Init();
    MX_USART2_UART_Init();

    // Start the ADC in polling mode
    HAL_ADC_Start(&hadc1);

    while (1) {
        // Wait for the ADC conversion to complete
        if (HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY) == HAL_OK) {
            // Read the ADC converted value
            uint32_t mic_value = HAL_ADC_GetValue(&hadc1);

            // Prepare buffer and print the mic value to UART
            char buffer[50];
            int len = snprintf(buffer, sizeof(buffer), "Mic Value: %lu\r\n", mic_value);
            HAL_UART_Transmit(&huart2, (uint8_t*)buffer, len, HAL_MAX_DELAY);

            HAL_Delay(10);  // Optional delay for readability
        }
    }
}

// ADC1 Initialization Function
static void MX_ADC1_Init(void) {
    ADC_ChannelConfTypeDef sConfig = {0};

    // Configure the ADC peripheral
    hadc1.Instance = ADC1;
    hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
    hadc1.Init.Resolution = ADC_RESOLUTION_12B;
    hadc1.Init.ScanConvMode = DISABLE;
    hadc1.Init.ContinuousConvMode = DISABLE;
    hadc1.Init.DiscontinuousConvMode = DISABLE;
    hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
    hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
    hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
    hadc1.Init.NbrOfConversion = 1;
    hadc1.Init.DMAContinuousRequests = DISABLE;
    hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
    if (HAL_ADC_Init(&hadc1) != HAL_OK) {
        // Initialization Error
        Error_Handler();
    }

    // Configure the ADC channel
    sConfig.Channel = ADC_CHANNEL_0;  // Adjust according to your analog pin
    sConfig.Rank = 1;
    sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
    if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK) {
        // Channel Configuration Error
        Error_Handler();
    }
}

// USART2 Initialization Function
static void MX_USART2_UART_Init(void) {
    // Configure the UART peripheral
    huart2.Instance = USART2;
    huart2.Init.BaudRate = 9600;
    huart2.Init.WordLength = UART_WORDLENGTH_8B;
    huart2.Init.StopBits = UART_STOPBITS_1;
    huart2.Init.Parity = UART_PARITY_NONE;
    huart2.Init.Mode = UART_MODE_TX_RX;
    huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart2.Init.OverSampling = UART_OVERSAMPLING_16;
    if (HAL_UART_Init(&huart2) != HAL_OK) {
        // Initialization Error
        Error_Handler();
    }
}

// GPIO Initialization Function
static void MX_GPIO_Init(void) {
    __HAL_RCC_GPIOA_CLK_ENABLE();  // Enable GPIOA clock for PA0 if needed
}

// System Clock Configuration
void SystemClock_Config(void) {
    // Configure the system clock here based on your system's needs
}

// Error Handler Function
void Error_Handler(void) {
    while (1) {
        // Stay in a loop if there's an error
    }
}
