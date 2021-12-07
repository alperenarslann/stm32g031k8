#include "stm32g0xx.h"

#define LEDDELAY 160000



volatile int button_counter=0;




void init_clocks(){
	RCC->IOPENR |= 7U; //activate A,B and C ports
	RCC-> IOPENR |= (1U << 0);
}


void set_A6_PIN_output(){
	 GPIOA->MODER &= ~(3U << 2*6);
	 GPIOA->MODER |= (1U << 2*6);
}




void set_output(int port,int pin){
	switch (port){
	case 1:
		 GPIOA->MODER &= ~(3U << 2*pin);
		 GPIOA->MODER |= (1U << 2*pin);
	case 2:
		 GPIOB->MODER &= ~(3U << 2*pin);
		 GPIOB->MODER |= (1U << 2*pin);
	}
}




void set_input(int port,int pin){
	switch (port){
	case 1:
		GPIOA->MODER &= ~(3U << 2*pin);
	case 2:
		GPIOB->MODER &= ~(3U << 2*pin);
	}
}


void turn_on_ext_led(int port,int pin){
	switch (port){
	case 1:
		GPIOA->ODR |= (1U << pin);
	case 2:
		GPIOB->ODR |= (1U << pin);
	}
}




void turn_off_ext_led(int port,int pin){
	switch (port){
	case 1:
	 GPIOA->ODR |= ~(1U << pin);
	case 2:
	 GPIOB->ODR |= ~(1U << pin);
	}
}




void delay(int time){
	for(;time>0;time--);
}



void check_mode(int mode){ //check button pressed and call necessary mode

	switch(mode){
	case 0:
		GPIOA->ODR &= ~(1U << 6); //turn off the led
		break;
	case 1:
		init_tim1(1000); //1 sec interval mode1
		break;
	case 2:
		init_tim1(500); //0.5 sec interval mode2
		break;
	case 3:
		init_tim1(100); //0.1 sec interval mode3
		break;
	case 4:
		GPIOA->ODR |= (1U << 6); //turn on the led mode 4
		break;
	}
}


int check_button_presses(){ // check how many times button pressed

	if (GPIOB->IDR &= (1U << 2)){
		button_counter++;
	}

	volatile int button_mod4 = button_counter % 5; //get the

	switch(button_mod4){
	case 0:
		return 0;
		break;
	case 1:
		return 1;
		break;
	case 2:
		return 2;
		break;
	case 3:
		return 3;
		break;
	case 4:
		return 4;
		break;
}
return 0;
}


void init_tim1(volatile uint32_t prescaler){
	RCC->APBENR2 = (1U << 11); //activate clock
	TIM1->CR1 = 0; //reset
	TIM1->CR1 |= (1 << 7);
	TIM1->CNT =0;
	TIM1->PSC = prescaler - 1; //set prescaler to increment

	TIM1->ARR = 16000; // set 16000 so we get .1, .5, 1..

	TIM1->DIER |= (1 << 0);
	TIM1->CR1 |= (1 << 0);
	NVIC_SetPriority(TIM1_BRK_UP_TRG_COM_IRQn,1); //set lower

	NVIC_EnableIRQ(TIM1_BRK_UP_TRG_COM_IRQn); //enable NVIC
}



void TIM1_BRK_UP_TRG_COM_IRQHandler(){ //low priority TIM1

	GPIOA->ODR ^= (1U << 6); //toggle led with using xor
	TIM1->SR &= ~(1U << 0); //reset
}



void EXTI2_3_IRQHandler(){ //high priority external interrupt

volatile int a = check_button_presses();
	check_mode(a);
	EXTI->RPR1 |= (1U << 0); //reset
}


void external_buttonHandler(){
	EXTI->RTSR1 |= (1U << 2); //set RRSR1's 9th bit to 1 for PB2
	EXTI->EXTICR[0] |= (1U << 8*2);

	EXTI->IMR1 |= (1U << 2);// set IMR1's 2nd to 1 for PB2
	NVIC_SetPriority(EXTI2_3_IRQn, 0); //it has the second

	 NVIC_EnableIRQ(EXTI2_3_IRQn);//enable NVIC
}




int main() {
	init_clocks();
	set_A6_PIN_output();

	set_input(2,2); //set PB2 pin as input for button
	external_buttonHandler(); //call handler
	 return 0;
}


