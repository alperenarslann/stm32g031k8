

/*
*
*
*Arslan - büber- taşkıran
*
*lab2_q1
*
*/

.syntax unified
.cpu cortex-m0plus
.fpu softvfp
.thumb


/* make linker see this */
.global Reset_Handler

/* get these from linker script */
.word _sdata
.word _edata
.word _sbss
.word _ebss


/* define peripheral addresses from RM0444 page 57, Tables 3-4 */
.equ RCC_BASE,         (0x40021000)          // RCC base address
.equ RCC_IOPENR,       (RCC_BASE   + (0x34)) // RCC IOPENR register offset bu sey hangi portlara clock gelcegini buluyo cool yani


.equ DELAY_CONST, (0x100000) //0x500000 1sye yakın oluyo bunla

.equ GPIOA_BASE,       (0x50000000)          // GPIOA base address
.equ GPIOA_MODER,      (GPIOA_BASE + (0x00)) // GPIOA MODER register offset
.equ GPIOA_ODR,        (GPIOA_BASE + (0x14)) // GPIOA ODR register offset
.equ GPIOA_IDR,        (GPIOA_BASE + (0x10)) // GPIOA ODR register offset



/* vector table, +1 thumb mode */
.section .vectors
vector_table:
	.word _estack             /*     Stack pointer */
	.word Reset_Handler +1    /*     Reset handler */
	.word Default_Handler +1  /*       NMI handler */
	.word Default_Handler +1  /* HardFault handler */
	/* add rest of them here if needed */


/* reset handler */
.section .text
Reset_Handler:
	/* set stack pointer */
	ldr r0, =_estack
	mov sp, r0

	/* initialize data and bss 
	 * not necessary for rom only code 
	 * */
	bl init_data
	/* call main */
	bl main
	/* trap if returned */
	b .


/* initialize data and bss sections */
.section .text
init_data:

	/* copy rom to ram */
	ldr r0, =_sdata
	ldr r1, =_edata
	ldr r2, =_sidata
	movs r3, #0
	b LoopCopyDataInit

	CopyDataInit:
		ldr r4, [r2, r3]
		str r4, [r0, r3]
		adds r3, r3, #4

	LoopCopyDataInit:
		adds r4, r0, r3
		cmp r4, r1
		bcc CopyDataInit

	/* zero bss */
	ldr r2, =_sbss
	ldr r4, =_ebss
	movs r3, #0
	b LoopFillZerobss

	FillZerobss:
		str  r3, [r2]
		adds r2, r2, #4

	LoopFillZerobss:
		cmp r2, r4
		bcc FillZerobss

	bx lr


/* default handler */
.section .text
Default_Handler:
	b Default_Handler


/* main function */
.section .text
main:
	/* enable GPIO A,C,D clock, bit2 on IOPENR */
	ldr r6, =RCC_IOPENR
	ldr r5, [r6]
	movs r4,  0b00001101
	orrs r5, r5, r4
	str r5, [r6]

	/* setup A0,1,2,3,4,5,6,7 as output */
	ldr r6, =GPIOA_MODER
	ldr r5, [r6]
	/*00 yaptık */
	ldr r4, =#0b1111111111111100001111
	bics r5, r5, r4

	ldr r4, =#0b01010101010100000101

	/*kaydettik*/
	orrs r5, r5, r4
	str r5, [r6]
//8 ledlik output alındı, a0,a1,a2,a3,a6,a7,d5,d4 pinleri
//modderda a0,a1,a2,a4,a5,a6,a7,a8,a9 a tekabül ediyo


