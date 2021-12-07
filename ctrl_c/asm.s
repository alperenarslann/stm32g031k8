/*
 * asm.s
 *
 * author: Furkan Cayci 
 *
 * description: Added the necessary stuff for turning on the green LED on the 
 *   G031K8 Nucleo board. Mostly for teaching.
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
.equ RCC_IOPENR,       (RCC_BASE   + (0x34)) // RCC IOPENR register offset

.equ GPIOB_BASE,       (0x50000400)          // GPIOB base address
.equ GPIOB_MODER,      (GPIOB_BASE + (0x00)) // GPIOB MODER register offset
.equ GPIOB_ODR,        (GPIOB_BASE + (0x14)) // GPIOB ODR register offset
.equ GPIOB_IDR,        (GPIOB_BASE + (0x10)) // GPIOB IDR register offset

.equ GPIOA_BASE,       (0x50000000)          // GPIOB base address
.equ GPIOA_MODER,      (GPIOA_BASE + (0x00)) // GPIOB MODER register offset
.equ GPIOA_ODR,        (GPIOA_BASE + (0x14)) // GPIOB ODR register offset


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
	/* enable GPIOB clock, bit2 on IOPENR */
	ldr r6, =RCC_IOPENR
	ldr r5, [r6]
	/* movs expects imm8, so this should be fine */
	movs r4, 0x3
	orrs r5, r5, r4
	str r5, [r6]




	/* setup PC6 for led 01 for bits 12-13 in MODER */
	ldr r6, =GPIOA_MODER
	ldr r5, [r6]
	/* cannot do with movs, so use pc relative */
	ldr r4, =0x3000
	mvns r4, r4
	ands r5, r5, r4
	ldr r4, =0x1000
	orrs r5, r5, r4
	str r5, [r6]

	/* turn on led connected to C6 in ODR */
	ldr r6, =GPIOA_ODR
	ldr r5, [r6]
	movs r4, 0x40
	orrs r5, r5, r4
	str r5, [r6]

	/* setup PC6 for led 01 for bits 12-13 in MODER */
	ldr r6, =GPIOB_MODER
	ldr r5, [r6]
	/* cannot do with movs, so use pc relative */
	ldr r4, =0x1555
	ands r5, r5, r4
	str r5, [r6]
	movs r0 , #0x0
	movs r1 , #0x9

	b loop




loop:

	ldr r6, =GPIOB_IDR
	ldr r5, [r6]
	lsrs r3,r5,#7 // üst button
	lsrs r2,r5,#8 // alt button
	cmp r3, #0x1
	beq changeButtonOne
	cmp r2, #0x1
	beq changeButtonTwo
	b loop

changeButtonOne:
	movs r2, #0x0
	ldr r3,=0xffffff
	cmp r0, #0x1
	beq changeBahadir
	blt changeHakan
	bgt changeAlperen
changeButtonTwo:
	movs r2, #0x0
	ldr r3,=0xffffff
	cmp r1, #0x0
	beq r4Zero
	cmp r1, #0x1
	beq r4One
	cmp r1, #0x2
	beq r4Two
	cmp r1, #0x3
	beq r4Three
	cmp r1, #0x4
	beq r4Four
	cmp r1, #0x5
	beq r4Five
	cmp r1, #0x6
	beq r4Six
	cmp r1, #0x7
	beq r4Seven
	cmp r1, #0x8
	beq r4ZEight
	cmp r1, #0x9
	beq r4Nine
	bne loop
delay:
	adds r2,r2,#0x4
	cmp r2,r3
	blt delay
	b changeButtonTwo
DelayForOne:
	adds r2,r2,#0x6
	cmp r2,r3
	blt DelayForOne
	b loop
lightTheCandle:
	ldr r6, =GPIOB_ODR
	ldr r5, [r6]
	orrs r5,r5,r4
	mvns r5,r4
	#lsls r5,r5,#25
	str r5 , [r6]
	subs r1,r1,#1
	b delay
r4Zero:
	movs r4, #0x3f
	b lightTheCandle
r4One:
	movs r4, #0x06
	b lightTheCandle
r4Two:
	movs r4, #0x5b
	b lightTheCandle
r4Three:
	movs r4, #0x4f
	b lightTheCandle
r4Four:
	movs r4, #0x66
	b lightTheCandle
r4Five:
	movs r4, #0x6d
	b lightTheCandle
r4Six:
	movs r4, #0x7d
	b lightTheCandle
r4Seven:
	movs r4, #0x07
	b lightTheCandle
r4ZEight:
	movs r4, #0x7f
	b lightTheCandle
r4Nine:
	movs r4, #0x67

	b lightTheCandle
	b loop


changeBahadir:
	movs r1, #0x1
	ldr r6, =GPIOB_ODR
	ldr r5, [r6]
	movs r4, #0x06
	orrs r5,r5,r4
	mvns r5,r4
	#lsls r5,r5,#25
	str r5 , [r6]
	adds r0,r0,#1
	b DelayForOne

changeHakan:
	movs r1, #0x3
	ldr r6, =GPIOB_ODR
	ldr r5, [r6]
	movs r4, #0x4f
	orrs r5,r5,r4
	mvns r5,r4
	#lsls r5,r5,#25
	str r5 , [r6]
	adds r0,r0,#1
	b DelayForOne
changeAlperen:
	movs r1, #0x9
	ldr r6, =GPIOB_ODR
	ldr r5, [r6]
	movs r4, #0x67
	orrs r5,r5,r4
	mvns r5,r4
	#lsls r5,r5,#25
	str r5 , [r6]
	movs r0, #0
	b DelayForOne
	/* for(;;); */
	b .

	/* this should never get executed */
	nop

