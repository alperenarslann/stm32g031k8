/*
 * asm.s
 *
 * author: Furkan Cayci 
 *
 * description: Added the necessary stuff for turning on the green LED on the 
 *   G031K8 Nucleo board. Mostly for teaching.
 */

/*		ALPEREN ARSLAN COOL PROJECT
		assembly öğretici pintipanda tarzı, acelemiz yok çayınızı kapın gelin beraber öğrencez!!
UNSTOPPABLE MACHİNEEEEEEEEEEEEEEEEEEE
8 led var yancak sırayla böyle gzl olcak*/

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

.equ GPIOC_BASE,       (0x50000800)          // GPIOC base address
.equ GPIOC_MODER,      (GPIOC_BASE + (0x00)) // GPIOC MODER register offset
.equ GPIOC_ODR,        (GPIOC_BASE + (0x14)) // GPIOC ODR offset, output data register, artik output verebilecez
.equ GPIOOC_IDR,		(GPIOC_BASE + (0x10)) //GPIOC IDR offset, input data register, artik input alabilecez


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
	/* enable GPIO A,B,C clock, bit2 on IOPENR */
	/*delikanlı adamız belki lazım olur diye 3 portu da actım ziyan seviyom galiba*/
	ldr r6, =RCC_IOPENR
	ldr r5, [r6]
	/* movs expects imm8, so this should be fine */
	movs r4, /*0x7 deneysel şeylr*/ 0b00000111 //çaycı bit yerine 0x7 yazıyo aynı şey ama daha karışık gerek yk
/*0x4 = 00000100 sadece c portu için olan yeri açtık
 A->0. register, B-> 1. register, C->2.register D->3. register gerisi alzım değil zaten
*/
	orrs r5, r5, r4
	str r5, [r6]

	/* setup PC6 for led 01 for bits 12-13 in MODER */
	ldr r6, =GPIOC_MODER //önce göte gelmemek için r6dan aldığımız state şeysisini r5e atoyoz ki r6 değişmesin
	ldr r5, [r6]

	//simdi kendi yontemimizle yapalim and kullanmaz delikanlı adam (bunu da çaycı yapmış btw)

/*bic kullanımını çözdümmmmm
r1 = 11111, r2 = 011001 olsun diyelim
bic r1, r1, r2 dersek r1le r2yi seyyapar r2de 1 olan yerlerde bitek r1i sıfırlar, geri kalan bitler sabit kalır!!
yani output 100110 olur!! çözdük ya hintli dayılar sağolsun
*/
	movs r4, 0x3  //0b011
	lsls r4, r4, #12 //r4 e 12bit left shift attik 0000000011 di ya 11'ler 12 bit ilerde artık şey gibi 00001100000000000
	bics r5, r5, r4  //bit clear
	movs r4, 0x1
	lsls r4, r4, #12
	orrs r5, r5, r4
	str r5, [r6]

//ben bunu da beğenmedim

/*hadi şimdi de PC7 ye output diyelim bakalım olcak mı*/

	movs r4, 0b011;
	lsls r4, r4, #14
	bics r5,r5, r4
	movs r4,#1
	lsls r4, r4, #14
	orrs r5,r5,r4
	str r5, [r6] //valla oldu çözdük işi


	/* turn on led connected to C6 in ODR */
	ldr r6, =GPIOC_ODR
	ldr r5, [r6]
	movs r4, 0x40
	orrs r5, r5, r4
	str r5, [r6]

	/* for(;;); */
	b .

	/* this should never get executed */
	nop

