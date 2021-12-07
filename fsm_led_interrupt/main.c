

#ifndef uint32_t
#define uint32_t unsigned int
#endif

/* Define peripheral addresses from RM0444 page 57, Tables 3-4 */
#define RCC_BASE         (0x40021000)          // RCC base address
#define RCC_IOPENR_ADDR  (RCC_BASE + (0x34))   // RCC IOPENR register offset

#define GPIOC_BASE       (0x50000800)          // GPIOC base address
#define GPIOC_MODER_ADDR (GPIOC_BASE + (0x00)) // GPIOC MODER register offset
#define GPIOC_ODR_ADDR   (GPIOC_BASE + (0x14)) // GPIOC ODR register offset

#define GPIOA_BASE       (0x50000000)          // GPIOA base address
#define GPIOA_MODER_ADDR (GPIOA_BASE + (0x00)) // GPIOA MODER register offset
#define GPIOA_ODR_ADDR   (GPIOA_BASE + (0x14)) // GPIOA ODR register offset
#define GPIOA_IDR_ADDR 	 (GPIOA_BASE + (0x10))

#define EXTI_RTSR1_ADDR	 (0x40021800)
#define EXTI_RPR1_ADDR	 (0x4002180c) //(EXTI_RTSR1_ADDR + (0xC))



#define DELAY_ONE_SEC 1777777

/* Define pointers to the addresses */
volatile uint32_t* RCC_IOPENR  = (uint32_t *)(RCC_IOPENR_ADDR);
volatile uint32_t* GPIOC_ODR   = (uint32_t *)(GPIOC_ODR_ADDR);
volatile uint32_t* GPIOC_MODER = (uint32_t *)(GPIOC_MODER_ADDR);

volatile uint32_t* GPIOA_ODR   = (uint32_t *)(GPIOA_ODR_ADDR);
volatile uint32_t* GPIOA_MODER = (uint32_t *)(GPIOA_MODER_ADDR);
volatile uint32_t* GPIOA_IDR   = (uint32_t *)(GPIOA_IDR_ADDR);

volatile uint32_t* EXTI_RTSR1  = (uint32_t *)(EXTI_RTSR1_ADDR);
volatile uint32_t* EXTI_RPR1   = (uint32_t *)(EXTI_RPR1_ADDR);




int main(void);
void Default_Handler(void);
void Reset_Handler(void);
void _init_data(void);

typedef void (* pFunc)(void);
extern unsigned long _estack;

__attribute__ ((section(".vectors")))
pFunc vector_table[] = {
    (pFunc)(&_estack), /* 0x000 Stack Pointer */
    Reset_Handler,     /* 0x004 Reset         */
    Default_Handler,   /* 0x008 NMI           */
    Default_Handler    /* 0x00C HardFault     */
    /* rest of the interrupts should go here  */
};

/* initialize data and bss sections */
void _init_data(void) {
    /* these are symbols from linker script */
    extern unsigned long __etext, __data_start__, __data_end__, __bss_start__, __bss_end__;
    unsigned long *src = &__etext;
    unsigned long *dst = &__data_start__;

    /* ROM has data at end of text. copy it */
    while (dst < &__data_end__)
        *dst++ = *src++;

    /* zero bss */
    for (dst = &__bss_start__; dst< &__bss_end__; dst++)
        *dst = 0;
}

/* Entry point, calls main after prologue */
void Reset_Handler(void) {
    _init_data();
     main();
    for(;;);
}

/* All faults will end up here, since we did not define the rest of the vector
 * table, the other exceptions/interrupts will most likely crash the program.
 */
void Default_Handler(void) { for(;;); }



enum{
	mod0 = 0,mod1 = 1,mod2 = 2,mod3 = 3,mod4 = 4,mod5 = 5
};

void setup(){
    /* Enable GPIOC and GPIOA clock */
    *RCC_IOPENR |= (0b00101);
	/* Setup AC6 for led */
    *GPIOA_MODER &= ~(uint32_t)(0b11 << 2*6);
    *GPIOA_MODER |= (1U << 2*6);

    /*setup Button input pin*/

    *GPIOA_MODER &= ~(uint32_t)(0b11 << 2*7);//set PA_7 as input mode
    *GPIOA_IDR = *GPIOA_IDR | (uint32_t)(0b111 < 2);

}

void delay(uint32_t s){
	while(s>0){
		s--;
	}
}


void states(volatile uint32_t mod){


			switch(mod){
	case mod0:
		*GPIOA_ODR &= ~(uint32_t)(0b1 << 6);
		delay(DELAY_ONE_SEC/10);//add some delay for neglect bouncing effect
		break;

	case mod1:
		*GPIOA_ODR |= (1U << 6);//led aç
			delay(2*DELAY_ONE_SEC);
		// for(volatile uint32_t s=1777777; s>0; s--); //yaklaşık 9 clock cycle her loop, 16m / 9 = 1.77m
	        *GPIOA_ODR &= ~(1U << 6);//led kapat
	        delay(2*DELAY_ONE_SEC);
		break;
	case mod2:
			*GPIOA_ODR |= (1U << 6);//led aç
				delay(DELAY_ONE_SEC);
			// for(volatile uint32_t s=1777777; s>0; s--); //yaklaşık 9 clock cycle her loop, 16m / 9 = 1.77m
		        *GPIOA_ODR &= ~(1U << 6);//led kapat
		        delay(DELAY_ONE_SEC);
		break;

	case mod3:
		*GPIOA_ODR |= (1U << 6);//led aç
			delay(DELAY_ONE_SEC/2);
		// for(volatile uint32_t s=1777777; s>0; s--); //yaklaşık 9 clock cycle her loop, 16m / 9 = 1.77m
	        *GPIOA_ODR &= ~(1U << 6);//led kapat
	        delay(DELAY_ONE_SEC/2);
		break;

	case mod4:
		*GPIOA_ODR |= (1U << 6);//led aç
			delay(DELAY_ONE_SEC/10);
		// for(volatile uint32_t s=1777777; s>0; s--); //yaklaşık 9 clock cycle her loop, 16m / 9 = 1.77m
	        *GPIOA_ODR &= ~(1U << 6);//led kapat
	        delay(DELAY_ONE_SEC/10);

		break;

	case mod5:
		*GPIOA_ODR |= (uint32_t)(0b1 << 6);
		delay(DELAY_ONE_SEC/10);//add some delay for neglect bouncing effect
		break;

}

}

void EXTI7_10_IRQHandler(void){


}


int main(void) {
	setup();
	volatile unsigned int mod = mod2;
	uint32_t deneme;


	while(1){

		states(mod);

		deneme = *GPIOA_IDR;
		deneme &= ~(uint32_t)(0b1111111101111111);
		if((deneme) == (uint32_t) 0b10000000){
			mod = mod + 1;
			if(mod > mod5){
				mod = mod0;
			}
		}
	}


    return 0;
}
