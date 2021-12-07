/*
 * BSP.c
 *
 *  Created on: 28 Kas 2021
 *      Author: Alpeten
 */

#include "BSP.h"
#include "stm32g0xx.h"
#include "system_stm32g0xx.h"

static uint32_t tDelay;
extern uint32_t SystemCoreClock;


/*delay function*/
void delay(volatile uint32_t s){
	for(; s>0; s--);
}

/*COOL FUNCTIONS*/
void openClock(char port){

	switch(port){
	case 'A':
		RCC-> IOPENR |= (1U << 0);
		break;

	case 'B':
		RCC->IOPENR |= (1U << 1);
		break;

	case 'C':
		RCC->IOPENR |= (1U << 2);
		break;

	case 'D':
		RCC->IOPENR |= (1U << 3);
		break;

	case 'F':
		RCC->IOPENR |= (1U << 5);
		break;

	}
}

void setMode(char port, uint32_t num, char IO){

	switch(port){
	case 'A':

		if(num == 2 || num == 3){//dont touch PA2 and PA3 ports even user want to change them
		break;
		}


		GPIOA-> MODER &= ~(3U << num*2); // set 0 both bytes (input mode)

		if(IO == 'O'){//output mode
			GPIOA-> MODER |= (1U << num*2);
		}

		else if(IO == 'I'){
			//do nothing
		}

		else if(IO == 'A'){//analog input mode
			GPIOA-> MODER |= (3U << num*2);
		}

		else if(IO == 'F'){//alternate function mode
			GPIOA -> MODER |= (2U << (num*2));

		}

		break;

	case 'B':

		GPIOB-> MODER &= ~(3U << num*2); // set 0 both bytes (input mode)

		if(IO == 'O'){//output mode
			GPIOB-> MODER |= (1U << num*2);
		}

		else if(IO == 'I'){
			//do nothing
		}

		else if(IO == 'A'){//analog input mode
			GPIOB-> MODER |= (3U << num*2);
		}

		else if(IO == 'F'){//alternate function mode
			GPIOB -> MODER |= (2U << (num*2));

		}

		break;

	case 'C':
		GPIOC-> MODER &= ~(3U << num*2); // set 0 both bytes (input mode)

		if(IO == 'O'){//output mode
			GPIOC-> MODER |= (1U << num*2);
		}

		else if(IO == 'I'){
			//do nothing
		}

		else if(IO == 'A'){//analog input mode
			GPIOC-> MODER |= (3U << num*2);
		}

		else if(IO == 'F'){//alternate function mode
			GPIOC -> MODER |= (2U << (num*2));

		}

		break;

	case 'D':
		GPIOD-> MODER &= ~(3U << num*2); // set 0 both bytes (input mode)

		if(IO == 'O'){//output mode
			GPIOD-> MODER |= (1U << num*2);
		}

		else if(IO == 'I'){
			//do nothing
		}

		else if(IO == 'A'){//analog input mode
			GPIOD-> MODER |= (3U << num*2);
		}

		else if(IO == 'F'){//alternate function mode
			GPIOD -> MODER |= (2U << (num*2));

		}

		break;

	case 'F':
		GPIOF-> MODER &= ~(3U << num*2); // set 0 both bytes (input mode)

		if(IO == 'O'){//output mode
			GPIOF-> MODER |= (1U << num*2);
		}

		else if(IO == 'I'){
			//do nothing
		}

		else if(IO == 'A'){//analog input mode
			GPIOF-> MODER |= (3U << num*2);
		}

		else if(IO == 'F'){//alternate function mode
			GPIOF -> MODER |= (2U << (num*2));

		}

		break;

	}


}


/*onboard led functions*/
void configureOnboardLed(){

	RCC->IOPENR |= (1U << 2);

	/* Setup PC6 as output */
	GPIOC->MODER &= ~(3U << 2*6);
	GPIOC->MODER |= (1U << 2*6);

}

