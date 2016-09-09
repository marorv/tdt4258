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

	.globl  _reset
	.type   _reset, %function
	.thumb_func
_reset:
	//   l o a d  CMU  b a s e   a d d r e s s
	ldr r1, cmu_base_addr
	//   l o a d   c u r r e n t   v a l u e   o f  HFPERCLK  ENABLE
	ldr r2 ,   [ r1 ,  #CMU_HFPERCLKEN0]
	//   s e t   b i t   f o r   GPIO   c l k
	mov r3,  #1
	lsl r3, r3, #CMU_HFPERCLKEN0_GPIO
	orr r2,r2,r3
	//   s t o r e   new   v a l u e
	str r2, [r1,#CMU_HFPERCLKEN0]


	//INIT LED LIGHTS
	//load gpio pa base
	ldr r1, gpio_pa_addr

	//set drive strength to high
	mov r2, #0x02
	str r2, [r2, #GPIO_CTRL]

	//set pins 8-15 to type output (LED)
	mov r2, #0x55555555
	str r2, [r1, #GPIO_MODEH]

	//set GPIO 8-15 to HIGH
	mov r2, #0xff00
	str r2, [r1, #GPIO_DOUT]


	//INIT BUTTONS
	ldr r1, gpio_pc_addr

	mov r2, #0x33333333
	str r2, [r1, #GPIO_MODEL]

	mov r2, #0xff
	str r2, [r1, #GPIO_DOUT]

	ldr r3, [r1, #GPIO_DIN]
	
cmu_base_addr:
	.long CMU_BASE
gpio_pc_addr:
	.long GPIO_PC_BASE
gpio_pa_addr:	
	.long GPIO_PA_BASE
	b .

/////////////////////////////////////////////////////////////////////////////
//
// GPIO handler
// The CPU will jump here when there is a GPIO interrupt
//
/////////////////////////////////////////////////////////////////////////////

    .thumb_func
gpio_handler:  
	b .  // do nothing

/////////////////////////////////////////////////////////////////////////////

    .thumb_func
dummy_handler:  
	b .  // do nothing

