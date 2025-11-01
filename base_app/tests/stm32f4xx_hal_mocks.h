/**
  ******************************************************************************
  * @file    stm32f4xx_hal_mocks.h
  * @author  Test Framework
  * @brief   Mock implementations for STM32F4xx HAL functions
  ******************************************************************************
  * This file provides mock implementations of HAL functions for unit testing
  ******************************************************************************
  */

#ifndef STM32F4XX_HAL_MOCKS_H
#define STM32F4XX_HAL_MOCKS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

/* HAL Status enumeration */
typedef enum {
    HAL_OK       = 0x00U,
    HAL_ERROR    = 0x01U,
    HAL_BUSY     = 0x02U,
    HAL_TIMEOUT  = 0x03U
} HAL_StatusTypeDef;

/* GPIO Definitions */
#define GPIO_PIN_0                 ((uint16_t)0x0001U)  /* Pin 0 selected    */
#define GPIO_PIN_1                 ((uint16_t)0x0002U)  /* Pin 1 selected    */
#define GPIO_PIN_12                ((uint16_t)0x1000U)  /* Pin 12 selected   */
#define GPIO_PIN_13                ((uint16_t)0x2000U)  /* Pin 13 selected   */
#define GPIO_PIN_14                ((uint16_t)0x4000U)  /* Pin 14 selected   */
#define GPIO_PIN_15                ((uint16_t)0x8000U)  /* Pin 15 selected   */

typedef enum {
    GPIO_PIN_RESET = 0U,
    GPIO_PIN_SET
} GPIO_PinState;

typedef enum {
    GPIO_MODE_INPUT                        = 0x00000000U,   /*!< Input Floating Mode                   */
    GPIO_MODE_OUTPUT_PP                    = 0x00000001U,   /*!< Output Push Pull Mode                 */
    GPIO_MODE_OUTPUT_OD                    = 0x00000011U,   /*!< Output Open Drain Mode                */
} GPIO_ModeTypeDef;

typedef enum {
    GPIO_NOPULL   = 0x00000000U,   /*!< No Pull-up or Pull-down activation  */
    GPIO_PULLUP   = 0x00000001U,   /*!< Pull-up activation                  */
    GPIO_PULLDOWN = 0x00000002U    /*!< Pull-down activation                */
} GPIO_PullTypeDef;

typedef enum {
    GPIO_SPEED_FREQ_LOW      = 0x00000000U,  /*!< IO works at 2 MHz, please refer to the product datasheet */
    GPIO_SPEED_FREQ_MEDIUM   = 0x00000001U,  /*!< range 12,5 MHz to 50 MHz, please refer to the product datasheet */
    GPIO_SPEED_FREQ_HIGH     = 0x00000002U,  /*!< range 25MHz to 100MHz, please refer to the product datasheet  */
    GPIO_SPEED_FREQ_VERY_HIGH = 0x00000003U  /*!< range 50MHz to 200MHz, please refer to the product datasheet  */
} GPIO_SpeedTypeDef;

typedef struct {
    uint32_t Pin;       /*!< Specifies the GPIO pins to be configured. */
    uint32_t Mode;      /*!< Specifies the operating mode for the selected pins. */
    uint32_t Pull;      /*!< Specifies the Pull-up or Pull-Down activation for the selected pins. */
    uint32_t Speed;     /*!< Specifies the speed for the selected pins. */
} GPIO_InitTypeDef;

typedef struct {
    volatile uint32_t MODER;    /*!< GPIO port mode register,               Address offset: 0x00      */
    volatile uint32_t OTYPER;   /*!< GPIO port output type register,        Address offset: 0x04      */
    volatile uint32_t OSPEEDR;  /*!< GPIO port output speed register,       Address offset: 0x08      */
    volatile uint32_t PUPDR;    /*!< GPIO port pull-up/pull-down register,  Address offset: 0x0C      */
    volatile uint32_t IDR;      /*!< GPIO port input data register,         Address offset: 0x10      */
    volatile uint32_t ODR;      /*!< GPIO port output data register,        Address offset: 0x14      */
    volatile uint32_t BSRR;     /*!< GPIO port bit set/reset register,      Address offset: 0x18      */
    volatile uint32_t LCKR;     /*!< GPIO port configuration lock register, Address offset: 0x1C      */
    volatile uint32_t AFR[2];   /*!< GPIO alternate function registers,     Address offset: 0x20-0x24 */
} GPIO_TypeDef;

/* UART Definitions */
typedef struct {
    uint32_t BaudRate;                  /*!< This member configures the UART communication baud rate. */
    uint32_t WordLength;                /*!< Specifies the number of data bits transmitted or received in a frame. */
    uint32_t StopBits;                  /*!< Specifies the number of stop bits transmitted. */
    uint32_t Parity;                    /*!< Specifies the parity mode. */
    uint32_t Mode;                      /*!< Specifies whether the Receive or Transmit mode is enabled or disabled. */
    uint32_t HwFlowCtl;                 /*!< Specifies whether the hardware flow control mode is enabled or disabled. */
    uint32_t OverSampling;              /*!< Specifies whether the Over sampling is enabled or disabled. */
} UART_InitTypeDef;

