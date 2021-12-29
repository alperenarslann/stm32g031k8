
/*
 * there is no timer tbh.
 *  Created on: 28 Kas 2021
 *      Author: Alpeten
 */
#include "stm32g0xx.h"
#include "BSP.h"
#include "system_stm32g0xx.h"



volatile uint32_t ddelay;

void reset(){

	GPIOB->ODR &= ~(1U << 2);	//reset  d7
	GPIOB->ODR &= ~(1U << 8);	//reset  d8
	GPIOB->ODR &= ~(1U << 5);	//reset  d11
	GPIOB->ODR &= ~(1U << 4);	//reset  d12
	GPIOA->ODR &= ~(1U << 1);	//reset  a1
	GPIOA->ODR &= ~(1U << 11);	//reset  a5
	GPIOA->ODR &= ~(1U << 6);	//reset  a6
}


void print(uint32_t dig){

	switch (dig){

	case 0:
		GPIOB->ODR |= (1U << 2);	//set  d7
		GPIOB->ODR |= (1U << 8);	//set  d8
		GPIOB->ODR |= (1U << 5);	//set  d11
		GPIOA->ODR |= (1U << 1);	//set  a1
		GPIOA->ODR |= (1U << 11);	//set  a5
		GPIOA->ODR |= (1U << 6);	//set  a6

		GPIOB->ODR &= ~(1U << 4);	//set  d12

		break;
	case 1:
		GPIOB->ODR |= (1U << 5);	//set  d11
		GPIOA->ODR |= (1U << 1);	//set  a1

		GPIOB->ODR &= ~(1U << 2);	//set  d7
		GPIOB->ODR &= ~(1U << 8);	//set  d8
		GPIOA->ODR &= ~(1U << 11);	//set  a5
		GPIOA->ODR &= ~(1U << 6);	//set  a6
		GPIOB->ODR &= ~(1U << 4);	//set  d12



		break;
	case 2:
		GPIOB->ODR |= (1U << 2);	//set  d7
		GPIOB->ODR |= (1U << 8);	//set  d8
		GPIOB->ODR |= (1U << 4);	//set  d12
		GPIOA->ODR |= (1U << 1);	//set  a1
		GPIOA->ODR |= (1U << 6);	//set  a6



		GPIOB->ODR &= ~(1U << 5);	//set  d11
		GPIOA->ODR &= ~(1U << 11);	//set  a5

		break;
	case 3:
		GPIOB->ODR |= (1U << 8);	//set  d8
		GPIOB->ODR |= (1U << 5);	//set  d11
		GPIOB->ODR |= (1U << 4);	//set  d12
		GPIOA->ODR |= (1U << 1);	//set  a1
		GPIOA->ODR |= (1U << 6);	//set  a6




		GPIOB->ODR &= ~(1U << 2);	//set  d7
		GPIOA->ODR &= ~(1U << 11);	//set  a5

		break;
	case 4:
		GPIOB->ODR |= (1U << 5);	//set  d11
		GPIOB->ODR |= (1U << 4);	//set  d12
		GPIOA->ODR |= (1U << 1);	//set  a1
		GPIOA->ODR |= (1U << 11);	//set  a5




		GPIOB->ODR &= ~(1U << 2);	//set  d7
		GPIOB->ODR &= ~(1U << 8);	//set  d8
		GPIOA->ODR &= ~(1U << 6);	//set  a6

		break;
	case 5:
		GPIOB->ODR |= (1U << 8);	//set  d8
		GPIOB->ODR |= (1U << 5);	//set  d11
		GPIOB->ODR |= (1U << 4);	//set  d12
		GPIOA->ODR |= (1U << 11);	//set  a5
		GPIOA->ODR |= (1U << 6);	//set  a6




		GPIOA->ODR &= ~(1U << 1);	//set  a1
		GPIOB->ODR &= ~(1U << 2);	//set  d7

		break;
	case 6:
		GPIOB->ODR |= (1U << 2);	//set  d7
		GPIOB->ODR |= (1U << 8);	//set  d8
		GPIOB->ODR |= (1U << 5);	//set  d11
		GPIOB->ODR |= (1U << 4);	//set  d12
		GPIOA->ODR |= (1U << 11);	//set  a5
		GPIOA->ODR |= (1U << 6);	//set  a6




		GPIOA->ODR &= ~(1U << 1);	//set  a1


		break;
	case 7:
		GPIOB->ODR |= (1U << 5);	//set  d11
		GPIOA->ODR |= (1U << 1);	//set  a1
		GPIOA->ODR |= (1U << 11);	//set  a5
		GPIOA->ODR |= (1U << 6);	//set  a6





		GPIOB->ODR &= ~(1U << 2);	//set  d7
		GPIOB->ODR &= ~(1U << 8);	//set  d8
		GPIOB->ODR &= ~(1U << 4);	//set  d12

		break;
	case 8:
		GPIOB->ODR |= (1U << 2);	//set  d7
		GPIOB->ODR |= (1U << 8);	//set  d8
		GPIOB->ODR |= (1U << 5);	//set  d11
		GPIOA->ODR |= (1U << 1);	//set  a1
		GPIOA->ODR |= (1U << 11);	//set  a5
		GPIOA->ODR |= (1U << 6);	//set  a6
		GPIOB->ODR |= (1U << 4);	//set  d12


		break;
	case 9:
		GPIOB->ODR |= (1U << 8);	//set  d8
		GPIOB->ODR |= (1U << 5);	//set  d11
		GPIOA->ODR |= (1U << 1);	//set  a1
		GPIOA->ODR |= (1U << 11);	//set  a5
		GPIOA->ODR |= (1U << 6);	//set  a6
		GPIOB->ODR |= (1U << 4);	//set  d12




		GPIOB->ODR &= ~(1U << 2);	//set  d7


		break;

	}

}




