/**
  ******************************************************************************
  * @file    main_testable.h
  * @author  Test Framework
  * @brief   Testable version of main.h for unit testing
  ******************************************************************************
  * This file provides testable interfaces for functions in main.c
  ******************************************************************************
  */

#ifndef MAIN_TESTABLE_H
#define MAIN_TESTABLE_H

#ifdef __cplusplus
extern "C" {
#endif

/* Include mock HAL instead of real HAL for testing */
#include "stm32f4xx_hal_mocks.h"

/* External variables that need to be accessible for testing */
extern TIM_HandleTypeDef htim6;
extern UART_HandleTypeDef huart4;

/* Function prototypes - make internal functions testable */
void SystemClock_Config(void);
void MX_GPIO_Init(void);
void MX_UART4_Init(void);
void MX_TIM6_Init(void);
void Error_Handler(void);

/* Test helper functions */
int main_init_only(void);  /* Initialize without infinite loop */
void main_single_iteration(void);  /* Single iteration of main loop */

#ifdef __cplusplus
}
#endif

#endif /* MAIN_TESTABLE_H */