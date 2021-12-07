#include "stm32g0xx.h"

void clearSSD		( void );
void setSSD			( int );

void clearRowsKeypad( void );
void setRowsKeypad  ( void );

void EXTI0_1_IRQHandler(void){ // interrupt from PB0
	clearRowsKeypad();

	GPIOB->ODR ^= (1U << 9); // PB9
	if ((GPIOB->IDR >> 0) & 1){
		//9
		setSSD(9);
	}
	GPIOB->ODR ^= (1U << 9); // PB9

	GPIOB->ODR ^= (1U << 5); // PB5
	if ((GPIOB->IDR >> 0) & 1){
		//6
		setSSD(6);
	}
	GPIOB->ODR ^= (1U << 5); // PB5

	GPIOB->ODR ^= (1U << 4); // PB4
	if ((GPIOB->IDR >> 0) & 1){
		//3
		setSSD(3);
	}
	GPIOB->ODR ^= (1U << 4); // PB4

	EXTI->RPR1 |= (1U << 0); // Clear interrupt flag
	setRowsKeypad();
}

void EXTI2_3_IRQHandler(void){ // interrupt from PB2
	clearRowsKeypad();

	GPIOA->ODR ^= (1U << 8); // PA8
		if ((GPIOB->IDR >> 2) & 1){
		//0
		setSSD(0);			//
			}
		GPIOA->ODR ^= (1U << 8); // PA8

		GPIOB->ODR ^= (1U << 9); // PB9
		if ((GPIOB->IDR >> 2) & 1){
			//8
			setSSD(8);
		}
		GPIOB->ODR ^= (1U << 9); // PB9

		GPIOB->ODR ^= (1U << 5); // PB5
		if ((GPIOB->IDR >> 2) & 1){
			//5
			setSSD(5);
		}
		GPIOB->ODR ^= (1U << 5); // PB5

		GPIOB->ODR ^= (1U << 4); // PB4
		if ((GPIOB->IDR >> 2) & 1){
			//2
			setSSD(2);
		}
		GPIOB->ODR ^= (1U << 4); // PB4

		EXTI->RPR1 |= (1U << 2); // Clear interrupt flag
	setRowsKeypad();
}

void EXTI4_15_IRQHandler(void){

	clearRowsKeypad();

		GPIOB->ODR ^= (1U << 9); // PB9
		if ((GPIOB->IDR >> 8) & 1){
			//7
			setSSD(7);
		}
		GPIOB->ODR ^= (1U << 9); // PB9

		GPIOB->ODR ^= (1U << 5); // PB5
		if ((GPIOB->IDR >> 8) & 1){
			//4
			setSSD(4);
		}
		GPIOB->ODR ^= (1U << 5); // PB5

		GPIOB->ODR ^= (1U << 4); // PB4
		if ((GPIOB->IDR >> 8) & 1){
			//1
			setSSD(1);
		}
		GPIOB->ODR ^= (1U << 4); // PB4


	EXTI->RPR1 |= (1U << 8); // Clear interrupt flag
		setRowsKeypad();
   }





void clearSSD(void){
	/* Set all outputs connected to SSD (clear SSD) */
	GPIOA->ODR |= (1U << 0);   // PA0  A
	GPIOA->ODR |= (1U << 1);   // PA1  B
	GPIOA->ODR |= (1U << 4);   // PA4  C
	GPIOA->ODR |= (1U << 5);   // PA5  D
	GPIOA->ODR |= (1U << 12);  // PA12 E
	GPIOA->ODR |= (1U << 11);  // PA11 F
	GPIOA->ODR |= (1U << 6);   // PA6  G
}