void toggleOnboardLed(){


	    /* Turn on LED */
	    GPIOC->ODR |= (1U << 6);

	    while(1) {
	        delay(LEDDELAY);
	        /* Toggle LED */
	        GPIOC->ODR ^= (1U << 6);
	    }
}

void turnOnOnboardLed(){

		    /* Turn on LED */
		    GPIOC->ODR |= (1U << 6);


}

void turnOffOnboardLed(){

			    /* Turn off LED */
			  		    GPIOC->ODR &= ~(1U << 6);
}



/*onboard Button Functions*/

void unlockFlash() {
    if (FLASH->CR & FLASH_CR_LOCK) {
        FLASH->KEYR = KEY1;
        FLASH->KEYR = KEY2;
    }
}

void lockFlash() {
    FLASH->CR |= FLASH_CR_LOCK; // bit 31
}

void configureOnboardButton(){

	/*activate clock for the port f*/
	RCC-> IOPENR |= (1U << 5);


	/*enable change the optr by clearing the lock bit*/
	unlockFlash();


	/*change button mode reset to GPIO*/
	FLASH -> OPTR &= ~(3U << 27);
	FLASH -> OPTR |= (1U << 27);

	/*setup PF2 as input*/
	GPIOF -> MODER &= ~(3U << 2*2);
	//GPIOF->MODER |= (1U << 2*2);
	//GPIOF-> ODR |= (1U << 2);

}

int readOnboardButton(){	//torigari cindari

	if(((GPIOF -> IDR)) & 4U){
		return 1;//if the onboard led is pressed, return 1
	}

	return 0;


}



/*processor clock functions*/

void set_sysclk_to_hse(){

	SystemInit();
	//enable HSE
	RCC->CR |= (1 << 16);
	//wait till HSE is ready
	 while(!(RCC->CR & (1 << 17)));

	 /*configure flash*/
	 FLASH->ACR = (1 << 8) | (1 << 9) | (1 << 10 ) | (0 << 0);


	 //select HSE as system clock
	RCC->CFGR &= ~(3U << 0);
	RCC->CFGR |=  (1 << 0);

     //wait till the PPL used as system clock
	while (!(RCC->CFGR & (1 << 2)));

    SystemCoreClock = HSE_VALUE;



}

void set_sysclk_to_hsi(){

	 /* Reset goes to HSI by default */
	    SystemInit();

	    /* Configure Flash
	     * prefetch enable (ACR:bit 8)
	     * instruction cache enable (ACR:bit 9)
	     * data cache enable (ACR:bit 10)
	     * set latency to 0 wait states (ARC:bits 2:0)
	     *   see Table 10 on page 80 in RM0090
	     */
	    FLASH->ACR = (1 << 8) | (1 << 9) | (1 << 10 ) | (0 << 0);

	    SystemCoreClock = HSI_VALUE;

}


