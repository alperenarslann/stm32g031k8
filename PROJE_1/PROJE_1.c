
/*
 *  Created on: 11 december 2021
 *      Author: Alpeten
 */
#include "stm32g0xx.h"
#include "BSP.h"
#include "system_stm32g0xx.h"
#include <stdlib.h> //for rand function


#define SYSTEM_CLK  16000000


volatile uint8_t num; //1 digit number comes from keypad
volatile uint8_t print_flag = 0; // flag for printnig the mode
volatile uint8_t amplitude_flag = 0; //flag for printing the amplitude
volatile uint8_t frequency_flag = 0;//flag for printing the frequency
volatile uint8_t enter_flag  = 0;
volatile uint8_t dot_flag = 0;
volatile uint8_t dot_print_flag = 0;
volatile uint8_t dot_index = 5;//assign is as invalid digit as initial
 uint8_t i = 0; //variable for check number of button presses before enter key
 volatile uint32_t tim_counter = 0;
volatile uint8_t print_counter = 0;


enum mode{sin,square,triangle,sawtooth,noise};

const uint32_t lookup_table[4][128] = {
		{
				//sin wave
			512, 537, 562, 587, 612, 637, 661, 685, 709, 732, 754, 776, 798, 818, 838,
			857, 875, 893, 909, 925, 939, 952, 965, 976, 986, 995, 1002, 1009, 1014, 1018,
			1021, 1023, 1023, 1022, 1020, 1016, 1012, 1006, 999, 990, 981, 970, 959, 946, 932,
			917, 901, 884, 866, 848, 828, 808, 787, 765, 743, 720, 697, 673, 649, 624,
			600, 575, 549, 524, 499, 474, 448, 423, 399, 374, 350, 326, 303, 280, 258,
			236, 215, 195, 175, 157, 139, 122, 106, 91, 77, 64, 53, 42, 33, 24,
			17, 11, 7, 3, 1, 0, 0, 2, 5, 9, 14, 21, 28, 37, 47,
			58, 71, 84, 98, 114, 130, 148, 166, 185, 205, 225, 247, 269, 291, 314,
			338, 362, 386, 411, 436, 461, 486, 511
},
{
			//square
			1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023,
			1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023,
			1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023,
			1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023,
			1023, 1023, 1023, 1023, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 1023
	},

	{
			//triangle
			0, 16, 32, 48, 64, 81, 97, 113, 129, 145, 161, 177, 193, 209, 226,
			242, 258, 274, 290, 306, 322, 338, 354, 371, 387, 403, 419, 435, 451, 467,
			483, 499, 516, 532, 548, 564, 580, 596, 612, 628, 644, 661, 677, 693, 709,
			725, 741, 757, 773, 789, 806, 822, 838, 854, 870, 886, 902, 918, 934, 951,
			967, 983, 999, 1015, 1015, 999, 983, 967, 951, 934, 918, 902, 886, 870, 854,
			838, 822, 806, 789, 773, 757, 741, 725, 709, 693, 677, 661, 644, 628, 612,
			596, 580, 564, 548, 532, 516, 499, 483, 467, 451, 435, 419, 403, 387, 371,
			354, 338, 322, 306, 290, 274, 258, 242, 226, 209, 193, 177, 161, 145, 129,
			113, 97, 81, 64, 48, 32, 16, 0
},

{
		//sawtooth
		0, 8, 16, 24, 32, 40, 48, 56, 64, 72, 81, 89, 97, 105, 113,
		121, 129, 137, 145, 153, 161, 169, 177, 185, 193, 201, 209, 217, 226, 234,
		242, 250, 258, 266, 274, 282, 290, 298, 306, 314, 322, 330, 338, 346, 354,
		362, 371, 379, 387, 395, 403, 411, 419, 427, 435, 443, 451, 459, 467, 475,
		483, 491, 499, 507, 516, 524, 532, 540, 548, 556, 564, 572, 580, 588, 596,
		604, 612, 620, 628, 636, 644, 652, 661, 669, 677, 685, 693, 701, 709, 717,
		725, 733, 741, 749, 757, 765, 773, 781, 789, 797, 806, 814, 822, 830, 838,
		846, 854, 862, 870, 878, 886, 894, 902, 910, 918, 926, 934, 942, 951, 959,
		967, 975, 983, 991, 999, 1007, 1015, 0
}
};




