// A COMPLETER

/*
Service permettant de chornométrer jusqu'à 59mn 59s 99 1/100
Utilise un timer au choix (TIMER1 à TIMER4).
Utilise la lib MyTimers.h /.c
*/



#include "Chrono.h"
#include "MyTimer.h"
#include "stm32f1xx_ll_bus.h"
#include "stm32f1xx_ll_tim.h" 
#include "stm32f1xx_ll_gpio.h"
#include "stm32f1xx_ll_rcc.h" 

// variable privée de type Time qui mémorise la durée mesurée
Time Chrono_Time; // rem : static rend la visibilité de la variable Chrono_Time limitée à ce fichier 

// variable privée qui mémorise pour le module le timer utilisé par le module
static TIM_TypeDef * Chrono_Timer=TIM1; // init par défaut au cas où l'utilisateur ne lance pas Chrono_Conf avant toute autre fct.

// déclaration callback appelé toute les 10ms
void Chrono_Task_10ms(void);

static int etat_precedent_chrono = 0;
static int chrono_started = 0;

static void Chrono_Conf_io(void){
	LL_GPIO_InitTypeDef GPIO_InitStruct;
	
	LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOC);
	
	/*LL_GPIO_SetPinMode(GPIOC, LL_GPIO_PIN_8, LL_GPIO_MODE_FLOATING);
	LL_GPIO_SetPinMode(GPIOC, LL_GPIO_PIN_13, LL_GPIO_MODE_FLOATING);
	LL_GPIO_SetPinMode(GPIOC, LL_GPIO_PIN_10, LL_GPIO_MODE_OUTPUT);
	LL_GPIO_SetPinSpeed(GPIOC, LL_GPIO_PIN_8, LL_GPIO_SPEED_FREQ_LOW);
	LL_GPIO_SetPinSpeed(GPIOC, LL_GPIO_PIN_13, LL_GPIO_SPEED_FREQ_LOW);
	LL_GPIO_SetPinSpeed(GPIOC, LL_GPIO_PIN_10, LL_GPIO_SPEED_FREQ_LOW);
	LL_GPIO_SetPinOutpuType(GPIOC, LL_GPIO_PIN_10, LL_GPIO_OUTPUT_PUSHPULL);
	*/
	
	GPIO_InitStruct.Pin = LL_GPIO_PIN_10;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
	LL_GPIO_Init(GPIOC, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin = LL_GPIO_PIN_8 | LL_GPIO_PIN_13;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_FLOATING;
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
	LL_GPIO_Init(GPIOC, &GPIO_InitStruct);
	
}

/**
	* @brief  Configure le chronomètre. 
  * @note   A lancer avant toute autre fonction.
	* @param  Timer : indique le timer à utiliser par le chronomètre, TIM1, TIM2, TIM3 ou TIM4
  * @retval None
  */
void Chrono_Conf(TIM_TypeDef * Timer)
{
	// Reset Time
	Chrono_Time.Hund=0;
	Chrono_Time.Sec=0;
	Chrono_Time.Min=0;
	
	// Fixation du Timer
	Chrono_Timer=Timer;
	
	//appel de chrono_conf_io
	Chrono_Conf_io();
	
	// Réglage Timer pour un débordement à 10ms
	MyTimer_Conf(Chrono_Timer,999, 719);
	
	// Réglage interruption du Timer avec callback : Chrono_Task_10ms()
	MyTimer_IT_Conf(Chrono_Timer, Chrono_Task_10ms,3);
	
	// Validation IT
	MyTimer_IT_Enable(Chrono_Timer);
	
	
}


/**
	* @brief  Démarre le chronomètre. 
  * @note   si la durée dépasse 59mn 59sec 99 Hund, elle est remise à zéro et repart
	* @param  Aucun
  * @retval Aucun
  */
void Chrono_Start(void)
{
	MyTimer_Start(Chrono_Timer);
}


/**
	* @brief  Arrête le chronomètre. 
  * @note   
	* @param  Aucun
  * @retval Aucun
  */
void Chrono_Stop(void)
{
	MyTimer_Stop(Chrono_Timer);
}


/**
	* @brief  Remet le chronomètre à 0 
  * @note   
	* @param  Aucun
  * @retval Aucun
  */
void Chrono_Reset(void)
{
  // Arrêt Chrono
	MyTimer_Stop(Chrono_Timer);

	// Reset Time
	Chrono_Time.Hund=0;
	Chrono_Time.Sec=0;
	Chrono_Time.Min=0;
}


/**
	* @brief  Renvoie l'adresse de la variable Time privée gérée dans le module Chrono.c
  * @note   
	* @param  Aucun
  * @retval adresse de la variable Time
  */
Time * Chrono_Read(void)
{
	return &Chrono_Time;
}




/**
	* @brief  incrémente la variable privée Chron_Time modulo 60mn 
  * @note   
	* @param  Aucun
  * @retval Aucun
  */
void Chrono_Task_10ms(void)
{ 
	Chrono_Time.Hund++;
	if (Chrono_Time.Hund==20){
		LL_GPIO_ResetOutputPin(GPIOC, LL_GPIO_PIN_10);
	}
	if (Chrono_Time.Hund==100)
	{
		Chrono_Time.Sec++;
		Chrono_Time.Hund=0;
		if (LL_GPIO_IsInputPinSet(GPIOC, LL_GPIO_PIN_10)){
			LL_GPIO_ResetOutputPin(GPIOC, LL_GPIO_PIN_10);
	}else{
		
	}
	if (Chrono_Time.Sec==60)
	{
		Chrono_Time.Min++;
		Chrono_Time.Sec=0;
	}
	if (Chrono_Time.Min==60)
	{
		Chrono_Time.Hund=0;
	}
	
}

void Chrono_Background(void){
 /*if(LL_GPIO_IsInputPinSet(GPIOC, LL_GPIO_PIN_8) == 1) {
	LL_GPIO_SetOutputPin(GPIOC,LL_GPIO_PIN_10);
 }else{
	LL_GPIO_ResetOutputPin(GPIOC, LL_GPIO_PIN_10);
 }
 */
	int etat_present_chrono = LL_GPIO_IsInputPinSet(GPIOC, LL_GPIO_PIN_8);
 if(etat_precedent_chrono == 0 && etat_present_chrono ==1){
	 if (chrono_started == 1) {
			chrono_started=0;
			Chrono_Stop();
	 }else{
			chrono_started = 1;
			Chrono_Start();
	 }
 }else if (etat_precedent_chrono == 1 && etat_present_chrono == 0){
	 etat_present_chrono = etat_precedent_chrono;

 }
 
 if (LL_GPIO_IsInputPinSet(GPIOC, LL_GPIO_PIN_13) == 1){
	 Chrono_Reset();
	 chrono_started = 0;
	 Chrono_Stop();
}

}


