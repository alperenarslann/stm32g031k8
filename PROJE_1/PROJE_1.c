
/*
 *  Created on: 28 Kas 2021
 *      Author: Alpeten
 */
#include "stm32g0xx.h"
#include "BSP.h"
#include "system_stm32g0xx.h"





void reset(){

	GPIOB->ODR &= ~(1U << 2);	//reset  d7
	GPIOB->ODR &= ~(1U << 8);	//reset  d8
	GPIOB->ODR &= ~(1U << 5);	//reset  d11
	GPIOB->ODR &= ~(1U << 4);	//reset  d12
	GPIOA->ODR &= ~(1U << 1);	//reset  a1
	GPIOA->ODR &= ~(1U << 11);	//reset  a5
	GPIOA->ODR &= ~(1U << 6);	//reset  a6
}









volatile uint32_t num = 2359;


int main(void) {


	init_systick(SystemCoreClock/1000);

/*open clocks*/
	openClock('A');
	openClock('B');

	/*configure 7 segment pins*/
	setMode('A',1,'O');
	setMode('A',4,'O');
	setMode('A',5,'O');
	setMode('A',12,'O');
	setMode('A',11,'O');
	setMode('A',6,'O');
	setMode('B',8,'O');
	setMode('A',8,'O');
	setMode('B',9,'O');
	setMode('B',5,'O');
	setMode('B',4,'O');
	setMode('B',2,'O');


	//set ssd digits low as initial to open all digits
	GPIOA->ODR &= ~(1U << 12);	//set  a4 to 1
	GPIOA->ODR &= ~(1U << 5);	//set  a3 to 1
	GPIOA->ODR &= ~(1U << 4);	//set  a2 to 1
	GPIOB->ODR &= ~(1U << 9);	//set  d10 to 1



	/*configure keypad*/
	//rows are output, columns are input
	config_keypad_pins();//configure the pins
	config_keypad_IRQs();//configure the interrupts

	//deneysel
	/*
	setMode('A',7,'O');
	setMode('B',3,'I');
	GPIOA->PUPDR |= (2U << 2*3);





	EXTI->EXTICR[0] |= (1U << 8*3);
	EXTI->RTSR1 |= (1U << 3);
	EXTI->IMR1 |= (1U << 3);

	NVIC_SetPriority(EXTI2_3_IRQn,1);
	NVIC_EnableIRQ(EXTI2_3_IRQn);
*/



while(1){
/*
	if((GPIOA->IDR >> 8) & 1 ){
		GPIOA->ODR |= (1U << 7);
	}
	else{
		GPIOA->ODR &= ~(1U << 7);
	}
*/

	GPIOA->ODR &= ~(1U << 7);

}


return 0;

}


