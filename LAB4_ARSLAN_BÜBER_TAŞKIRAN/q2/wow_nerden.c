


























/*
 *  Created on: 28 Kas 2021
 *      Author: Alpeten
 */
#include "stm32g0xx.h"
#include "BSP.h"
#include "system_stm32g0xx.h"


int main(void) {

	 //problem 2 kodu
	openClock('A');
	setMode('A',6U,'O');

	 init_systick(SystemCoreClock/1000);

	while(1){
		delay_ms(1000);
		GPIOA-> ODR ^= (1U << 6);
	}
	return 0;




}


