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

//extern volatile uint8_t enter_flag = 0; // check if enter (# button) is pressed


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








/*
void EXTI4_15IRQHandler(void){


}
*/



void configure_A0_int(){/*
	RCC-> APBENR2 |= (1U << 0); //enable SYSCFG clock
	EXTI-> EXTICR[0] |= (0U << 8*0); //chose port A (0. port) and 0th pin  (8*0)

	 EXTI->RTSR1 |= (1U << 0);//chose falling edge trigger at A0 (0th pin, so shift 0 bits to the left)

	    EXTI->IMR1 |= (1U << 0);  // Mask pin 0


	      NVIC_SetPriority(EXTI0_1_IRQn,1);
	      NVIC_EnableIRQ(EXTI0_1_IRQn);
*/
}


/*SYSTICK functions*/

void SysTick_Handler(void)
{
    if (tDelay != 0)
    {
        tDelay--;
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

/*SSD functions*/
void print_digit(volatile uint32_t dig)//SSD digit print function
{


	switch (dig){

	case 0:
		GPIOB->ODR |= (1U << 4);	//set  d12
		GPIOB->ODR |= (1U << 5);	//set  d11
		GPIOA->ODR |= (1U << 8);	//set  d9
		GPIOA->ODR |= (1U << 4);	//set  a2
		GPIOA->ODR |= (1U << 6);	//set  a6
		GPIOA->ODR |= (1U << 7);	//set  a7


		GPIOB->ODR &= ~(1U << 8);	//set  d8

		break;
	case 1:
		GPIOA->ODR |= (1U << 8);	//set  d9
		GPIOA->ODR |= (1U << 4);	//set  a2

		GPIOB->ODR &= ~(1U << 4);	//set  d12
		GPIOB->ODR &= ~(1U << 5);	//set  d11
		GPIOB->ODR &= ~(1U << 8);	//set  d8
		GPIOA->ODR &= ~(1U << 6);	//set  a6
		GPIOA->ODR &= ~(1U << 7);	//set  a7

		break;
	case 2:
		GPIOB->ODR |= (1U << 4);	//set  d12
		GPIOB->ODR |= (1U << 5);	//set  d11
		GPIOB->ODR |= (1U << 8);	//set  d8
		GPIOA->ODR |= (1U << 4);	//set  a2
		GPIOA->ODR |= (1U << 7);	//set  a7


		GPIOA->ODR &= ~(1U << 8);	//set  d9
		GPIOA->ODR &= ~(1U << 6);	//set  a6

		break;
	case 3:
		GPIOB->ODR |= (1U << 5);	//set  d11
		GPIOA->ODR |= (1U << 8);	//set  d9
		GPIOB->ODR |= (1U << 8);	//set  d8
		GPIOA->ODR |= (1U << 4);	//set  a2
		GPIOA->ODR |= (1U << 7);	//set  a7

		GPIOB->ODR &= ~(1U << 4);	//reset  d12
		GPIOA->ODR &= ~(1U << 6);	//reset  a6


		break;
	case 4:
		GPIOA->ODR |= (1U << 8);	//set  d9
		GPIOB->ODR |= (1U << 8);	//set  d8
		GPIOA->ODR |= (1U << 4);	//set  a2
		GPIOA->ODR |= (1U << 6);	//set  a6

		GPIOB->ODR &= ~(1U << 4);	//reset  d12
		GPIOB->ODR &= ~(1U << 5);	//reset  d11
		GPIOA->ODR &= ~(1U << 7);	//reset  a7

		break;
	case 5:
		GPIOB->ODR |= (1U << 5);	//set  d11
		GPIOA->ODR |= (1U << 8);	//set  d9
		GPIOB->ODR |= (1U << 8);	//set  d8
		GPIOA->ODR |= (1U << 6);	//set  a6
		GPIOA->ODR |= (1U << 7);	//set  a7

		GPIOB->ODR &= ~(1U << 4);	//reset  d12
		GPIOA->ODR &= ~(1U << 4);	//reset  a2


		break;
	case 6:
		GPIOB->ODR |= (1U << 4);	//set  d12
		GPIOB->ODR |= (1U << 5);	//set  d11
		GPIOA->ODR |= (1U << 8);	//set  d9
		GPIOB->ODR |= (1U << 8);	//set  d8
		GPIOA->ODR |= (1U << 6);	//set  a6
		GPIOA->ODR |= (1U << 7);	//set  a7

		GPIOA->ODR &= ~(1U << 4);	//reset  a2


		break;
	case 7:
		GPIOA->ODR |= (1U << 8);	//set  d9
		GPIOA->ODR |= (1U << 4);	//set  a2
		GPIOA->ODR |= (1U << 6);	//set  a6
		GPIOA->ODR |= (1U << 7);	//set  a7

		GPIOB->ODR &= ~(1U << 4);	//reset  d12
		GPIOB->ODR &= ~(1U << 5);	//reset  d11
		GPIOB->ODR &= ~(1U << 8);	//reset  d8

		break;
	case 8:
		GPIOB->ODR |= (1U << 4);	//set  d12
		GPIOB->ODR |= (1U << 5);	//set  d11
		GPIOA->ODR |= (1U << 8);	//set  d9
		GPIOB->ODR |= (1U << 8);	//set  d8
		GPIOA->ODR |= (1U << 4);	//set  a2
		GPIOA->ODR |= (1U << 6);	//set  a6
		GPIOA->ODR |= (1U << 7);	//set  a7

		break;
	case 9:
		GPIOB->ODR |= (1U << 5);	//set  d11
		GPIOA->ODR |= (1U << 8);	//set  d9
		GPIOB->ODR |= (1U << 8);	//set  d8
		GPIOA->ODR |= (1U << 4);	//set  a2
		GPIOA->ODR |= (1U << 6);	//set  a6
		GPIOA->ODR |= (1U << 7);	//set  a7

		GPIOB->ODR &= ~(1U << 4);	//reset  d12

		break;

	case 10: //letter for enter key (#)

		GPIOB->ODR |= (1U << 5);	//set  d11
		GPIOA->ODR |= (1U << 8);	//set  d9
		GPIOB->ODR |= (1U << 8);	//set  d8
		GPIOA->ODR |= (1U << 4);	//set  a2
		GPIOA->ODR |= (1U << 7);	//set  a

		GPIOA->ODR &= ~(1U << 6);	//set  a6
		GPIOB->ODR &= ~(1U << 4);	//reset  d12

		break;

	case 11: //letter A
		GPIOB->ODR |= (1U << 4);	//set  d12
		GPIOA->ODR |= (1U << 4);	//set  a2
		GPIOA->ODR |= (1U << 6);	//set  a6
		GPIOA->ODR |= (1U << 7);	//set  a7
		GPIOB->ODR |= (1U << 8);	//set  d8
		GPIOA->ODR |= (1U << 8);	//set  d9

		GPIOB->ODR &= ~(1U << 5);	//set  d11

			break;
	case 12://letter c



		GPIOA->ODR |= (1U << 7);	//set  a
		GPIOA->ODR &= ~(1U << 4);	//set  b
		GPIOA->ODR &= ~(1U << 8);	//set  c
		GPIOB->ODR |= (1U << 5);	//set  d
		GPIOB->ODR |= (1U << 4);	//set  e
		GPIOA->ODR |= (1U << 6);	//set  f
		GPIOB->ODR &= ~(1U << 8);	//set  g

		break;

	case 13: // letter E

			GPIOA->ODR |= (1U << 7);	//set  a
			GPIOA->ODR &= ~(1U << 4);	//set  b
			GPIOA->ODR &= ~(1U << 8);	//set  c
			GPIOB->ODR |= (1U << 5);	//set  d
			GPIOB->ODR |= (1U << 4);	//set  e
			GPIOA->ODR |= (1U << 6);	//set  f
			GPIOB->ODR |= (1U << 8);	//set  g

			break;

		case 14: //letter n

			GPIOA->ODR &= ~(1U << 7);	//set  a
			GPIOA->ODR &= ~(1U << 4);	//set  b
			GPIOA->ODR |= (1U << 8);	//set  c
			GPIOB->ODR &= ~(1U << 5);	//set  d
			GPIOB->ODR |= (1U << 4);	//set  e
			GPIOA->ODR &= ~(1U << 6);	//set  f
			GPIOB->ODR |= (1U << 8);	//set  g

			break;

		case 15: //letter t

			GPIOA->ODR &= ~(1U << 7);	//set  a
			GPIOA->ODR &= ~(1U << 4);	//set  b
			GPIOA->ODR &= ~(1U << 8);	//set  c
			GPIOB->ODR |= (1U << 5);	//set  d
			GPIOB->ODR |= (1U << 4);	//set  e
			GPIOA->ODR |= (1U << 6);	//set  f
			GPIOB->ODR |= (1U << 8);	//set  g

			break;

		case 16: //letter r

			GPIOA->ODR &= ~(1U << 7);	//set  a
			GPIOA->ODR &= ~(1U << 4);	//set  b
			GPIOA->ODR &= ~(1U << 8);	//set  c
			GPIOB->ODR &= ~(1U << 5);	//set  d
			GPIOB->ODR |= (1U << 4);	//set  e
			GPIOA->ODR &= ~(1U << 6);	//set  f
			GPIOB->ODR |= (1U << 8);	//set  g

			break;

		case 17: //letter H

			GPIOA->ODR &= ~(1U << 7);	//set  a
			GPIOA->ODR |= (1U << 4);	//set  b
			GPIOA->ODR |= (1U << 8);	//set  c
			GPIOB->ODR &= ~(1U << 5);	//set  d
			GPIOB->ODR |= (1U << 4);	//set  e
			GPIOA->ODR |= (1U << 6);	//set  f
			GPIOB->ODR |= (1U << 8);	//set  g

			break;

		case 18: // letter q

			GPIOA->ODR |= (1U << 7);	//set  a
			GPIOA->ODR |= (1U << 4);	//set  b
			GPIOA->ODR |= (1U << 8);	//set  c
			GPIOB->ODR &= ~(1U << 5);	//set  d
			GPIOB->ODR &= ~(1U << 4);	//set  e
			GPIOA->ODR |= (1U << 6);	//set  f
			GPIOB->ODR |= (1U << 8);	//set  g

			break;

		case 19://dot symbol

			GPIOA->ODR &= ~(1U << 7);	//set  a
			GPIOA->ODR &= ~(1U << 4);	//set  b
			GPIOA->ODR &= ~(1U << 8);	//set  c
			GPIOB->ODR |= (1U << 5);	//set  d
			GPIOB->ODR &= ~(1U << 4);	//set  e
			GPIOA->ODR &= ~(1U << 6);	//set  f
			GPIOB->ODR &= ~(1U << 8);	//set  g

			break;

		case 20: // letter o

			GPIOA->ODR &= ~(1U << 7);	//set  a
			GPIOA->ODR &= ~(1U << 4);	//set  b
			GPIOA->ODR |= (1U << 8);	//set  c
			GPIOB->ODR |= (1U << 5);	//set  d
			GPIOB->ODR |= (1U << 4);	//set  e
			GPIOA->ODR &= ~(1U << 6);	//set  f
			GPIOB->ODR |= (1U << 8);	//set  g
	}

}



void ssd_output(uint32_t x)//special function to print 4 digit numbers
{


		uint32_t dig1;
		uint32_t dig2;
		uint32_t dig3;
		uint32_t dig4;


		dig4 = x % 10U;
		x = x /10U;
		dig3 = x % 10U;
		x = x /10U;
		dig2 = x % 10U;
		x = x /10U;
		dig1 = x % 10U;

	//uint8_t flag = 0;
		int c = 160;

		while(c>0){

			print_digit(dig1);
			GPIOA->ODR &= ~(1U << 11);	//reset  a5
			delay_ms(2);
			GPIOA->ODR |= (1U << 11);	//set  a5

			print_digit(dig2);
			GPIOA->ODR &= ~(1U << 12);	//reset  a4
			delay_ms(2);
			GPIOA->ODR |= (1U << 12);	//set  a4

			print_digit(dig3);
			GPIOA->ODR &= ~(1U << 5);	//reset  a3
			delay_ms(2);
			GPIOA->ODR |= (1U << 5);	//set  a3


			print_digit(dig4);
			GPIOB->ODR &= ~(1U << 9);	//reset  d10
			delay_ms(2);
			GPIOB->ODR |= (1U << 9);	//set  d10

		c--;
		}

	}








/*keypad functions*/

void config_keypad_pins(){
	//assumed that clocks are already opened

	//rows
		setMode('B',6,'O');  //D1 -> row1
		setMode('B',7,'O');  //D0 -> row2
		setMode('A',15,'O'); //D2 -> row3
		setMode('B',1,'O');  //D3 -> row4

		//columns
		setMode('B',2,'I'); //D7 -> column1
		setMode('B',0,'I'); //D6 -> column2
		setMode('A',1,'I'); //A1 -> column3
		setMode('B',3,'I'); //D13 -> column4

		//set input pins pulldown mode for stability
		GPIOB->PUPDR |= (2U << 2*2);
		GPIOB->PUPDR |= (2U << 2*0);
		GPIOA->PUPDR |= (2U << 2*1);
		GPIOB->PUPDR |= (2U << 2*3);

		//set all rows high as initially
		GPIOB->ODR |= (1U << 6);
		GPIOB->ODR |= (1U << 7);
		GPIOA->ODR |= (1U << 15);
		GPIOB->ODR |= (1U << 1);

}

void config_keypad_IRQs(){
//config PA10,PA9,PB0,PA7;

	//RCC-> APBENR2 |= (1U << 0); //enable SYSCFG clock

	//PB2  0-1-2-3 mux
	EXTI->EXTICR[0] |= (1U << 8*2);
	EXTI->RTSR1 |= (1U << 2);
	EXTI->IMR1 |= (1U << 2);



	//PA1  0-1-2-3 mux
	EXTI->EXTICR[0] |= (0U << 8*1);
	EXTI->RTSR1 |= (1U << 1);
	EXTI->IMR1 |= (1U << 1);


	//PA0  0-1-2-3 mux
	EXTI->EXTICR[0] |= (1U << 8*0);
	EXTI->RTSR1 |= (1U << 0);
	EXTI->IMR1 |= (1U << 0);


	//PB3 0-1-2-3 mux
	EXTI->EXTICR[0] |= (1U << 8*3);
	EXTI->RTSR1 |= (1U << 3);
	EXTI->IMR1 |= (1U << 3);


    NVIC_SetPriority(EXTI0_1_IRQn,2);
    NVIC_EnableIRQ(EXTI0_1_IRQn);
	NVIC_SetPriority(EXTI2_3_IRQn,1);
	NVIC_EnableIRQ(EXTI2_3_IRQn);

    /*
    NVIC_SetPriority(EXTI4_15_IRQn,2);
    NVIC_EnableIRQ(EXTI4_15_IRQn);
*/
}


void clear_rows_keypad(){
	//set all rows low
	GPIOB->ODR &= ~(1U << 6);
	GPIOB->ODR &= ~(1U << 7);
	GPIOA->ODR &= ~(1U << 15);
	GPIOB->ODR &= ~(1U << 1);
}


void set_rows_keypad(){
	//set all rows high as initially
	GPIOB->ODR |= (1U << 6);
	GPIOB->ODR |= (1U << 7);
	GPIOA->ODR |= (1U << 15);
	GPIOB->ODR |= (1U << 1);
}

