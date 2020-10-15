// A COMPLETER

/*
Service permettant de chornom�trer jusqu'� 59mn 59s 99 1/100
Utilise un timer au choix (TIMER1 � TIMER4).
Utilise la lib MyTimers.h /.c
*/



#include "Chrono.h"
#include "MyTimer.h"
#include "stm32f1xx_ll_bus.h"
#include "stm32f1xx_ll_tim.h" 
#include "stm32f1xx_ll_gpio.h"
#include "stm32f1xx_ll_rcc.h" 

// variable priv�e de type Time qui m�morise la dur�e mesur�e
Time Chrono_Time; // rem : static rend la visibilit� de la variable Chrono_Time limit�e � ce fichier 

// variable priv�e qui m�morise pour le module le timer utilis� par le module
static TIM_TypeDef * Chrono_Timer=TIM1; // init par d�faut au cas o� l'utilisateur ne lance pas Chrono_Conf avant toute autre fct.

// d�claration callback appel� toute les 10ms
void Chrono_Task_10ms(void);

static int chr=1;

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
	* @brief  Configure le chronom�tre. 
  * @note   A lancer avant toute autre fonction.
	* @param  Timer : indique le timer � utiliser par le chronom�tre, TIM1, TIM2, TIM3 ou TIM4
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
	
	// R�glage Timer pour un d�bordement � 10ms
	MyTimer_Conf(Chrono_Timer,999, 719);
	
	// R�glage interruption du Timer avec callback : Chrono_Task_10ms()
	MyTimer_IT_Conf(Chrono_Timer, Chrono_Task_10ms,3);
	
	// Validation IT
	MyTimer_IT_Enable(Chrono_Timer);
	
	
}


/**
	* @brief  D�marre le chronom�tre. 
  * @note   si la dur�e d�passe 59mn 59sec 99 Hund, elle est remise � z�ro et repart
	* @param  Aucun
  * @retval Aucun
  */
void Chrono_Start(void)
{
	MyTimer_Start(Chrono_Timer);
}


/**
	* @brief  Arr�te le chronom�tre. 
  * @note   
	* @param  Aucun
  * @retval Aucun
  */
void Chrono_Stop(void)
{
	MyTimer_Stop(Chrono_Timer);
}


/**
	* @brief  Remet le chronom�tre � 0 
  * @note   
	* @param  Aucun
  * @retval Aucun
  */
void Chrono_Reset(void)
{
  // Arr�t Chrono
	MyTimer_Stop(Chrono_Timer);

	// Reset Time
	Chrono_Time.Hund=0;
	Chrono_Time.Sec=0;
	Chrono_Time.Min=0;
}


/**
	* @brief  Renvoie l'adresse de la variable Time priv�e g�r�e dans le module Chrono.c
  * @note   
	* @param  Aucun
  * @retval adresse de la variable Time
  */
Time * Chrono_Read(void)
{
	return &Chrono_Time;
}




/**
	* @brief  incr�mente la variable priv�e Chron_Time modulo 60mn 
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
		LL_GPIO_SetOutputPin(GPIOC, LL_GPIO_PIN_10);
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
 if((LL_GPIO_IsInputPinSet(GPIOC, LL_GPIO_PIN_8) == 1) && chr==0 ){
	 Chrono_Start();
	  chr=1;

 }else if ((LL_GPIO_IsInputPinSet(GPIOC, LL_GPIO_PIN_8) == 1)&&chr == 1){
	 Chrono_Stop();
	 chr=0;

 }
 
 if (LL_GPIO_IsInputPinSet(GPIOC, LL_GPIO_PIN_13) == 1){
	 Chrono_Reset();
	 Chrono_Stop();
}

}

char* format_msg(Time chrono){
	char tab[12];
	tab[0] = (chrono.Min % 10)+0x30;
	tab[1] = (chrono.Min - (chrono.Min % 10 )*10)+0x30;
	tab[2] = 0x3A; 
	tab[3] = (chrono.Sec % 10)+0x30 ;
	tab[4] = (chrono.Sec - (chrono.Sec % 10 )*10)+0x30;
	tab[5]= 0x3A;
	tab[6] = (chrono.Hund % 10)+0x30 ;
	tab[7] = (chrono.Hund - (chrono.Hund % 10 )*10)+0x30;
	tab[8] = 0x3A;
	tab[9] = 0x3C;
	tab[10] = 0x0D;
	tab[11] = 0x3C;
}

