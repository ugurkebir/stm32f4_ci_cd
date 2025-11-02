/**
  ******************************************************************************
  * @file    stm32f4xx_hal_mocks.c
  * @author  Test Framework
  * @brief   Mock implementations for STM32F4xx HAL functions
  ******************************************************************************
  * This file provides mock implementations of HAL functions for unit testing
  ******************************************************************************
  */

#include "stm32f4xx_hal_mocks.h"
#include <stdio.h>
#include <string.h>

/* Mock GPIO structures */
GPIO_TypeDef mock_gpioa = {0};
GPIO_TypeDef mock_gpiob = {0};
GPIO_TypeDef mock_gpioc = {0};
GPIO_TypeDef mock_gpiod = {0};

/* Mock peripheral instances */
void* UART4 = (void*)0x40004C00UL;
void* TIM6 = (void*)0x40001000UL;

/* System core clock */
uint32_t SystemCoreClock = 168000000UL;

/* Mock control structure */
MockControl_t mock_control = {0};

/**
  * @brief  Reset all mock states
  * @retval None
  */
void mock_reset(void)
{
    memset(&mock_control, 0, sizeof(MockControl_t));
    memset(&mock_gpioa, 0, sizeof(GPIO_TypeDef));
    memset(&mock_gpiob, 0, sizeof(GPIO_TypeDef));
    memset(&mock_gpioc, 0, sizeof(GPIO_TypeDef));
    memset(&mock_gpiod, 0, sizeof(GPIO_TypeDef));
    
    /* Set default return values to HAL_OK */
    mock_control.hal_init_return_value = HAL_OK;
    mock_control.rcc_osc_config_return_value = HAL_OK;
    mock_control.rcc_clk_config_return_value = HAL_OK;
    mock_control.uart_init_return_value = HAL_OK;
    mock_control.tim_init_return_value = HAL_OK;
}

/**
  * @brief  Set HAL_Init return value
  * @param  status: Return status to set
  * @retval None
  */
void mock_set_hal_init_return(HAL_StatusTypeDef status)
{
    mock_control.hal_init_return_value = status;
}

/**
  * @brief  Set RCC config return value
  * @param  status: Return status to set
  * @retval None
  */
void mock_set_rcc_config_return(HAL_StatusTypeDef status)
{
    mock_control.rcc_osc_config_return_value = status;
    mock_control.rcc_clk_config_return_value = status;
}

/**
  * @brief  Set UART init return value
  * @param  status: Return status to set
  * @retval None
  */
void mock_set_uart_init_return(HAL_StatusTypeDef status)
{
    mock_control.uart_init_return_value = status;
}

/**
  * @brief  Set Timer init return value
  * @param  status: Return status to set
  * @retval None
  */
void mock_set_tim_init_return(HAL_StatusTypeDef status)
{
    mock_control.tim_init_return_value = status;
}

/**
  * @brief  Get delay call count
  * @retval Number of times HAL_Delay was called
  */
uint32_t mock_get_delay_call_count(void)
{
    return mock_control.delay_ms_called_count;
}

/**
  * @brief  Get last delay value
  * @retval Last delay value passed to HAL_Delay
  */
uint32_t mock_get_last_delay_value(void)
{
    return mock_control.last_delay_value;
}

/**
  * @brief  Get GPIO toggle count
  * @retval Number of times HAL_GPIO_TogglePin was called
  */
uint32_t mock_get_gpio_toggle_count(void)
{
    return mock_control.gpio_toggle_count;
}

/**
  * @brief  Get last GPIO port used
  * @retval Pointer to last GPIO port
  */
GPIO_TypeDef* mock_get_last_gpio_port(void)
{
    return mock_control.last_gpio_port;
}

/**
  * @brief  Get last GPIO pin used
  * @retval Last GPIO pin number
  */
uint16_t mock_get_last_gpio_pin(void)
{
    return mock_control.last_gpio_pin;
}

/* HAL Function Mocks */

/**
  * @brief  Mock HAL initialization
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_Init(void)
{
    mock_control.hal_init_called = true;
    return mock_control.hal_init_return_value;
}

/**
  * @brief  Mock RCC oscillator configuration
  * @param  RCC_OscInitStruct: Oscillator configuration structure
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_RCC_OscConfig(RCC_OscInitTypeDef* RCC_OscInitStruct)
{
    (void)RCC_OscInitStruct;  /* Unused parameter */
    return mock_control.rcc_osc_config_return_value;
}