typedef struct {
    void*           Instance;        /*!< UART registers base address        */
    UART_InitTypeDef Init;           /*!< UART communication parameters      */
    uint8_t*        pTxBuffPtr;      /*!< Pointer to UART Tx transfer Buffer */
    uint16_t        TxXferSize;      /*!< UART Tx Transfer size              */
    volatile uint16_t TxXferCount;   /*!< UART Tx Transfer Counter           */
    uint8_t*        pRxBuffPtr;      /*!< Pointer to UART Rx transfer Buffer */
    uint16_t        RxXferSize;      /*!< UART Rx Transfer size              */
    volatile uint16_t RxXferCount;   /*!< UART Rx Transfer Counter           */
} UART_HandleTypeDef;

/* Timer Definitions */
typedef struct {
    uint32_t Prescaler;         /*!< Specifies the prescaler value used to divide the TIM clock. */
    uint32_t CounterMode;       /*!< Specifies the counter mode. */
    uint32_t Period;            /*!< Specifies the period value to be loaded into the active Auto-Reload Register at the next update event. */
    uint32_t ClockDivision;     /*!< Specifies the clock division. */
    uint32_t RepetitionCounter;  /*!< Specifies the repetition counter value. */
    uint32_t AutoReloadPreload; /*!< Specifies the auto-reload preload. */
} TIM_Base_InitTypeDef;

typedef struct {
    void*               Instance;     /*!< Register base address             */
    TIM_Base_InitTypeDef Init;        /*!< TIM Time Base required parameters */
    uint32_t            Channel;      /*!< Active channel                    */
    void*               hdma[7];      /*!< DMA Handlers array                */
    uint32_t            Lock;         /*!< Locking object                    */
    volatile uint32_t   State;        /*!< TIM operation state               */
} TIM_HandleTypeDef;

typedef struct {
    uint32_t MasterOutputTrigger;   /*!< Trigger output (TRGO) selection */
    uint32_t MasterSlaveMode;       /*!< Master/slave mode selection */
    uint32_t MasterOutputTrigger2;  /*!< Trigger output2 (TRGO2) selection */
} TIM_MasterConfigTypeDef;

/* RCC Definitions */
typedef struct {
    uint32_t OscillatorType;       /*!< The oscillators to be configured. */
    uint32_t HSEState;             /*!< The new state of the HSE. */
    uint32_t LSEState;             /*!< The new state of the LSE. */
    uint32_t HSIState;             /*!< The new state of the HSI. */
    uint32_t HSICalibrationValue;  /*!< The HSI calibration trimming value (default is RCC_HSICALIBRATION_DEFAULT). */
    uint32_t LSIState;             /*!< The new state of the LSI. */
    struct {
        uint32_t PLLState;         /*!< The new state of the PLL. */
        uint32_t PLLSource;        /*!< RCC_PLLSource: PLL entry clock source. */
        uint32_t PLLM;             /*!< PLLM: Division factor for PLL VCO input clock. */
        uint32_t PLLN;             /*!< PLLN: Multiplication factor for PLL VCO output clock. */
        uint32_t PLLP;             /*!< PLLP: Division factor for main system clock (SYSCLK). */
        uint32_t PLLQ;             /*!< PLLQ: Division factor for OTG FS, SDIO and RNG clocks. */
    } PLL;
} RCC_OscInitTypeDef;

typedef struct {
    uint32_t ClockType;             /*!< The clock to be configured. */
    uint32_t SYSCLKSource;          /*!< The clock source (SYSCLKS) used as system clock. */
    uint32_t AHBCLKDivider;         /*!< The AHB clock (HCLK) divider. This clock is derived from the system clock (SYSCLK). */
    uint32_t APB1CLKDivider;        /*!< The APB1 clock (PCLK1) divider. This clock is derived from the AHB clock (HCLK). */
    uint32_t APB2CLKDivider;        /*!< The APB2 clock (PCLK2) divider. This clock is derived from the AHB clock (HCLK). */
} RCC_ClkInitTypeDef;

/* Mock GPIO Base Addresses */
#define GPIOA_BASE            (0x40020000UL)
#define GPIOB_BASE            (0x40020400UL)
#define GPIOC_BASE            (0x40020800UL)
#define GPIOD_BASE            (0x40020C00UL)

extern GPIO_TypeDef mock_gpioa;
extern GPIO_TypeDef mock_gpiob;
extern GPIO_TypeDef mock_gpioc;
extern GPIO_TypeDef mock_gpiod;

#define GPIOA               (&mock_gpioa)
#define GPIOB               (&mock_gpiob)
#define GPIOC               (&mock_gpioc)
#define GPIOD               (&mock_gpiod)

/* Mock UART Base Addresses */
extern void* UART4;

/* Mock Timer Base Addresses */
extern void* TIM6;

/* System Core Clock */
extern uint32_t SystemCoreClock;