struct wave{
	float amp;
	uint8_t amp_dig[5];
	uint32_t freq;
	uint8_t freq_dig[5];
	enum mode state;
};

volatile struct wave wave;

volatile enum mode state = noise;


void reset() //function for reset the ssd
{

	GPIOB->ODR &= ~(1U << 4);	//reset  d12
	GPIOB->ODR &= ~(1U << 5);	//reset  d11
	GPIOA->ODR &= ~(1U << 8);	//reset  d9
	GPIOB->ODR &= ~(1U << 8);	//reset  d8
	GPIOA->ODR &= ~(1U << 4);	//reset  a2
	GPIOA->ODR &= ~(1U << 6);	//reset  a6
	GPIOA->ODR &= ~(1U << 7);	//reset  a7

}




void print_mode()// function for printing the modes (sine,triangular etc.)
{

	while(print_flag){
	switch(wave.state){

	case sin:
			print_digit(5);//S
			GPIOA->ODR &= ~(1U << 11);	//reset  a5
			delay_ms(2);
			GPIOA->ODR |= (1U << 11);	//set  a5


			print_digit(1);//I
			GPIOA->ODR &= ~(1U << 12);	//reset  a4
			delay_ms(2);
			GPIOA->ODR |= (1U << 12);	//set  a4


			print_digit(14);//n
			GPIOA->ODR &= ~(1U << 5);	//reset  a3
			delay_ms(2);
			GPIOA->ODR |= (1U << 5);	//set  a3


			print_digit(13);//E
			GPIOB->ODR &= ~(1U << 9);	//reset  d10
			delay_ms(2);
			GPIOB->ODR |= (1U << 9);	//set  d10


		break;
	case square:


			print_digit(5);//S
			GPIOA->ODR &= ~(1U << 11);	//reset  a5
			delay_ms(2);
			GPIOA->ODR |= (1U << 11);	//set  a5


			print_digit(18);//q
			GPIOA->ODR &= ~(1U << 12);	//reset  a4
			delay_ms(2);
			GPIOA->ODR |= (1U << 12);	//set  a4


			print_digit(16);//r
			GPIOA->ODR &= ~(1U << 5);	//reset  a3
			delay_ms(2);
			GPIOA->ODR |= (1U << 5);	//set  a3


			print_digit(13);//E
			GPIOB->ODR &= ~(1U << 9);	//reset  d10
			delay_ms(2);
			GPIOB->ODR |= (1U << 9);	//set  d10



		break;
	case triangle:


			print_digit(15);//r
			GPIOA->ODR &= ~(1U << 12);	//reset  a4
			delay_ms(2);
			GPIOA->ODR |= (1U << 12);	//set  a4


			print_digit(16);//I
			GPIOA->ODR &= ~(1U << 5);	//reset  a3
			delay_ms(2);
			GPIOA->ODR |= (1U << 5);	//set  a3


			print_digit(1);//E
			GPIOB->ODR &= ~(1U << 9);	//reset  d10
			delay_ms(2);
			GPIOB->ODR |= (1U << 9);	//set  d10



		break;
	case sawtooth:


			print_digit(5);//S
			GPIOA->ODR &= ~(1U << 11);	//reset  a5
			delay_ms(2);
			GPIOA->ODR |= (1U << 11);	//set  a5


			print_digit(15);//t
			GPIOA->ODR &= ~(1U << 12);	//reset  a4
			delay_ms(2);
			GPIOA->ODR |= (1U << 12);	//set  a4


			print_digit(15);//t
			GPIOA->ODR &= ~(1U << 5);	//reset  a3
			delay_ms(2);
			GPIOA->ODR |= (1U << 5);	//set  a3


			print_digit(17);//H
			GPIOB->ODR &= ~(1U << 9);	//reset  d10
			delay_ms(2);
			GPIOB->ODR |= (1U << 9);	//set  d10



		break;
	case noise:


			print_digit(14);//n
			GPIOA->ODR &= ~(1U << 11);	//reset  a5
			delay_ms(2);
			GPIOA->ODR |= (1U << 11);	//set  a5


			print_digit(20);//o
			GPIOA->ODR &= ~(1U << 12);	//reset  a4
			delay_ms(2);
			GPIOA->ODR |= (1U << 12);	//set  a4


			print_digit(1);//1
			GPIOA->ODR &= ~(1U << 5);	//reset  a3
			delay_ms(2);
			GPIOA->ODR |= (1U << 5);	//set  a3


			print_digit(5);//S
			GPIOB->ODR &= ~(1U << 9);	//reset  d10
			delay_ms(2);
			GPIOB->ODR |= (1U << 9);	//set  d10

		break;
	}

	}
}


