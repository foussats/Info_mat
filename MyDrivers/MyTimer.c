// TOUT A FAIRE !! //

/*
 indispensable pour pouvoir adresser les registres des périphériques.
 Rem : OBLIGATION d'utiliser les définitions utiles contenues dans ce fichier (ex : TIM_CR1_CEN, RCC_APB1ENR_TIM2EN ...)
 pour une meilleure lisibilité du code.

 Pour les masques, utiliser également les définitions proposée
 Rappel : pour mettre à 1  , reg = reg | Mask (ou Mask est le représente le ou les bits à positionner à 1)
				  pour mettre à 0  , reg = reg&~ Mask (ou Mask est le représente le ou les bits à positionner à 0)
 
*/ 
#include "stm32f103xb.h" 
#include "MyTimer.h"
#include "Chrono.h"

void (*pt_fct_tim1)(void);
void (*pt_fct_tim2)(void);
void (*pt_fct_tim3)(void);
void (*pt_fct_tim4)(void);

void MyTimer_Conf(TIM_TypeDef * Timer,int Arr, int Psc){
	
	if(Timer == TIM1){
		RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;
	}else if(Timer == TIM2){
		RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
	}else if(Timer == TIM3){
		RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
	}else if(Timer == TIM4){
		RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;
	}
	
	Timer -> ARR = Arr;
	Timer -> PSC = Psc;
}

void MyTimer_Start(TIM_TypeDef * Timer){
		Timer->CR1 |= TIM_CR1_CEN;	
}

void MyTimer_Stop(TIM_TypeDef * Timer){
		Timer->CR1 &= ~(TIM_CR1_CEN);
}


void MyTimer_IT_Conf(TIM_TypeDef * Timer, void (*IT_function) (void),int Prio){
	Timer->DIER |= TIM_DIER_UIE; //mise à 0 de DIER registre d'autorisation d'interruption
	
	if (Timer==TIM1){
		pt_fct_tim1 = IT_function;
		NVIC->IP[TIM1_UP_IRQn] = Prio<<4;
		NVIC->ISER[0] |= 0x1<<TIM1_UP_IRQn; //mise à 1 du bit de l'interruption
	}else if (Timer==TIM2){
		pt_fct_tim2 = IT_function;
		NVIC->IP[TIM2_IRQn] |= Prio<<4;
		NVIC->ISER[0] |= 0x1<<TIM2_IRQn;
	}else if (Timer==TIM3){
		pt_fct_tim3 = IT_function;
		NVIC->IP[TIM3_IRQn] |= Prio<<4;
		NVIC->ISER[0] |= 0x1<<TIM3_IRQn;
	}else if (Timer==TIM4){
		pt_fct_tim4 = IT_function;
		NVIC->IP[TIM4_IRQn] |= Prio<<4;
		NVIC->ISER[0] |= 0x1<<TIM4_IRQn;
	}
	
}

void MyTimer_IT_Disable(TIM_TypeDef * Timer){
	Timer->DIER &= ~(TIM_DIER_UIE); //mise à 0 de DIER registre d'autorisation d'interruption
	if (Timer==TIM1){
		NVIC->ISER[0] &= ~(0x1<<TIM1_UP_IRQn);
	}else if (Timer==TIM2){
		NVIC->ISER[0] &= ~(0x1<<TIM2_IRQn);
	}else if (Timer==TIM3){
		NVIC->ISER[0] &= ~(0x1<<TIM3_IRQn);
	}else if (Timer==TIM4){
		NVIC->ISER[0] &= ~(0x1<<TIM4_IRQn);
	}
}


void TIM1_UP_IRQHandler(void){
	TIM1->SR &= ~(TIM_SR_UIF); //mise à 0 du status register
	(*pt_fct_tim1)();
}

void TIM2_IRQHandler(void){
	TIM2->SR &= ~(TIM_SR_UIF);
	(*pt_fct_tim2)();
}

void TIM3_IRQHandler(void){
	TIM3->SR &= ~(TIM_SR_UIF);
	(*pt_fct_tim3)();
}

void TIM4_IRQHandler(void){
	TIM4->SR &= ~(TIM_SR_UIF);
	(*pt_fct_tim4)();
}





