void setSSD( int x ) {
	clearSSD();
	switch ( x )
	{

	case 0:
		GPIOA->ODR &= ~(1U << 0);   // PA0  A
		GPIOA->ODR &= ~(1U << 1);   // PA1  B
		GPIOA->ODR &= ~(1U << 4);   // PA4  C
		GPIOA->ODR &= ~(1U << 5);   // PA5  D
		GPIOA->ODR &= ~(1U << 12);  // PA12 E
		GPIOA->ODR &= ~(1U << 11);  // PA11 F
		break;

	case 1:
		GPIOA->ODR &= ~(1U << 1);   // PA1  B
		GPIOA->ODR &= ~(1U << 4);   // PA4  C
		break;

	case 2:
		GPIOA->ODR &= ~(1U << 0);   // PA0  A
		GPIOA->ODR &= ~(1U << 1);   // PA1  B
		GPIOA->ODR &= ~(1U << 5);   // PA5  D
		GPIOA->ODR &= ~(1U << 12);  // PA12 E
		GPIOA->ODR &= ~(1U << 6);   // PA6  G
		break;

	case 3:
		GPIOA->ODR &= ~(1U << 0);   // PA0  A
		GPIOA->ODR &= ~(1U << 1);   // PA1  B
		GPIOA->ODR &= ~(1U << 4);   // PA4  C
		GPIOA->ODR &= ~(1U << 5);   // PA5  D
		GPIOA->ODR &= ~(1U << 6);   // PA6  G
		break;

	case 4:
		GPIOA->ODR &= ~(1U << 1);   // PA1  B
		GPIOA->ODR &= ~(1U << 4);   // PA4  C
		GPIOA->ODR &= ~(1U << 11);  // PA11 F
		GPIOA->ODR &= ~(1U << 6);   // PA6  G
		break;

	case 5:
		GPIOA->ODR &= ~(1U << 0);   // PA0  A
		GPIOA->ODR &= ~(1U << 4);   // PA4  C
		GPIOA->ODR &= ~(1U << 5);   // PA5  D
		GPIOA->ODR &= ~(1U << 11);  // PA11 F
		GPIOA->ODR &= ~(1U << 6);   // PA6  G
		break;

	case 6:
		GPIOA->ODR &= ~(1U << 0);   // PA0  A
		GPIOA->ODR &= ~(1U << 4);   // PA4  C
		GPIOA->ODR &= ~(1U << 5);   // PA5  D
		GPIOA->ODR &= ~(1U << 12);  // PA12 E
		GPIOA->ODR &= ~(1U << 11);  // PA11 F
		GPIOA->ODR &= ~(1U << 6);   // PA6  G
		break;

	case 7:
		GPIOA->ODR &= ~(1U << 0);   // PA0  A
		GPIOA->ODR &= ~(1U << 1);   // PA1  B
		GPIOA->ODR &= ~(1U << 4);   // PA4  C
		break;

	case 8:
		GPIOA->ODR &= ~(1U << 0);   // PA0  A
		GPIOA->ODR &= ~(1U << 1);   // PA1  B
		GPIOA->ODR &= ~(1U << 4);   // PA4  C
		GPIOA->ODR &= ~(1U << 5);   // PA5  D
		GPIOA->ODR &= ~(1U << 12);  // PA12 E
		GPIOA->ODR &= ~(1U << 11);  // PA11 F
		GPIOA->ODR &= ~(1U << 6);   // PA6  G
		break;

	case 9:
		GPIOA->ODR &= ~(1U << 0);   // PA0  A
		GPIOA->ODR &= ~(1U << 1);   // PA1  B
		GPIOA->ODR &= ~(1U << 4);   // PA4  C
		GPIOA->ODR &= ~(1U << 5);   // PA5  D
		GPIOA->ODR &= ~(1U << 11);  // PA11 F
		GPIOA->ODR &= ~(1U << 6);   // PA6  G
		break;
	}
}

void clearRowsKeypad(void){
	/* Clearing the rows here */
	GPIOA->ODR &= ~(1U << 8);  // PA8
	GPIOB->ODR &= ~(1U << 9);  // PB9
	GPIOB->ODR &= ~(1U << 5);  // PB5
	GPIOB->ODR &= ~(1U << 4);  // PB4
}

void setRowsKeypad(void){
	/* Setting the rows here */
	GPIOA->ODR |= (1U << 8);  // PA8
	GPIOB->ODR |= (1U << 9);  // PB9
	GPIOB->ODR |= (1U << 5);  // PB5
	GPIOB->ODR |= (1U << 4);  // PB4
}

