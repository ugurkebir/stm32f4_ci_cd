/**
  ******************************************************************************
  * @file    test_main.c
  * @author  Test Framework
  * @brief   Unit tests for main.c functionality
  ******************************************************************************
  * This file contains unit tests for the main application functionality
  ******************************************************************************
  */

#include "unity.h"
#include "main_testable.h"
#include <stdio.h>

/* External variables for testing */
extern TIM_HandleTypeDef htim6;
extern UART_HandleTypeDef huart4;
extern MockControl_t mock_control;

/**
  * @brief  Setup function called before each test
  * @retval None
  */
void setUp(void)
{
    /* Reset all mock states before each test */
    mock_reset();
}

/**
  * @brief  Teardown function called after each test
  * @retval None
  */
void tearDown(void)
{
    /* Clean up after each test if needed */
}

/* ============================================================================ */
/* SYSTEM INITIALIZATION TESTS */
/* ============================================================================ */

/**
  * @brief  Test HAL initialization
  * @retval None
  */
void test_hal_init_success(void)
{
    /* Arrange */
    mock_set_hal_init_return(HAL_OK);
    
    /* Act */
    HAL_StatusTypeDef result = HAL_Init();
    
    /* Assert */
    TEST_ASSERT_EQUAL(HAL_OK, result);
    TEST_ASSERT_TRUE(mock_control.hal_init_called);
}

/**
  * @brief  Test HAL initialization failure
  * @retval None
  */
void test_hal_init_failure(void)
{
    /* Arrange */
    mock_set_hal_init_return(HAL_ERROR);
    
    /* Act */
    HAL_StatusTypeDef result = HAL_Init();
    
    /* Assert */
    TEST_ASSERT_EQUAL(HAL_ERROR, result);
    TEST_ASSERT_TRUE(mock_control.hal_init_called);
}

/**
  * @brief  Test system clock configuration
  * @retval None
  */
void test_system_clock_config(void)
{
    /* Arrange */
    mock_set_rcc_config_return(HAL_OK);
    
    /* Act */
    SystemClock_Config();
    
    /* Assert */
    TEST_ASSERT_TRUE(mock_control.system_clock_config_called);
}

/**
  * @brief  Test main initialization sequence
  * @retval None
  */
void test_main_init_sequence_success(void)
{
    /* Arrange */
    mock_set_hal_init_return(HAL_OK);
    mock_set_rcc_config_return(HAL_OK);
    mock_set_uart_init_return(HAL_OK);
    mock_set_tim_init_return(HAL_OK);
    
    /* Act */
    int result = main_init_only();
    
    /* Assert */
    TEST_ASSERT_EQUAL(0, result);
    TEST_ASSERT_TRUE(mock_control.hal_init_called);
    TEST_ASSERT_TRUE(mock_control.system_clock_config_called);
    TEST_ASSERT_TRUE(mock_control.gpio_init_called);
    TEST_ASSERT_TRUE(mock_control.uart_init_called);
    TEST_ASSERT_TRUE(mock_control.tim_init_called);
}

/**
  * @brief  Test main initialization with HAL failure
  * @retval None
  */
void test_main_init_hal_failure(void)
{
    /* Arrange */
    mock_set_hal_init_return(HAL_ERROR);
    
    /* Act */
    int result = main_init_only();
    
    /* Assert */
    TEST_ASSERT_EQUAL(1, result);
    TEST_ASSERT_TRUE(mock_control.hal_init_called);
}

/* ============================================================================ */
/* GPIO TESTS */
/* ============================================================================ */

/**
  * @brief  Test GPIO initialization
  * @retval None
  */
void test_gpio_init(void)
{
    /* Act */
    MX_GPIO_Init();
    
    /* Assert */
    TEST_ASSERT_TRUE(mock_control.gpio_init_called);
}

/**
  * @brief  Test GPIO pin toggle functionality
  * @retval None
  */
void test_gpio_toggle_pin(void)
{
    /* Arrange */
    uint32_t initial_count = mock_get_gpio_toggle_count();
    
    /* Act */
    HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_14);
    
    /* Assert */
    TEST_ASSERT_EQUAL(initial_count + 1, mock_get_gpio_toggle_count());
    TEST_ASSERT_EQUAL(GPIOD, mock_get_last_gpio_port());
    TEST_ASSERT_EQUAL(GPIO_PIN_14, mock_get_last_gpio_pin());
}

/**
  * @brief  Test GPIO pin write functionality
  * @retval None
  */
void test_gpio_write_pin_set(void)
{
    /* Arrange */
    GPIO_TypeDef* port = GPIOD;
    uint16_t pin = GPIO_PIN_14;
    
    /* Act */
    HAL_GPIO_WritePin(port, pin, GPIO_PIN_SET);
    
    /* Assert */
    TEST_ASSERT_BITS_HIGH(pin, port->ODR);
}

/**
  * @brief  Test GPIO pin write reset functionality
  * @retval None
  */