/**
  * @brief  Mock RCC clock configuration
  * @param  RCC_ClkInitStruct: Clock configuration structure
  * @param  FLatency: Flash latency
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_RCC_ClockConfig(RCC_ClkInitTypeDef* RCC_ClkInitStruct, uint32_t FLatency)
{
    (void)RCC_ClkInitStruct;  /* Unused parameter */
    (void)FLatency;           /* Unused parameter */
    mock_control.system_clock_config_called = true;
    return mock_control.rcc_clk_config_return_value;
}

/**
  * @brief  Mock GPIO initialization
  * @param  GPIOx: GPIO port
  * @param  GPIO_Init: GPIO configuration structure
  * @retval None
  */
void HAL_GPIO_Init(GPIO_TypeDef* GPIOx, GPIO_InitTypeDef* GPIO_Init)
{
    (void)GPIOx;      /* Unused parameter */
    (void)GPIO_Init;  /* Unused parameter */
    mock_control.gpio_init_called = true;
}

/**
  * @brief  Mock GPIO write pin
  * @param  GPIOx: GPIO port
  * @param  GPIO_Pin: GPIO pin
  * @param  PinState: Pin state
  * @retval None
  */
void HAL_GPIO_WritePin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, GPIO_PinState PinState)
{
    if (GPIOx && PinState == GPIO_PIN_SET) {
        GPIOx->ODR |= GPIO_Pin;
    } else if (GPIOx && PinState == GPIO_PIN_RESET) {
        GPIOx->ODR &= ~GPIO_Pin;
    }
}

/**
  * @brief  Mock GPIO toggle pin
  * @param  GPIOx: GPIO port
  * @param  GPIO_Pin: GPIO pin
  * @retval None
  */
void HAL_GPIO_TogglePin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
    mock_control.gpio_toggle_count++;
    mock_control.last_gpio_port = GPIOx;
    mock_control.last_gpio_pin = GPIO_Pin;
    
    if (GPIOx) {
        GPIOx->ODR ^= GPIO_Pin;
    }
}

/**
  * @brief  Mock GPIO read pin
  * @param  GPIOx: GPIO port
  * @param  GPIO_Pin: GPIO pin
  * @retval Pin state
  */
GPIO_PinState HAL_GPIO_ReadPin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
    const GPIO_TypeDef *const gpio = GPIOx; // use const inside for cppcheck
    
    if (gpio && (gpio->IDR & GPIO_Pin)) {
        return GPIO_PIN_SET;
    }
    return GPIO_PIN_RESET;
}

/**
  * @brief  Mock UART initialization
  * @param  huart: UART handle
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_UART_Init(UART_HandleTypeDef* huart)
{
    (void)huart;  /* Unused parameter */
    mock_control.uart_init_called = true;
    return mock_control.uart_init_return_value;
}

/**
  * @brief  Mock Timer base initialization
  * @param  htim: Timer handle
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_TIM_Base_Init(TIM_HandleTypeDef* htim)
{
    (void)htim;  /* Unused parameter */
    mock_control.tim_init_called = true;
    return mock_control.tim_init_return_value;
}

/**
  * @brief  Mock Timer master configuration
  * @param  htim: Timer handle
  * @param  sMasterConfig: Master configuration structure
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_TIMEx_MasterConfigSynchronization(TIM_HandleTypeDef* htim, TIM_MasterConfigTypeDef* sMasterConfig)
{
    (void)htim;           /* Unused parameter */
    (void)sMasterConfig;  /* Unused parameter */
    return mock_control.tim_init_return_value;
}

/**
  * @brief  Mock HAL delay function
  * @param  Delay: Delay in milliseconds
  * @retval None
  */
void HAL_Delay(uint32_t Delay)
{
    mock_control.delay_ms_called_count++;
    mock_control.last_delay_value = Delay;
}

/* Additional mock functions that might be needed */

/**
  * @brief  Mock system clock configuration (weak definition)
  * @retval None
  */
__attribute__((weak)) void SystemClock_Config(void)
{
    mock_control.system_clock_config_called = true;
}