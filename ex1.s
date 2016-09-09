.syntax unified

.include "efm32gg.s"

/////////////////////////////////////////////////////////////////////////////
//
// Exception vector table
// This table contains addresses for all exception handlers
//
/////////////////////////////////////////////////////////////////////////////

.section .vectors

	.long   stack_top               /* Top of Stack                 */
	.long   _reset                  /* Reset Handler                */
	.long   dummy_handler           /* NMI Handler                  */
	.long   dummy_handler           /* Hard Fault Handler           */
	.long   dummy_handler           /* MPU Fault Handler            */
	.long   dummy_handler           /* Bus Fault Handler            */
	.long   dummy_handler           /* Usage Fault Handler          */
	.long   dummy_handler           /* Reserved                     */
	.long   dummy_handler           /* Reserved                     */
	.long   dummy_handler           /* Reserved                     */
	.long   dummy_handler           /* Reserved                     */
	.long   dummy_handler           /* SVCall Handler               */
	.long   dummy_handler           /* Debug Monitor Handler        */
	.long   dummy_handler           /* Reserved                     */
	.long   dummy_handler           /* PendSV Handler               */
	.long   dummy_handler           /* SysTick Handler              */

	/* External Interrupts */
	.long   dummy_handler
	.long   gpio_handler            /* GPIO even handler */
	.long   dummy_handler
	.long   dummy_handler
	.long   dummy_handler
	.long   dummy_handler
	.long   dummy_handler
	.long   dummy_handler
	.long   dummy_handler
	.long   dummy_handler
	.long   dummy_handler
	.long   gpio_handler            /* GPIO odd handler */
	.long   dummy_handler
	.long   dummy_handler
	.long   dummy_handler
	.long   dummy_handler
	.long   dummy_handler
	.long   dummy_handler
	.long   dummy_handler
	.long   dummy_handler
	.long   dummy_handler
	.long   dummy_handler
	.long   dummy_handler
	.long   dummy_handler
	.long   dummy_handler
	.long   dummy_handler
	.long   dummy_handler
	.long   dummy_handler
	.long   dummy_handler
	.long   dummy_handler
	.long   dummy_handler
	.long   dummy_handler
	.long   dummy_handler
	.long   dummy_handler
	.long   dummy_handler
	.long   dummy_handler
	.long   dummy_handler
	.long   dummy_handler
	.long   dummy_handler

.section .text

/////////////////////////////////////////////////////////////////////////////
//
// Reset handler
// The CPU will start executing here after a reset
//
/////////////////////////////////////////////////////////////////////////////
	
cmu_base_addr:
	.long CMU_BASE
gpio_pc_addr:
	.long GPIO_PC_BASE
gpio_pa_addr:	
	.long GPIO_PA_BASE
iser0:
	.long ISER0
gpio_base:
	.long GPIO_BASE

loop:

//get button values
ldr r3, [r1, #GPIO_DIN]
//left shift r3 8 bits to match the LED bitstring
lsl r3, r3, #8

//set relevant LED lights to on
mov r5, #0x0100
str r3, [r2, #GPIO_DOUT]

bl loop

	.globl  _reset
	.type   _reset, %function
	.thumb_func
_reset:

	ldr r1, cmu_base_addr

	ldr r2 ,   [ r1 ,  #CMU_HFPERCLKEN0]
	mov r3,  #1
	lsl r3, r3, #CMU_HFPERCLKEN0_GPIO
	orr r2,r2,r3
	str r2, [r1,#CMU_HFPERCLKEN0]


	//INIT LED LIGHTS

	//load port a as base
	ldr r1, gpio_pa_addr

	//set gpio drive strength to high
	mov r2, #0x02
	str r2, [r1, #GPIO_CTRL]

	//set pins 8-15 to type output (LED)
	mov r2, #0x55555555
	str r2, [r1, #GPIO_MODEH]


	//INIT BUTTONS
	//load port c as base
	ldr r1, gpio_pc_addr

	//set button pins as type input
	mov r2, #0x33333333
	str r2, [r1, #GPIO_MODEL]

	//set r2 to 0xff and store the value to GPIO_PC_DOUT
	mov r2, #0xff
	str r2, [r1, #GPIO_DOUT]

	//INIT interrupt
	ldr r1, gpio_base

	mov r2, #0x22222222
	str r2, [r1, #GPIO_EXTIPSELL]
	
	mov r2, #0xff
	str r2, [r1, #GPIO_EXTIFALL]

	mov r2, #0xff
	str r2, [r1, #GPIO_EXTIRISE]

	mov r2, #0xff
	str r2, [r1, #GPIO_IEN]

	movw r2, #0x802
	ldr r3, iser0
	str r2, [r3]

	wfi

	b .

/////////////////////////////////////////////////////////////////////////////
//
// GPIO handler
// The CPU will jump here when there is a GPIO interrupt
//
/////////////////////////////////////////////////////////////////////////////

    .thumb_func
gpio_handler:
	ldr r1, gpio_base
	ldr r2, [r1, GPIO_IF]
	str r2, [r1, GPIO_IFC]

	ldr r1, gpio_pc_addr
	ldr r2, gpio_pa_addr

	//get button values
	ldr r3, [r1, #GPIO_DIN]
	//left shift r3 8 bits to match the LED bitstring
	lsl r3, r3, #8

	//set relevant LED lights to on
	mov r5, #0x0100
	str r3, [r2, #GPIO_DOUT]

	bx lr

/////////////////////////////////////////////////////////////////////////////

    .thumb_func
dummy_handler:  
	bx lr  // do nothing

