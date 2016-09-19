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
scr_addr:
	.long SCR


	.globl  _reset
	.type   _reset, %function
	.thumb_func
_reset:

	//INIT THE GPIO CLOCK

	ldr r1, cmu_base_addr //load cmu base to r1

	ldr r2 ,   [ r1 ,  #CMU_HFPERCLKEN0] //load r2 with the current value of CMU_HFPERCLKEN0
	mov r3,  #1 //set bit to r3
	lsl r3, r3, #CMU_HFPERCLKEN0_GPIO //do logical shift left on r2, put it in r3
	orr r2,r2,r3 //do orr operation to preserve all other bits currently on CMU_HFPERCLKEN0
	str r2, [r1,#CMU_HFPERCLKEN0] //write the new value to CMU_HFPERCLKEN0


	//INIT LED LIGHTS

	ldr r1, gpio_pa_addr //load port a as base (base of the LEDs)

	//set gpio drive strength to high
	mov r2, #0x02 //set value for high to r2
	str r2, [r1, #GPIO_CTRL] //store the r2 value to GPIO_CTRL

	//set pins 8-15 to type output (LED)
	mov r2, #0x55555555 //set r2 to value of type output
	str r2, [r1, #GPIO_MODEH] //write r2 value to LED GPIO_MODEH register


	//INIT BUTTONS
	//load port c as base (base of the buttons)
	ldr r1, gpio_pc_addr

	//set button pins as type input
	mov r2, #0x33333333 //write the value that indicates input to r2
	str r2, [r1, #GPIO_MODEL] //store the r2 value to the button GPIO_MODEL

	//set r2 to 0xff and store the value to GPIO_PC_DOUT
	mov r2, #0xff //set value of r2 to ff
	str r2, [r1, #GPIO_DOUT] //store the value of r2 to button GPIO_DOUT

	//INIT interrupt
	ldr r1, gpio_base

	mov r2, #0x22222222
	str r2, [r1, #GPIO_EXTIPSELL] 
	
	//enable gpio interrupt to happen on both button fall and rise
	mov r2, #0xff
	str r2, [r1, #GPIO_EXTIFALL] //store ff to GPIO_EXTIFALL to enable interrupt on button fall
	str r2, [r1, #GPIO_EXTIRISE] //store ff to GPIO_EXTIFALL to enable interrupt on button rise
	str r2, [r1, #GPIO_IEN] //enable gpio interrupts

	movw r2, #0x802 //set lower bits of r2 to 0x802
	ldr r3, iser0 //load r3 with the address of NVIC ISER0
	str r2, [r3] //store the 0x802 value to the ISER0 address.

	wfi //wait for interrupt, enabling sleep mode

	b deepsleep //branch to deepsleep label

	.thumb_func
deepsleep:
	ldr r1, scr_addr //load the state address into r1
	mov r2, #6 //set r2 register to value for deep sleep state
	str r2, [r1] //store the deep sleep value to the SCR address to initiate deep sleep modus

	bx lr //jump back to wfi

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
	mov r5, #0xff00
	str r5, [r2, #GPIO_DOUT]

	bx lr

/////////////////////////////////////////////////////////////////////////////

    .thumb_func
dummy_handler:  
	bx lr  // do nothing