void print_amplitude(){

	while(print_flag){
		print_digit(wave.amp_dig[4]);
		GPIOA->ODR &= ~(1U << 11);	//reset  a5
		delay_ms(2);
		GPIOA->ODR |= (1U << 11);	//set  a5

		print_digit(wave.amp_dig[3]);
		GPIOA->ODR &= ~(1U << 12);	//reset  a4
		delay_ms(2);
		GPIOA->ODR |= (1U << 12);	//set  a4

		print_digit(wave.amp_dig[2]);
		GPIOA->ODR &= ~(1U << 5);	//reset  a3
		delay_ms(2);
		GPIOA->ODR |= (1U << 5);	//set  a3

		print_digit(wave.amp_dig[1]);
		GPIOB->ODR &= ~(1U << 9);	//reset  d10
		delay_ms(2);
		GPIOB->ODR |= (1U << 9);	//set  d10
	}

}

void print_frequency(){

	while(print_flag){
		print_digit(wave.freq_dig[4]);
		GPIOA->ODR &= ~(1U << 11);	//reset  a5
		delay_ms(2);
		GPIOA->ODR |= (1U << 11);	//set  a5

		print_digit(wave.freq_dig[3]);
		GPIOA->ODR &= ~(1U << 12);	//reset  a4
		delay_ms(2);
		GPIOA->ODR |= (1U << 12);	//set  a4

		print_digit(wave.freq_dig[2]);
		GPIOA->ODR &= ~(1U << 5);	//reset  a3
		delay_ms(2);
		GPIOA->ODR |= (1U << 5);	//set  a3

		print_digit(wave.freq_dig[1]);
		GPIOB->ODR &= ~(1U << 9);	//reset  d10
		delay_ms(2);
		GPIOB->ODR |= (1U << 9);	//set  d10
	}
}