void set_output(){/* Setup PA8, PB9, PB5, PB4 as output (rows) */

	GPIOA->MODER &= ~(3U << 2*8);
	GPIOA->MODER |=  (1U << 2*8); // PA8 is output ( 17-16 )

	GPIOB->MODER &= ~(3U << 2*9);
	GPIOB->MODER |=  (1U << 2*9); // PB9 is output ( 19-18 )

	GPIOB->MODER &= ~(3U << 2*5);
	GPIOB->MODER |=  (1U << 2*5); // PB5 is output ( 11-10 )

	GPIOB->MODER &= ~(3U << 2*4);
	GPIOB->MODER |=  (1U << 2*4); // PB4 is output ( 09-08 )

}

void set_input(){	/* Setup PB0, PB2, PB8 as input (colums) */

	GPIOB->MODER &= ~(3U << 2*0); // PB0 is output ( 01-00 )
	GPIOB->PUPDR |=  (2U << 2*0); // Pull down mode

	GPIOB->MODER &= ~(3U << 2*2); // PB2 is output ( 05-04 )
	GPIOB->PUPDR |=  (2U << 2*2); // Pull down mode

	GPIOB->MODER &= ~(3U << 2*8); // PB8 is output ( 17-16 )
	GPIOB->PUPDR |=  (2U << 2*8); // Pull down mode


}

void set_interrupts(){	/* Setup interrupts for inputs */

	EXTI->EXTICR[0] |= (1U << 0);    // PB0
	EXTI->EXTICR[0] |= (1U << 8*2);  // PB2
	EXTI->EXTICR[2] |= (1U << 0);    // PB8


}

void set_display(){	/* Setup PA0, PA1, PA4, PA5, PA12, PA11, PA6 as output for SSD */

	GPIOA->MODER &= ~(3U << 2*0);
	GPIOA->MODER |=  (1U << 2*0);  // PA0 is output

	GPIOA->MODER &= ~(3U << 2*1);
	GPIOA->MODER |=  (1U << 2*1);  // PA1 is output

    GPIOA->MODER &= ~(3U << 2*4);
	GPIOA->MODER |=  (1U << 2*4);  // PA4 is output

	GPIOA->MODER &= ~(3U << 2*5);
	GPIOA->MODER |=  (1U << 2*5);  // PA5 is output

	GPIOA->MODER &= ~(3U << 2*12);
	GPIOA->MODER |=  (1U << 2*12);  // PA12 is output

	GPIOA->MODER &= ~(3U << 2*11);
	GPIOA->MODER |=  (1U << 2*11);  // PA11 is output

	GPIOA->MODER &= ~(3U << 2*6);
	GPIOA->MODER |=  (1U << 2*6);  // PA6 is output


}


void enable_clock(){

	RCC->IOPENR |= (1U << 0);//Enable GPIOA clock

	RCC->IOPENR |= (1U << 1);//Enable GPIOB clock

}

void set_rows(){

	/* Set all rows */
	GPIOA->ODR |= (1U << 8);
	GPIOA->ODR |= (1U << 9);
	GPIOA->ODR |= (1U << 5);
	GPIOA->ODR |= (1U << 4);

}


void rising_edge(){

	/* Rising Edge */
	EXTI->RTSR1 |= (1U << 0);  // 0th pin
	EXTI->RTSR1 |= (1U << 2);  // 2th pin
	EXTI->RTSR1 |= (1U << 8);  // 8th pin
}


void mask(){	/* Mask */
	EXTI->IMR1 |= (1U << 0);
	EXTI->IMR1 |= (1U << 2);
	EXTI->IMR1 |= (1U << 8);
}


void nvic(){	/* NVIC */
	NVIC_SetPriority(EXTI0_1_IRQn , 0);
	NVIC_EnableIRQ(EXTI0_1_IRQn);

	NVIC_SetPriority(EXTI2_3_IRQn , 0);
	NVIC_EnableIRQ(EXTI2_3_IRQn);

	NVIC_SetPriority(EXTI4_15_IRQn , 0);
	NVIC_EnableIRQ(EXTI4_15_IRQn);


}



int main(void) {

	enable_clock();

	set_output();
	set_input();
	set_interrupts();

	mask();

	nvic();

	set_display();
	set_rows();

	clearSSD();


	while(1) {

	}

	return 0;
}