/*LEDLERİN SIRASI
1.led -> d5 -> modder9
2.led -> a7 -> modder7
3.led -> a6 -> modder6
4.led -> a0 -> modder0
5.led -> a1 -> mooder1
6.led -> a2 -> modder4
7.led -> a3 -> modder5
8.led -> d9 -> modder8
button input->d4->modder10*/

	loop:

		bl button
		/*error ledini sıfırla*/
		ldr r6, =GPIOA_ODR
		ldr r5,[r6]
		movs r4, #0x1
		lsls r4, #8
		bics r5, r5, r4
		str r5, [r6]

		/*------t0---led kapama -------------*/
		ldr r6, =GPIOA_ODR
		ldr r5,[r6]
		/*6. pine 1 atandı*/
		ldr r4, =0b1111110011
		bics r5, r5, r4
		str r5, [r6]
		ldr r1, =DELAY_CONST
		bl delay

		bl button
		/*error ledini sıfırla*/
		ldr r6, =GPIOA_ODR
		ldr r5,[r6]
		movs r4, #0x1
		lsls r4, #8
		bics r5, r5, r4
		str r5, [r6]


		/*----------------t1 led yakma---------------*/
		/*veriler alındı*/
		ldr r6, =GPIOA_ODR
		ldr r5,[r6]
		movs r4, #0x1
		orrs r5, r5, r4
		str r5, [r6]
		ldr r1, =DELAY_CONST
		bl delay

		bl button
		/*error ledini sıfırla*/
		ldr r6, =GPIOA_ODR
		ldr r5,[r6]
		movs r4, #0x1
		lsls r4, #8
		bics r5, r5, r4
		str r5, [r6]

		/*---------------t2 led yakma--------------------*/
				/*veriler alındı*/
		ldr r6, =GPIOA_ODR
		ldr r5,[r6]
		ldr r4, =0b0001000011
		orrs r5, r5, r4
		str r5, [r6]
		ldr r1, =DELAY_CONST
		bl delay



		bl button
		/*error ledini sıfırla*/
		ldr r6, =GPIOA_ODR
		ldr r5,[r6]
		movs r4, #0x1
		lsls r4, #8
		bics r5, r5, r4
		str r5, [r6]


		/*--------t3 led yakma------------*/
		/*veriler alındı*/
		ldr r6, =GPIOA_ODR
		ldr r5,[r6]
		ldr r4, =0b0011010011
		orrs r5, r5, r4
		str r5, [r6]
		ldr r1, =DELAY_CONST
		bl delay

		bl button
		/*error ledini sıfırla*/
		ldr r6, =GPIOA_ODR
		ldr r5,[r6]
		movs r4, #0x1
		lsls r4, #8
		bics r5, r5, r4
		str r5, [r6]



		/*-------------t4 led yakma---------------------*/
		/*veriler alındı*/
		ldr r6, =GPIOA_ODR
		ldr r5,[r6]
		ldr r4, =0b1011110011
		orrs r5, r5, r4
		str r5, [r6]
		ldr r1, =DELAY_CONST
		bl delay

		bl button
		/*error ledini sıfırla*/
		ldr r6, =GPIOA_ODR
		ldr r5,[r6]
		movs r4, #0x1
		lsls r4, #8
		bics r5, r5, r4
		str r5, [r6]



		/*--------t5 led yakma------------*/
		/*veriler alındı*/
		ldr r6, =GPIOA_ODR
		ldr r5,[r6]
		ldr r4, =#0b1111111111
		bics r5, r5, r4
		ldr r4, =0b0011010011
		orrs r5, r5, r4
		str r5, [r6]
		ldr r1, =DELAY_CONST
		bl delay

		bl button
		/*error ledini sıfırla*/
		ldr r6, =GPIOA_ODR
		ldr r5,[r6]
		movs r4, #0x1
		lsls r4, #8
		bics r5, r5, r4
		str r5, [r6]



		/*---------------t6 led yakma--------------------*/
				/*veriler alındı*/
		ldr r6, =GPIOA_ODR
		ldr r5,[r6]
		ldr r4, =#0b1111111111
		bics r5, r5, r4
		ldr r4, =0b0001000011
		orrs r5, r5, r4
		str r5, [r6]
		ldr r1, =DELAY_CONST
		bl delay


		bl button
		/*error ledini sıfırla*/
		ldr r6, =GPIOA_ODR
		ldr r5,[r6]
		movs r4, #0x1
		lsls r4, #8
		bics r5, r5, r4
		str r5, [r6]




		/*----------------t7 led yakma---------------*/
		/*veriler alındı*/
		ldr r6, =GPIOA_ODR
		ldr r5,[r6]
		ldr r4, =#0b1111111111
		bics r5, r5, r4
		movs r4, #0x1
		orrs r5, r5, r4
		str r5, [r6]
		ldr r1, =DELAY_CONST
		bl delay

		/*error ledini sıfırla*/
		ldr r6, =GPIOA_ODR
		ldr r5,[r6]
		movs r4, #0x1
		lsls r4, #8
		bics r5, r5, r4
		str r5, [r6]


		b loop

		delay:
		 subs r1, r1, #1 /*1. registerdaki sayıyı 1 azalt*/
		 bne delay  //branch not equal than
		 bx lr      // branch equal

		button:
		ldr r6, =GPIOA_IDR
		ldr r5, [r6]
		ldr r4, =#0b111101111111111
		bics r5, r5, r4
		movs r4, #1
		lsls r4, #10

		cmp r5, r4
		beq stop_led
		bx lr

		stop_led:
		ldr r6, =GPIOA_ODR
		ldr r5,[r6]
		movs r4, #0x1
		lsls r4, #8
		orrs r5, r5, r4
		str r5, [r6]
		b button