void ssd_output(uint32_t x){



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

flag = 0;
while(ddelay > 0){

	print(dig1);
	GPIOA->ODR &= ~(1U << 12);	//set  a4 to 0
	delay(160);
	GPIOA->ODR |= (1U << 12);	//set  a4 to 1
	//reset();

	print(dig2);
	GPIOA->ODR &= ~(1U << 5);	//set  a3 to 0
	delay(160);
	GPIOA->ODR |= (1U << 5);	//set  a3 to 1
	//reset();

	print(dig3);
	GPIOA->ODR &= ~(1U << 4);	//set  a2 to 0
	delay(160);
	GPIOA->ODR |= (1U << 4);	//set  a2 to 1
//	reset();

	print(dig4);
	GPIOB->ODR &= ~(1U << 9);	//set  d10 to 0
	delay(160);
	GPIOB->ODR |= (1U << 9);	//set  d10 to 1
	//reset();


ddelay = ddelay - 1;

	//i = i-1;
}




}


volatile uint32_t num = 2359;


int main(void) {

	init_systick(SystemCoreClock/1000);
	/*
	 //problem 2 kodu
	openClock('A');
	setMode('A',6U,'O');

	 init_systick(SystemCoreClock/1000);

	while(1){
		delay_ms(1000);
		GPIOA-> ODR ^= (1U << 6);
	}
	return 0;

	*/

openClock('A');
openClock('B');

setMode('A',0,'I');
configure_A0_int();

setMode('A',1,'O');
setMode('A',4,'O');
setMode('A',5,'O');
setMode('A',12,'O');
setMode('A',11,'O');
setMode('A',6,'O');

setMode('B',2,'O');
setMode('B',8,'O');
setMode('A',8,'O');
setMode('B',9,'O');
setMode('B',5,'O');
setMode('B',4,'O');


//denemelik
setMode('A',7,'O');
state = mod1;





GPIOA->ODR &= ~(1U << 12);	//set  a4 to 1
GPIOA->ODR &= ~(1U << 5);	//set  a3 to 1
GPIOA->ODR &= ~(1U << 4);	//set  a2 to 1
GPIOB->ODR &= ~(1U << 9);	//set  d10 to 1


while(1){
	ssd_output(num);

	ddelay = 1600;

	switch(state){
	case mod0:
		ddelay = 1600;
		break;
	case mod1:
		ddelay = 2000;
		break;
	case mod2:
		ddelay = 800;
		break;
	case mod3:
		ddelay = 160;
		break;
	case mod4:
		ddelay = 16;
		break;
	case mod5:
		ddelay = 8;
		break;
	}

	if(((num / 10) % 10) > 5){
		num=num - 40;
	}

	else if(num == 0){
		num = 2359;
	}

	num--;

	GPIOA-> ODR  &= ~(1U << 7);

}




}


