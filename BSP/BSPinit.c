#include "BSP.h"
#include "uartFinshCharIO.h"

/**
  * @brief System Clock Configuration
  * @retval None
  */
extern void SystemClock_Config(void);

void myBSPInit(void)
{
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
  MX_DMA_Init();
  MX_ADC1_Init();
  MX_TIM1_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_TIM4_Init();
  MX_TIM5_Init();
  MX_USART3_UART_Init();
  MX_SPI1_Init();
  
  /* Init FinSH */
  FinshIoInit();
}

INIT_BOARD_EXPORT(myBSPInit);

