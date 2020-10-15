#include "stm32f1xx_ll_usart.h"
#include "stm32f1xx_ll_gpio.h"
#include "stm32f1xx_ll_bus.h"
#include "stm32f1xx_ll_rcc.h"
#include "stm32f1xx_ll_tim.h" 

//PA2 Tx PA3 Rx


void conf(USART_TypeDef us) {
	LL_USART_InitTypeDef USART_InitStruct;
	
	if (&us == USART1) {
		LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_USART1);
		USART1->BRR = 9600;
	}else if(&us == USART2){
		LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_USART2);
		USART2->BRR = 9600;
	}else if(&us == USART3){
		LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_USART3);
		USART3->BRR = 9600;
	}	
	
	LL_USART_SetDataWidth(&us, LL_USART_DATAWIDTH_9B);
	LL_USART_SetStopBitsLength(&us, LL_USART_STOPBITS_1);
	LL_USART_SetParity(&us, LL_USART_PARITY_ODD);
	LL_USART_SetTransferDirection(&us, LL_USART_DIRECTION_TX_RX);
	LL_USART_SetHWFlowCtrl(&us, LL_USART_HWCONTROL_NONE);
	
	LL_USART_Enable(&us);
}

void write(USART_TypeDef us, uint16_t val, int t_msg){
	int index = 0;
	while (index<t_msg){
		if(LL_USART_IsActiveFlag_TXE(&us)){
			LL_USART_TransmitData9(&us, val);
			index++;
		}
	}
}

