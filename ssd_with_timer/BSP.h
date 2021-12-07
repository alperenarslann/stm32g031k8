/*
 * BSP.h
 *
 *  Created on: 28 Kas 2021
 *      Author: Alpeten
 */
#include "stm32g0xx.h"
//#include "system_stm32g0xx.h"

#ifndef BSP_H_
#define BSP_H_

#define LEDDELAY 1600000

#define KEY1   0x45670123
#define KEY2   0xCDEF89AB


#if !defined  (HSE_VALUE)
#define HSE_VALUE    (8000000UL)    /*!< Value of the External oscillator in Hz */
#endif /* HSE_VALUE */

#if !defined  (HSI_VALUE)
  #define HSI_VALUE  (16000000UL)   /*!< Value of the Internal oscillator in Hz*/
#endif /* HSI_VALUE */

#define PLL_M 	1U
#define PLL_N 	8U
#define PLL_P 	7U
#define PLL_Q 	2U
#define PLL_L 	2U




extern uint32_t SystemCoreClock;

enum mode{
	mod0=0,mod1=1,mod2=2,mod3=3,mod4=4,mod5=5
};





/*COOL FUNCTIONS*/
void openClock(char port);
void setMode(char port, uint32_t num,char IO);

/*onboard led functions*/
void configureOnboardLed();
void toggleOnboardLed();
void turnOnOnboardLed();
void turnOffOnboardLed();

/*onboard button functions*/
void unlockFlash();
void lockFlash();
void configureOnboardButton();
int readOnboardButton();


/*clock configure functions*/
void set_sysclk_to_hse();
void set_sysclk_to_hsi();
void set_sysclk_to_84();


/*INTERRUPTS*/
void EXTI_A0_IRQHandler();
void configure_A0_int();

/*SYSTICK functions*/
void SysTick_Handler();
void init_systick(uint32_t s);


void delay(volatile uint32_t s);
void delay_ms(uint32_t s);



/*SSD functions*/
void print_digit(volatile uint32_t dig);



#endif /* BSP_H_ */