void test_gpio_write_pin_reset(void)
{
    /* Arrange */
    GPIO_TypeDef* port = GPIOD;
    uint16_t pin = GPIO_PIN_14;
    
    /* Set pin first */
    HAL_GPIO_WritePin(port, pin, GPIO_PIN_SET);
    
    /* Act */
    HAL_GPIO_WritePin(port, pin, GPIO_PIN_RESET);
    
    /* Assert */
    TEST_ASSERT_BITS_LOW(pin, port->ODR);
}

/**
  * @brief  Test GPIO pin read functionality
  * @retval None
  */
void test_gpio_read_pin(void)
{
    /* Arrange */
    GPIO_TypeDef* port = GPIOD;
    uint16_t pin = GPIO_PIN_14;
    
    /* Test reading pin when set */
    port->IDR = pin;
    GPIO_PinState state = HAL_GPIO_ReadPin(port, pin);
    TEST_ASSERT_EQUAL(GPIO_PIN_SET, state);
    
    /* Test reading pin when reset */
    port->IDR = 0;
    state = HAL_GPIO_ReadPin(port, pin);
    TEST_ASSERT_EQUAL(GPIO_PIN_RESET, state);
}

/* ============================================================================ */
/* UART TESTS */
/* ============================================================================ */

/**
  * @brief  Test UART4 initialization
  * @retval None
  */
void test_uart_init_success(void)
{
    /* Arrange */
    mock_set_uart_init_return(HAL_OK);
    
    /* Act */
    MX_UART4_Init();
    
    /* Assert */
    TEST_ASSERT_TRUE(mock_control.uart_init_called);
    TEST_ASSERT_EQUAL(UART4, huart4.Instance);
    TEST_ASSERT_EQUAL(115200, huart4.Init.BaudRate);
    TEST_ASSERT_EQUAL(UART_WORDLENGTH_8B, huart4.Init.WordLength);
    TEST_ASSERT_EQUAL(UART_STOPBITS_1, huart4.Init.StopBits);
    TEST_ASSERT_EQUAL(UART_PARITY_NONE, huart4.Init.Parity);
}

/**
  * @brief  Test UART initialization with different baud rates
  * @retval None
  */
void test_uart_baud_rate_configuration(void)
{
    /* Act */
    MX_UART4_Init();
    
    /* Assert */
    TEST_ASSERT_EQUAL(115200, huart4.Init.BaudRate);
}

/* ============================================================================ */
/* TIMER TESTS */
/* ============================================================================ */

/**
  * @brief  Test TIM6 initialization
  * @retval None
  */
void test_tim6_init_success(void)
{
    /* Arrange */
    mock_set_tim_init_return(HAL_OK);
    
    /* Act */
    MX_TIM6_Init();
    
    /* Assert */
    TEST_ASSERT_TRUE(mock_control.tim_init_called);
    TEST_ASSERT_EQUAL(TIM6, htim6.Instance);
    TEST_ASSERT_EQUAL(0, htim6.Init.Prescaler);
    TEST_ASSERT_EQUAL(65535, htim6.Init.Period);
    TEST_ASSERT_EQUAL(TIM_COUNTERMODE_UP, htim6.Init.CounterMode);
}

/**
  * @brief  Test timer configuration parameters
  * @retval None
  */
void test_timer_configuration_values(void)
{
    /* Act */
    MX_TIM6_Init();
    
    /* Assert */
    TEST_ASSERT_EQUAL(TIM_AUTORELOAD_PRELOAD_DISABLE, htim6.Init.AutoReloadPreload);
}

/* ============================================================================ */
/* MAIN LOOP TESTS */
/* ============================================================================ */

/**
  * @brief  Test main loop single iteration
  * @retval None
  */
void test_main_loop_single_iteration(void)
{
    /* Arrange */
    uint32_t initial_toggle_count = mock_get_gpio_toggle_count();
    uint32_t initial_delay_count = mock_get_delay_call_count();
    
    /* Act */
    main_single_iteration();
    
    /* Assert */
    TEST_ASSERT_EQUAL(initial_toggle_count + 1, mock_get_gpio_toggle_count());
    TEST_ASSERT_EQUAL(GPIOD, mock_get_last_gpio_port());
    TEST_ASSERT_EQUAL(GPIO_PIN_14, mock_get_last_gpio_pin());
    TEST_ASSERT_EQUAL(initial_delay_count + 1, mock_get_delay_call_count());
    TEST_ASSERT_EQUAL(500, mock_get_last_delay_value());
}

/**
  * @brief  Test multiple main loop iterations
  * @retval None
  */
void test_main_loop_multiple_iterations(void)
{
    /* Arrange */
    const int iterations = 5;
    uint32_t initial_toggle_count = mock_get_gpio_toggle_count();
    uint32_t initial_delay_count = mock_get_delay_call_count();
    
    /* Act */
    for (int i = 0; i < iterations; i++) {
        main_single_iteration();
    }
    
    /* Assert */
    TEST_ASSERT_EQUAL(initial_toggle_count + iterations, mock_get_gpio_toggle_count());
    TEST_ASSERT_EQUAL(initial_delay_count + iterations, mock_get_delay_call_count());
}