void set_amplitude(){

	for(int j = 0; j < 5; ++j){
	wave.amp_dig[j] = 0; //initialize the digits as zero
	}

	uint8_t zero_flag = 0;
	uint8_t one_flag = 0;
	uint8_t two_flag = 0;
	uint8_t three_flag = 0;
	uint8_t four_flag = 0;
	dot_flag = 0;

	wave.amp = 0; //delete the previous amplitude value

	while(!enter_flag){


		switch(i){
		case 0:
			if( zero_flag == 0){


				wave.amp = num;
				zero_flag  = 1;
			//	dig[i] = num;


			}
			break;
		case 1:

			if( one_flag == 0){

				wave.amp_dig[1] = num;

				if(dot_flag == 1){


					if(dot_print_flag == 1){
						wave.amp_dig[1] = 19;
						dot_print_flag = 0;
					}

				}


				else{
				wave.amp = ((wave.amp*10) + num);
				}

				one_flag  = 1;
			}
			break;

		case 2:

			if( two_flag == 0){

				//shifting operation
				wave.amp_dig[2] = wave.amp_dig[1];
				wave.amp_dig[1] = num;

				if(dot_flag == 1){


					if(dot_print_flag == 1){
						wave.amp_dig[1] = 19;
						dot_print_flag = 0;
					}
				}


				else{
				wave.amp = ((wave.amp*10) + num);
				}

				two_flag  = 1;


			}
			break;

		case 3:

			if( three_flag == 0){

				//shifting operation
				wave.amp_dig[3] = wave.amp_dig[2];
				wave.amp_dig[2] = wave.amp_dig[1];
				wave.amp_dig[1] = num;

				if(dot_flag == 1){
					wave.amp = wave.amp + (float)(num * 0.1);//handle the floating point number
					if(dot_print_flag == 1){
						wave.amp_dig[1] = 19;
						dot_print_flag = 0;
					}
				}

				else{
				wave.amp = ((wave.amp*10) + num);
				}

				three_flag  = 1;
			}
			break;

		case 4:
			if( four_flag == 0){

				//shifting operation
				wave.amp_dig[4] = wave.amp_dig[3];
				wave.amp_dig[3] = wave.amp_dig[2];
				wave.amp_dig[2] = wave.amp_dig[1];
				wave.amp_dig[1] = num;

				if(dot_flag == 1){
					wave.amp = wave.amp + (float)(num*0.01);//handle the floating point number
					if(dot_print_flag == 1){
						wave.amp_dig[1] = 19;
						dot_print_flag = 0;
					}
				}

				else{
				wave.amp = ((wave.amp*10) + num);
				}

				four_flag  = 1;
			}
			break;


		default:
			i = 0; // if i is not 0,1,2,3 or 4, assign it to zero
			dot_flag = 0;//reset the dot
			break;
		}




			print_digit(wave.amp_dig[4]);
			GPIOA->ODR &= ~(1U << 11);	//reset  a5
			delay_ms(2);
			GPIOA->ODR |= (1U << 11);	//set  a5

			print_digit(wave.amp_dig[3]);
			GPIOA->ODR &= ~(1U << 12);	//reset  a4
			delay_ms(2);
			GPIOA->ODR |= (1U << 12);	//set  a4

			print_digit(wave.amp_dig[2]);
			GPIOA->ODR &= ~(1U << 5);	//reset  a3
			delay_ms(2);
			GPIOA->ODR |= (1U << 5);	//set  a3

			print_digit(wave.amp_dig[1]);
			GPIOB->ODR &= ~(1U << 9);	//reset  d10
			delay_ms(2);
			GPIOB->ODR |= (1U << 9);	//set  d10

		}


}

void set_frequency(){

	for(int j = 0; j < 5; ++j){
	wave.freq_dig[j] = 0; //initialize the digits as zero
	}



	uint8_t zero_flag = 0;
	uint8_t one_flag = 0;
	uint8_t two_flag = 0;
	uint8_t three_flag = 0;
	uint8_t four_flag = 0;

	wave.freq = 0;

	while(!enter_flag){


		switch(i){
				case 0:
					if( zero_flag == 0){


						wave.freq = num;
						zero_flag  = 1;
					//	dig[i] = num;


					}
					break;
				case 1:

					if( one_flag == 0){

						wave.freq_dig[1] = num;


						wave.freq = ((wave.freq*10) + num);

						one_flag  = 1;
					}
					break;

				case 2:

					if( two_flag == 0){

						//shifting operation
						wave.freq_dig[2] = wave.freq_dig[1];
						wave.freq_dig[1] = num;



						wave.freq = ((wave.freq*10) + num);


						two_flag  = 1;


					}
					break;

				case 3:

					if( three_flag == 0){

						//shifting operation
						wave.freq_dig[3] = wave.freq_dig[2];
						wave.freq_dig[2] = wave.freq_dig[1];
						wave.freq_dig[1] = num;


						wave.freq = ((wave.freq*10) + num);


						three_flag  = 1;
					}
					break;

				case 4:
					if( four_flag == 0){

						//shifting operation
						wave.freq_dig[4] = wave.freq_dig[3];
						wave.freq_dig[3] = wave.freq_dig[2];
						wave.freq_dig[2] = wave.freq_dig[1];
						wave.freq_dig[1] = num;


						wave.freq = ((wave.freq*10) + num);

						four_flag  = 1;
					}
					break;


				default:
					i = 0; // if i is not 0,1,2,3 or 4, assign it to zero
					break;
				}




			print_digit(wave.freq_dig[4]);
			GPIOA->ODR &= ~(1U << 11);	//reset  a5
			delay_ms(2);
			GPIOA->ODR |= (1U << 11);	//set  a5

			print_digit(wave.freq_dig[3]);
			GPIOA->ODR &= ~(1U << 12);	//reset  a4
			delay_ms(2);
			GPIOA->ODR |= (1U << 12);	//set  a4

			print_digit(wave.freq_dig[2]);
			GPIOA->ODR &= ~(1U << 5);	//reset  a3
			delay_ms(2);
			GPIOA->ODR |= (1U << 5);	//set  a3

			print_digit(wave.freq_dig[1]);
			GPIOB->ODR &= ~(1U << 9);	//reset  d10
			delay_ms(2);
			GPIOB->ODR |= (1U << 9);	//set  d10

		}




}



