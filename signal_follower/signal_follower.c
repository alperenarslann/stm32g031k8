
/*
 *  Created on: 21 december 2021
 *      Author: Alpeten
 *
 *      Signal follower with ADC and PWM
 * 
 * you can 'dız' but please change the code a little
 *
 *
 *
 */
#include "stm32g0xx.h"
#include "BSP.h"
#include "system_stm32g0xx.h"



/*PWM Timer */
void TIM2_IRQHandler(void) {
    // update duty (CCR1)
    TIM2->CCR2 = ADC1->DR;

    // Clear update status register
    TIM2->SR &= ~(1U << 0);
}



/* Function to initialize a pwm on PB3 (D13 pin)*/
void init_pwm(){
	 // Setup GPIO
	    //

	    // Enable GPIOB clock
	    RCC->IOPENR |= (1U << 1);
	    // Enable TIM2 clock
	    RCC->APBENR1 |= RCC_APBENR1_TIM2EN;

	    // Set alternate function to 2
	    // 3 comes from PB3
	    GPIOB->AFR[0] |= (2U << 4*3);
	    // Select AF from Moder
	    GPIOB->MODER &= ~(3U << 2*3);
	    GPIOB->MODER |= (2U << 2*3);

	    // zero out the control register just in case
	    TIM2->CR1 = 0;

	    // Select PWM Mode 1
	    TIM2->CCMR1 |= (6U << 12);
	    // Preload Enable
	    TIM2->CCMR1 |= TIM_CCMR1_OC2PE;

	    // Capture compare ch2 enable
	    TIM2->CCER |= TIM_CCER_CC2E;

	    // zero out counter
	    TIM2->CNT = 0;
	    // 40 Hz signal
	    TIM2->PSC = (100-1);
	    TIM2->ARR = (3000-1);

	    // zero out duty
	    TIM2->CCR2 = 0;

	    // Update interrupt enable
	    TIM2->DIER |= (1 << 0);

	    // TIM1 Enable
	    TIM2->CR1 |= TIM_CR1_CEN;

	    NVIC_SetPriority(TIM2_IRQn, 3);
	    NVIC_EnableIRQ(TIM2_IRQn);
}




/*ADC on PA1 (A1)*/
void init_ADC(){
	//set A1 as analog mode
	setMode('A',1,'A');

	//open ADC clock
	RCC->APBENR2 |= (1U << 20);

	//ADC struct is defined for common mode, we have to use ADC1 struct to change ADC register


//disable the ADC as initial just in case
 // this fields are probably zero as initial, it is not mandatory to assign them zero

	ADC1->CR &= ~(1U << 0); // disable the ADC enable bit ADEN
	ADC1->CR &= ~(1U << 1); // disable ADDIS
	ADC1->CR &= ~(1U << 2); // disable ADSTART
	ADC1->CR &= ~(1U << 4); // disable ADSTP
	ADC1->CR &= ~(1U << 31); // disable ADCAL

	//open ADC voltage regulator enable bit
	ADC1->CR |= (1U << 28);
	delay_ms(1); //1ms delay for wait to regulator to regulate the voltage (20 us should be fine, no need to 1ms)

	//do the ADC calibration ADC calibration
	ADC1->CR |= (1U << 31);
	while(ADC1->CR == (9U << 28));//wait until calibration is done
	ADC1->IER |= (1U << 11); //enable end of the calibration interrupt register (EOCALIE)



	//set the resolution
	ADC1->CFGR1 |= (1U << 3); //set the ADC resolution as 10 bits (max 1023)(CFGR->RES register (10))

	//configure to continuous mode
	ADC1->CFGR1 &= ~(1U << 16);//disable discontinuous mode if its opened before just in case
	ADC1->CFGR1 |= (1U << 13);

	//configure sampling rates
	ADC1->SMPR |= (5U << 0);// set the sampling rate 110 mode (sample per 79.5 clock cycles)
	ADC1->SMPR &= ~(1U << 8);//set smpsel1 register which we're just configured

	//Enable the channels
	ADC1->CHSELR |= (1U << 1);//select channel A1 to read

	ADC1->CR |= (1U << 0); // enable ADC
	while(((ADC1->CR)>>0)  == (1U));
	ADC1 -> CR |= (1U << 2);//ADSTAR1 for ADC

}







int main(void) {

	openClock('A');
	openClock('B');
	init_systick(SystemCoreClock/1000);
	//initilize a pwm to give input signal back
	init_pwm();

	//initialize the ADC
	init_ADC();


	while(1){


	}

	return 0;


}