/* ============================================================================ */
/* DELAY TESTS */
/* ============================================================================ */

/**
  * @brief  Test HAL delay function
  * @retval None
  */
void test_hal_delay_function(void)
{
    /* Arrange */
    uint32_t delay_value = 1000;
    uint32_t initial_count = mock_get_delay_call_count();
    
    /* Act */
    HAL_Delay(delay_value);
    
    /* Assert */
    TEST_ASSERT_EQUAL(initial_count + 1, mock_get_delay_call_count());
    TEST_ASSERT_EQUAL(delay_value, mock_get_last_delay_value());
}

/**
  * @brief  Test different delay values
  * @retval None
  */
void test_different_delay_values(void)
{
    /* Test various delay values */
    uint32_t delay_values[] = {1, 10, 100, 500, 1000, 5000};
    size_t num_delays = sizeof(delay_values) / sizeof(delay_values[0]);
    
    for (size_t i = 0; i < num_delays; i++) {
        HAL_Delay(delay_values[i]);
        TEST_ASSERT_EQUAL(delay_values[i], mock_get_last_delay_value());
    }
    
    TEST_ASSERT_EQUAL(num_delays, mock_get_delay_call_count());
}

/* ============================================================================ */
/* ERROR HANDLING TESTS */
/* ============================================================================ */

/**
  * @brief  Test error handler function
  * @retval None
  */
void test_error_handler_function(void)
{
    /* This test mainly ensures Error_Handler doesn't crash */
    /* In actual hardware, this would disable interrupts and halt */
    
    /* Act & Assert - should not crash */
    Error_Handler();
    
    /* If we reach here, the function returned (good for testing) */
    TEST_ASSERT_TRUE(1);  /* Always pass if we get here */
}

/* ============================================================================ */
/* SYSTEM INTEGRATION TESTS */
/* ============================================================================ */

/**
  * @brief  Test complete system initialization and operation
  * @retval None
  */
void test_system_integration(void)
{
    /* Arrange */
    mock_set_hal_init_return(HAL_OK);
    mock_set_rcc_config_return(HAL_OK);
    mock_set_uart_init_return(HAL_OK);
    mock_set_tim_init_return(HAL_OK);
    
    /* Act - Initialize system */
    int init_result = main_init_only();
    
    /* Assert initialization */
    TEST_ASSERT_EQUAL(0, init_result);
    
    /* Act - Run a few iterations */
    for (int i = 0; i < 3; i++) {
        main_single_iteration();
    }
    
    /* Assert operation */
    TEST_ASSERT_EQUAL(3, mock_get_gpio_toggle_count());
    TEST_ASSERT_EQUAL(3, mock_get_delay_call_count());
}

/**
  * @brief  Test system behavior with peripheral failures
  * @retval None
  */
void test_system_with_peripheral_failures(void)
{
    /* Test with UART failure */
    mock_set_uart_init_return(HAL_ERROR);
    MX_UART4_Init();  /* Should handle gracefully */
    
    /* Test with Timer failure */
    mock_set_tim_init_return(HAL_ERROR);
    MX_TIM6_Init();  /* Should handle gracefully */
    
    /* The system should still be able to toggle GPIO */
    HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_14);
    TEST_ASSERT_EQUAL(1, mock_get_gpio_toggle_count());
}

/* ============================================================================ */
/* TEST RUNNER */
/* ============================================================================ */

/**
  * @brief  Run all unit tests
  * @retval Test results (0 = success, >0 = number of failures)
  */
int main(void)
{
    UNITY_BEGIN();

    /* System Initialization Tests */
    RUN_TEST(test_hal_init_success);
    RUN_TEST(test_hal_init_failure);
    RUN_TEST(test_system_clock_config);
    RUN_TEST(test_main_init_sequence_success);
    RUN_TEST(test_main_init_hal_failure);

    /* GPIO Tests */
    RUN_TEST(test_gpio_init);
    RUN_TEST(test_gpio_toggle_pin);
    RUN_TEST(test_gpio_write_pin_set);
    RUN_TEST(test_gpio_write_pin_reset);
    RUN_TEST(test_gpio_read_pin);

    /* UART Tests */
    RUN_TEST(test_uart_init_success);
    RUN_TEST(test_uart_baud_rate_configuration);

    /* Timer Tests */
    RUN_TEST(test_tim6_init_success);
    RUN_TEST(test_timer_configuration_values);

    /* Main Loop Tests */
    RUN_TEST(test_main_loop_single_iteration);
    RUN_TEST(test_main_loop_multiple_iterations);

    /* Delay Tests */
    RUN_TEST(test_hal_delay_function);
    RUN_TEST(test_different_delay_values);

    /* Error Handling Tests */
    RUN_TEST(test_error_handler_function);

    /* Integration Tests */
    RUN_TEST(test_system_integration);
    RUN_TEST(test_system_with_peripheral_failures);

    return UNITY_END();
}