void EXTI2_3_IRQHandler(void)//interrupt function for keypads first and last columns
{

	//reset ssd pins to have a clear look
	GPIOA->ODR |= (1U << 11);	//set  a5
	GPIOA->ODR |= (1U << 12);	//set  a4
	GPIOA->ODR |= (1U << 5);	//set  a3
	GPIOB->ODR |= (1U << 9);	//set  d10

	/*handles first and last columns*/



	enter_flag = 0;

	GPIOB->ODR &= ~(1U << 9);	//reset  d10

	if((GPIOB->IDR >> 3) & 1){
		clear_rows_keypad();
		//try for each keypad rows

		GPIOB->ODR |= (1U << 6); //keypad A button
		if((GPIOB->IDR >> 3) & 1){

			print_flag = 0; //get out printing
			amplitude_flag = 1;
			frequency_flag  = 0;
			i = 0;
			//print_digit(11); //letter A
			delay_ms(500); //little bit delay for debouncing


		}


		GPIOB->ODR &= ~(1U << 6); //close first row


		GPIOB->ODR |= (1U << 7); //keypad B button
		if((GPIOB->IDR >> 3) & 1){


			print_flag = 0; //get out printing
			amplitude_flag = 0;
			frequency_flag  = 1;
			//print_digit(8);
			delay_ms(500); //little bit delay for debouncing

		}


		GPIOB->ODR &= ~(1U << 7); //close second row



		GPIOA->ODR |= (1U << 15); //keypad C button
		if((GPIOB->IDR >> 3) & 1){

			amplitude_flag = 0;
			frequency_flag  = 0;
			//print_digit(12);
			wave.state++;

			if(wave.state > noise){
				wave.state = sin;
			}

			delay_ms(500); //little bit delay for debouncing

		}


		GPIOA->ODR &= ~(1U << 15); //close third row



		GPIOB->ODR |= (1U << 1); //keypad D button
		if((GPIOB->IDR >> 3) & 1){

			frequency_flag  = 0;
			print_flag = 1;
			print_counter++; //counter for switching between printing modes(amplitude,freq,wave type)
			delay_ms(500); //little bit delay for debouncing

		}


		GPIOB->ODR &= ~(1U << 1); //close fourth row


		EXTI-> RPR1 |= (1 << 3); //clear pending bit
		set_rows_keypad();
	}





	if((GPIOB->IDR >> 2) & 1){
		clear_rows_keypad();
		//try for each keypad rows

		GPIOB->ODR |= (1U << 6);  //keypad 1 button
		if((GPIOB->IDR >> 2) & 1){

			print_flag = 0; //get out printing
			num = 1;
			i++;
			//print_digit(1);
			delay_ms(500); //little bit delay for debouncing
		}


		GPIOB->ODR &= ~(1U << 6); //close first row


		GPIOB->ODR |= (1U << 7);  //keypad 4 button
		if((GPIOB->IDR >> 2) & 1){


			print_flag = 0; //get out printing
			num = 4;
			i++;
			//print_digit(4);
			delay_ms(500); //little bit delay for debouncing
		}


		GPIOB->ODR &= ~(1U << 7); //close second row



		GPIOA->ODR |= (1U << 15); //keypad 7 button
		if((GPIOB->IDR >> 2) & 1){

			print_flag = 0; //get out printing
			num  = 7;
			i++;
			//print_digit(7);
			delay_ms(500); //little bit delay for debouncing
		}


		GPIOA->ODR &= ~(1U << 15); //close third row



		GPIOB->ODR |= (1U << 1);  //keypad * button
		if((GPIOB->IDR >> 2) & 1){

			i++;
			print_flag = 0; //get out printing
			dot_flag = 1;
			dot_print_flag = 1;
			dot_index = i;


			//print_digit(19);
			delay_ms(500); //little bit delay for debouncing
		}


		GPIOB->ODR &= ~(1U << 1); //close fourth row


		EXTI-> RPR1 |= (1 << 2); //clear pending bit
		set_rows_keypad();
	}



}