/* Mock Control Variables */
typedef struct {
    bool hal_init_called;
    bool system_clock_config_called;
    bool gpio_init_called;
    bool uart_init_called;
    bool tim_init_called;
    uint32_t delay_ms_called_count;
    uint32_t last_delay_value;
    uint32_t gpio_toggle_count;
    GPIO_TypeDef* last_gpio_port;
    uint16_t last_gpio_pin;
    HAL_StatusTypeDef hal_init_return_value;
    HAL_StatusTypeDef rcc_osc_config_return_value;
    HAL_StatusTypeDef rcc_clk_config_return_value;
    HAL_StatusTypeDef uart_init_return_value;
    HAL_StatusTypeDef tim_init_return_value;
} MockControl_t;

extern MockControl_t mock_control;

/* Mock Function Declarations */
HAL_StatusTypeDef HAL_Init(void);
HAL_StatusTypeDef HAL_RCC_OscConfig(RCC_OscInitTypeDef* RCC_OscInitStruct);
HAL_StatusTypeDef HAL_RCC_ClockConfig(RCC_ClkInitTypeDef* RCC_ClkInitStruct, uint32_t FLatency);
void HAL_GPIO_Init(GPIO_TypeDef* GPIOx, GPIO_InitTypeDef* GPIO_Init);
void HAL_GPIO_WritePin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, GPIO_PinState PinState);
void HAL_GPIO_TogglePin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
GPIO_PinState HAL_GPIO_ReadPin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
HAL_StatusTypeDef HAL_UART_Init(UART_HandleTypeDef* huart);
HAL_StatusTypeDef HAL_TIM_Base_Init(TIM_HandleTypeDef* htim);
HAL_StatusTypeDef HAL_TIMEx_MasterConfigSynchronization(TIM_HandleTypeDef* htim, TIM_MasterConfigTypeDef* sMasterConfig);
void HAL_Delay(uint32_t Delay);

/* Mock Control Functions */
void mock_reset(void);
void mock_set_hal_init_return(HAL_StatusTypeDef status);
void mock_set_rcc_config_return(HAL_StatusTypeDef status);
void mock_set_uart_init_return(HAL_StatusTypeDef status);
void mock_set_tim_init_return(HAL_StatusTypeDef status);
uint32_t mock_get_delay_call_count(void);
uint32_t mock_get_last_delay_value(void);
uint32_t mock_get_gpio_toggle_count(void);
GPIO_TypeDef* mock_get_last_gpio_port(void);
uint16_t mock_get_last_gpio_pin(void);

/* Clock enable macros (mocked) */
#define __HAL_RCC_PWR_CLK_ENABLE()          do { } while(0)
#define __HAL_RCC_GPIOA_CLK_ENABLE()        do { } while(0)
#define __HAL_RCC_GPIOB_CLK_ENABLE()        do { } while(0)
#define __HAL_RCC_GPIOC_CLK_ENABLE()        do { } while(0)
#define __HAL_RCC_GPIOD_CLK_ENABLE()        do { } while(0)
#define __HAL_RCC_GPIOH_CLK_ENABLE()        do { } while(0)

/* Power macros (mocked) */
#define __HAL_PWR_VOLTAGESCALING_CONFIG(x)  do { } while(0)
#define __disable_irq()                     do { } while(0)

/* Constants */
#define RCC_OSCILLATORTYPE_HSI              0x00000001U
#define RCC_HSI_ON                          0x00000001U
#define RCC_HSICALIBRATION_DEFAULT          16U
#define RCC_PLL_ON                          0x00000001U
#define RCC_PLLSOURCE_HSI                   0x00000000U
#define RCC_PLLP_DIV2                       0x00000002U
#define RCC_CLOCKTYPE_HCLK                  0x00000001U
#define RCC_CLOCKTYPE_SYSCLK                0x00000002U
#define RCC_CLOCKTYPE_PCLK1                 0x00000004U
#define RCC_CLOCKTYPE_PCLK2                 0x00000008U
#define RCC_SYSCLKSOURCE_PLLCLK             0x00000002U
#define RCC_SYSCLK_DIV1                     0x00000000U
#define RCC_HCLK_DIV4                       0x00001400U
#define RCC_HCLK_DIV2                       0x00001000U
#define FLASH_LATENCY_5                     0x00000005U
#define PWR_REGULATOR_VOLTAGE_SCALE1        0x00004000U

#define UART_WORDLENGTH_8B                  0x00000000U
#define UART_STOPBITS_1                     0x00000000U
#define UART_PARITY_NONE                    0x00000000U
#define UART_MODE_TX_RX                     0x0000000CU
#define UART_HWCONTROL_NONE                 0x00000000U
#define UART_OVERSAMPLING_16                0x00000000U

#define TIM_COUNTERMODE_UP                  0x00000000U
#define TIM_AUTORELOAD_PRELOAD_DISABLE      0x00000000U
#define TIM_TRGO_RESET                      0x00000000U
#define TIM_MASTERSLAVEMODE_DISABLE         0x00000000U

#ifdef __cplusplus
}
#endif

#endif /* STM32F4XX_HAL_MOCKS_H */