void set_sysclk_to_64(){ 	//torigari cindari???

	SystemInit();

	    #undef PLL_P
	    uint32_t PLL_P = 4;

	    /* Enable HSE (CR: bit 16) */
	    RCC->CR |= (1 << 16);
	    /* Wait till HSE is ready (CR: bit 17) */
	    while(!(RCC->CR & (1 << 17)));

	    /* set voltage scale to 1 for max frequency */
	    /* first enable power interface clock (APB1ENR:bit 28) */
	    RCC->APBENR1 |= (1 << 28);

	    /* then set voltage scale to 1 for max frequency (PWR_CR:bit 14)
	     * (0) scale 2 for fCLK <= 144 Mhz
	     * (1) scale 1 for 144 Mhz < fCLK <= 168 Mhz
	     */
	    PWR->CR1 |= (1 << 14);

	    /* set AHB prescaler to /1 (CFGR:bits 7:4) */
	    RCC->CFGR |= (0 << 4);
	    /* set ABP low speed prescaler to /4 (APB1) (CFGR:bits 12:10) */
	    RCC->CFGR |= (5 << 10);
	    /* set ABP high speed prescaper to /2 (ABP2) (CFGR:bits 15:13) */
	    RCC->CFGR |= (4 << 13);

	    /* Set M, N, P and Q PLL dividers
	     * PLLCFGR: bits 5:0 (M), 14:6 (N), 17:16 (P), 27:24 (Q)
	     * Set PLL source to HSE, PLLCFGR: bit 22, 1:HSE, 0:HSI
	     */
	    RCC->PLLCFGR = PLL_M | (PLL_N << 6) | (((PLL_P >> 1) -1) << 16) |
	                   (PLL_Q << 24) | (1 << 22);
	    /* Enable the main PLL (CR: bit 24) */
	    RCC->CR |= (1 << 24);
	    /* Wait till the main PLL is ready (CR: bit 25) */
	    while(!(RCC->CR & (1 << 25)));
	    /* Configure Flash
	     * prefetch enable (ACR:bit 8)
	     * instruction cache enable (ACR:bit 9)
	     * data cache enable (ACR:bit 10)
	     * set latency to 2 wait states (ARC:bits 2:0)
	     *   see Table 10 on page 80 in RM0090
	     */
	    FLASH->ACR = (1 << 8) | (1 << 9) | (1 << 10 ) | (2 << 0);

	    /* Select the main PLL as system clock source, (CFGR:bits 1:0)
	     * 0b00 - HSI
	     * 0b01 - HSE
	     * 0b10 - PLL
	     */
	    RCC->CFGR &= ~(3U << 0);
	    RCC->CFGR |=  (2  << 0);
	    /* Wait till the main PLL is used as system clock source (CFGR:bits 3:2) */
	    while (!(RCC->CFGR & (2 << 2)));

	    SystemCoreClock = 64000000;

}

/*Interrupts*/
void EXTI0_1_IRQHandler(void){

		GPIOA-> ODR |= (1U << 7);//open the led on the PA0

		state++;

		if(state > 5){
			state = 0;
		}
		//delay(LEDDELAY);
	/*

	if(GPIOA->IDR << 15 == (1<<15)){

	delay(16000);

	}
*/

	EXTI-> RPR1 |= (1 << 0); //clear pending bit
}

void configure_A0_int(){
	RCC-> APBENR2 |= (1U << 0); //enable SYSCFG clock
	EXTI-> EXTICR[0] |= (0U << 8*0); //chose port A (0. port) and 0th pin  (8*0)

	 EXTI->RTSR1 |= (1U << 0);//chose falling edge trigger at A0 (0th pin, so shift 0 bits to the left)

	    EXTI->IMR1 |= (1U << 0);  // Mask pin 0


	      NVIC_SetPriority(EXTI0_1_IRQn,1);
	      NVIC_EnableIRQ(EXTI0_1_IRQn);

}


/*SYSTICK functions*/

void SysTick_Handler(void)
{
    if (tDelay != 0)
    {
        tDelay--;
    }

    if(tDelay == 0){
     	flag = 1;
     }

}

void init_systick(uint32_t s){

    // Clear CTRL register
    SysTick->CTRL = 0x00000;
    // Main clock source is running with HSI by default which is at 8 Mhz.
    // SysTick clock source can be set with CTRL register (Bit 2)
    // 0: Processor clock/8 (AHB/8)
    // 1: Processor clock (AHB)
    SysTick->CTRL |= (1 << 2);
    // Enable callback (bit 1)
    SysTick->CTRL |= (1 << 1);
    // Load the value
    SysTick->LOAD = (uint32_t)(s-1);
    // Set the current value to 0
    SysTick->VAL = 0;
    // Enable SysTick (bit 0)
    SysTick->CTRL |= (1 << 0);
}





void delay_ms(uint32_t s)
{
    tDelay = s;
    while(tDelay != 0);


}