void EXTI0_1_IRQHandler(void)//interrupt function for keypads second and third columns
{
	//reset ssd pins to have a clear look
	GPIOA->ODR |= (1U << 11);	//set  a5
	GPIOA->ODR |= (1U << 12);	//set  a4
	GPIOA->ODR |= (1U << 5);	//set  a3
	GPIOB->ODR |= (1U << 9);	//set  d10
	print_flag = 0;//get out from print state

	/*handles second and third columns*/



	GPIOB->ODR &= ~(1U << 9);	//reset  d10

	if((GPIOB->IDR >> 0) & 1){
		clear_rows_keypad();
		//try for each keypad rows

		GPIOB->ODR |= (1U << 6);  //keypad 3 button
		if((GPIOB->IDR >> 0) & 1){

			enter_flag = 0;
			//print_digit(3);
			num  = 3;
			i++;
			delay_ms(500); //little bit delay for debouncing


		}
		GPIOB->ODR &= ~(1U << 6); //close first row


		GPIOB->ODR |= (1U << 7);  //keypad 6 button
		if((GPIOB->IDR >> 0) & 1){

			enter_flag = 0;
			//print_digit(6);
			num = 6;
			i++;
			delay_ms(500); //little bit delay for debouncing




		}
		GPIOB->ODR &= ~(1U << 7); //close second row



		GPIOA->ODR |= (1U << 15);  //keypad 9 button
		if((GPIOB->IDR >> 0) & 1){


			//print_digit(9);
			enter_flag = 0;
			num = 9;
			i++;
			delay_ms(500); //little bit delay for debouncing
		}


		GPIOA->ODR &= ~(1U << 15); //close third row



		GPIOB->ODR |= (1U << 1);  //keypad # button
		if((GPIOB->IDR >> 0) & 1){

			amplitude_flag = 0;
			frequency_flag = 0;
			enter_flag = 1;
			//print_digit(15);
			delay_ms(500); //little bit delay for debouncing
		}


		GPIOB->ODR &= ~(1U << 1); //close fourth row

		EXTI-> RPR1 |= (1 << 0); //clear pending bit
		set_rows_keypad();


	}





	if((GPIOA->IDR >> 1) & 1){
		clear_rows_keypad();
		//try for each keypad rows

		GPIOB->ODR |= (1U << 6);  //keypad 2 button
		if((GPIOA->IDR >> 1) & 1){


			enter_flag = 0;
			//print_digit(2);
			num = 2;
			i++;
			delay_ms(500); //little bit delay for debouncing



		}

		GPIOB->ODR &= ~(1U << 6); //close first row


		GPIOB->ODR |= (1U << 7);  //keypad 5 button
		if((GPIOA->IDR >> 1) & 1){



			enter_flag = 0;
			//print_digit(5);
			num = 5;
			i++;
			delay_ms(500); //little bit delay for debouncing
		}


		GPIOB->ODR &= ~(1U << 7); //close second row



		GPIOA->ODR |= (1U << 15);  //keypad 8 button
		if((GPIOA->IDR >> 1) & 1){


			enter_flag = 0;
			//print_digit(8);
			num = 8;
			i++;
			delay_ms(500); //little bit delay for debouncing



		}

		GPIOA->ODR &= ~(1U << 15); //close third row



		GPIOB->ODR |= (1U << 1);  //keypad 0 button
		if((GPIOA->IDR >> 1) & 1){




			//print_digit(0);
			enter_flag = 0;
			num = 0;
			i++;
			delay_ms(500); //little bit delay for debouncing


		}


		GPIOB->ODR &= ~(1U << 1); //close fourth row

		EXTI-> RPR1 |= (1 << 1); //clear pending bit
		set_rows_keypad();

	}

}




