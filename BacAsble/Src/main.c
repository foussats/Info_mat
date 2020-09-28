/**
  ******************************************************************************
  * @file    Templates_LL/Src/main.c
  * @author  MCD Application Team
  * @brief   Main program body through the LL API
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2016 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
#include "stm32f1xx_ll_rcc.h" // utile dans la fonction SystemClock_Config
#include "stm32f1xx_ll_utils.h"   // utile dans la fonction SystemClock_Config
#include "stm32f1xx_ll_system.h" // utile dans la fonction SystemClock_Config

#include "Chrono.h"
#include "MyTimer.h"

void  SystemClock_Config(void);
int valeur8;
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{	
  /* Configure the system clock to 72 MHz */
  SystemClock_Config();
	
	RCC->APB2ENR = RCC->APB2ENR | RCC_APB2ENR_IOPAEN ;
	
	//config
	//floating input start & stop
	GPIOC->CRH |= GPIO_CRH_CNF8_0;
	GPIOC->CRH &= ~(GPIO_CRH_CNF8_1);
		
	GPIOC->CRH &= ~(GPIO_CRH_MODE8_0);
	GPIOC->CRH &= ~(GPIO_CRH_MODE8_1);

//output pushpull mode [1::0] max speed 2MHz
	GPIOC->CRH &= ~(GPIO_CRH_CNF10_0);
	GPIOC->CRH &= ~(GPIO_CRH_CNF10_1);
		
	GPIOC->CRH &= ~(GPIO_CRH_CNF10_0);
	GPIOC->CRH |= GPIO_CRH_MODE10_1;

	//output opendrain max speed 2MHz
	/*
	GPIOC->CRH |= GPIO_CRH_CNF10_0;
	GPIOC->CRH &= ~(GPIO_CRH_CNF10_1);
		
	GPIOC->CRH &= ~(GPIO_CRH_CNF10_0);
	GPIOC->CRH |= GPIO_CRH_MODE10_1;
	*/
	
	//start and stop
	while(1){
		valeur8 = (GPIOC->IDR &(0x1<<8))>>8;
		//if (GPIO_IDR_IDR){
		if (valeur8 == 1){
			GPIOC->ODR |= GPIO_ODR_ODR10;
		}else{
			GPIOC->ODR &= ~(GPIO_ODR_ODR10);
	}
}
	
	/*TIM1->CR2|=0x1 ;
	TIM1->CR2=0xFFFF ;
	TIM1->CR2 &= ~0x0040 ; // pas g e ni al comme l i s i b i l i t e
	TIM1->CR2=0xFFFF ;
	TIM1->CR2&=~(1<<6) ; // pl u s l i s i b l e non ?
	GPIOB->CRL=(0xFFFF ) ;
	GPIOB->CRL&=~(0xF<<8) ;
	GPIOB->CRL|= (0x5<<8) ;
	GPIOB->ODR|=GPIO_ODR_ODR1 ; // grand s t y l e !
	*/
	
	/*SPI1->CR1 |= SPI_CR1_CPOL;
	SPI1->CR1 &= ~(SPI_CR1_DFF);
	USART2->CR2 &= ~(0xF<<USART_CR2_STOP_Pos);
	USART2->CR2 |= (0x2<<USART_CR2_STOP_Pos);
	*/
	
  /* Add your application code here */
  // Configuration chronomètre
	Chrono_Conf(TIM2);
	
	
	
	
	// Lancement chronomètre
	Chrono_Start(); 
  
  /* Infinite loop */
  while (1)
  {
  }
}







/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow :
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 72000000
  *            HCLK(Hz)                       = 72000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 2
  *            APB2 Prescaler                 = 1
  *            HSE Frequency(Hz)              = 8000000
  *            PLLMUL                         = 9
  *            Flash Latency(WS)              = 2
  * @param  None
  * @retval None
  */
void SystemClock_Config(void)
{
  /* Set FLASH latency */
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_2);

  /* Enable HSE oscillator */
	// ********* Commenter la ligne ci-dessous pour MCBSTM32 *****************
	// ********* Conserver la ligne si Nucléo*********************************
  LL_RCC_HSE_EnableBypass();
  LL_RCC_HSE_Enable();
  while(LL_RCC_HSE_IsReady() != 1)
  {
  };

  /* Main PLL configuration and activation */
  LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSE_DIV_1, LL_RCC_PLL_MUL_9);

  LL_RCC_PLL_Enable();
  while(LL_RCC_PLL_IsReady() != 1)
  {
  };

  /* Sysclk activation on the main PLL */
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL)
  {
  };

  /* Set APB1 & APB2 prescaler*/
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_2);
  LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);

  /* Set systick to 1ms in using frequency set to 72MHz */
  //LL_Init1msTick(72000000); !! décommenter que si l'IT est récupérée

  /* Update CMSIS variable (which can be updated also through SystemCoreClockUpdate function) */
  LL_SetSystemCoreClock(72000000);
}



/* ==============   BOARD SPECIFIC CONFIGURATION CODE END      ============== */

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
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