void TIM2_IRQHandler(void) {
    // update duty (CCR2)


	uint32_t bol = (245*(wave.freq+1));

	 TIM2->PSC = (SYSTEM_CLK / bol);


	switch(wave.state){
	case sin:
		TIM2 -> CCR1 =(float)(wave.amp/3.3)*(lookup_table[0][tim_counter]);
		break;
	case square:
		TIM2 -> CCR1 =(float) (wave.amp/3.3)* lookup_table[1][tim_counter];
		break;
	case triangle:
		TIM2 -> CCR1 = (float) (wave.amp/3.3)*lookup_table[2][tim_counter];
		break;
	case sawtooth:
		TIM2 -> CCR1 =(float) (wave.amp/3.3)*lookup_table[3][tim_counter];
		break;
	case noise:
		TIM2 -> CCR1 = rand() % 1024;
		break;

	}

	tim_counter++;
	if(tim_counter>128){
		tim_counter = 0;
	}

    // Clear update status register
		TIM2->SR &= ~(1U << 0);
}




void init_pwm(){



    // Enable TIM2 clock
    RCC->APBENR1 |= RCC_APBENR1_TIM2EN;

    // Set alternate function to 2
    // 0 comes from PA0
    GPIOA->AFR[0] |= (2U << 4*0);
    // Select AF from Moder
    setMode('A',0,'F');

    //I NEED TIM2 CH 1

    // zero out the control register just in case
    TIM2->CR1 = 0;

    // Select PWM Mode 1
    TIM2->CCMR1 |= (6U << 4);
    // Preload Enable
    TIM2->CCMR1 |= TIM_CCMR1_OC1PE;

    // Capture compare ch1 enable
    TIM2->CCER |= TIM_CCER_CC1E;

    // zero out counter
    TIM2->CNT = 0;
    //initialize the frequency
    TIM2->PSC = 100;
    TIM2->ARR = 245;

    // zero out duty
    TIM2->CCR1 = 0;

    // Update interrupt enable
    TIM2->DIER |= (1 << 0);

    // TIM2 Enable
    TIM2->CR1 |= TIM_CR1_CEN;

    NVIC_SetPriority(TIM2_IRQn, 3);
    NVIC_EnableIRQ(TIM2_IRQn);
}






int main(void) {


	init_systick(SystemCoreClock/1000);

/*open clocks*/
	openClock('A');
	openClock('B');

	/*configure 7 segment pins*/
	setMode('A',8,'O');
	setMode('A',4,'O');
	setMode('A',5,'O');
	setMode('A',12,'O');
	setMode('A',6,'O');
	setMode('A',7,'O');
	setMode('A',11,'O');

	setMode('B',4,'O');
	setMode('B',5,'O');
	setMode('B',8,'O');
	setMode('B',9,'O');



	//set ssd digits high as initial to close all digits
	GPIOA->ODR |= (1U << 11);	//set  a5
	GPIOA->ODR |= (1U << 12);	//set  a4
	GPIOA->ODR |= (1U << 5);	//set  a3
	GPIOB->ODR |= (1U << 9);	//set  d10



	/*configure keypad*/
	//rows are output, columns are input
	config_keypad_pins();//configure the pins
	config_keypad_IRQs();//configure the interrupts

	//initialize the wave
	wave.state = square;

	init_pwm();




while(1){

	if(print_flag == 1){//print the wave mode if the D button is pushed

		switch(print_counter){
		case 0:
			print_amplitude();
			break;
		case 1:
			print_mode();
			break;
		case 2:
			print_frequency();
			break;

		default:
			print_counter = 0;
			break;
		}


	}

	if(amplitude_flag == 1){ //switch to amplitude mode
		num = 0;
		set_amplitude();
	}

	if(frequency_flag == 1){
		num = 0;
		set_frequency();
	}

	   //TIM2->ARR = (uint32_t)(16000000/(8*(wave.freq+1)));//8 comes from arr







}


return 0;

}


