/*
 * This is a demo Linux kernel module.
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/moduleparam.h>
#include <linux/kdev_t.h>
#include <asm/io.h>
#include <asm/uaccess.h>
#include <asm/signal.h>
#include <asm/siginfo.h>
#include <linux/interrupt.h>
#include "efm32gg.h"
#include "driver-gamepad.h"


MODULE_LICENSE ("GPL");  /* programlisence for the module */
module_init(gamepad_init); /*  init-funksjon */
module_exit(gamepad_cleanup); /* exit-funksjon */


/* file pointer */
static struct file_operations gamepad_fops ={
			.owner = THIS_MODULE,
			.read = gamepad_read,
			.write = gamepad_write,
			.open = gamepad_open,
			.release = gamepad_release,
			.fasync = gamepad_fasync,
			};
			
static int gamepad_open(struct inode *inode, struct file *filep){
	printk(KERN_INFO "open");
	return 0;
}

static int gamepad_release(struct inode *inode, struct file *filep){
	printk(KERN_INFO "release");
	return 0;
}

static ssize_t gamepad_read(struct file *filep, char __user *buff, size_t count, loff_t *offp){
	uint32_t buttonstatus = ioread32(GPIO_PC_DIN);
	copy_to_user(buff, &buttonstatus, 1);
	return 1; 
}

static ssize_t gamepad_write(struct file *filep, char __user *buff, size_t count, loff_t *offp){
	printk(KERN_INFO "you cann't write to buttons");
	return 1;
}

/* fasync function */

static int gamepad_fasync(int fd, struct file* filp, int mode) {
	printk(KERN_INFO "Driver:%i \n", SIGIO);
    return fasync_helper(fd, filp, mode, &async_queue);
}


/* handler interrupt*/
irqreturn_t gpio_interrupt_handler(int irq, void* dev_id, struct pt_regs* regs){
	printk(KERN_INFO "interrupt\n");
    iowrite32(ioread32(GPIO_IF), GPIO_IFC); 
    if (async_queue) {
        kill_fasync(&async_queue, SIGIO, POLL_IN);
    }
    return IRQ_HANDLED;
}

/*
 * gamepad_init - function to insert this module into kernel space
 *
 * This is the first of two exported functions to handle inserting this
 * code into a running kernel
 *
 * Returns 0 if successfull, otherwise -1
 */

static int __init gamepad_init(void)
{
	int result;
	result = alloc_chrdev_region(&devno, 0, DEV_NR_COUNT, DRIVER_NAME); /* allocation of the driver number */
	
	/* allocating memory region and mapping */
	if (result < 0){
		printk(KERN_ALERT "Error, coundn't allocate device numbers \n");
		return -1;
	}
	
    if (request_mem_region(GPIO_PC_MODEL, 4, DRIVER_NAME) == NULL ) {
        printk(KERN_ALERT "Error requesting GPIO_PC_MODEL memory region\n");
        return -1;
    }
    if (request_mem_region(GPIO_PC_DOUT, 4, DRIVER_NAME) == NULL ) {
        printk(KERN_ALERT "Error requesting GPIO_PC_DOUT memory region\n");
        return -1;
    }
    if (request_mem_region(GPIO_PC_DIN, 4, DRIVER_NAME) == NULL ) {
        printk(KERN_ALERT "Error requesting GPIO_PC_DIN memory region\n");
        return -1;
    }
    /*setup GPIO*/
    iowrite32(0x33333333, GPIO_PC_MODEL);/*set button pins to input pins*/
    iowrite32(0xff, GPIO_PC_DOUT); /*enable data output for button pins*/
    iowrite32(0x22222222, GPIO_EXTIPSELL); /*set GPIO_EXTIPSELL to 0x22222222 for the button pins*/
    iowrite32(0xff, GPIO_EXTIFALL); /*set GPIO_EXTIFALL TO 0xff to active interrupts on button down*/
    iowrite32(0xff, GPIO_IEN); /* set GPIO_IEN to 0xff to enable interrupt generation */
    iowrite32(0x802, ISER0); /* enable interrupt handling */
    
    /* register the interrupthandler and enables the interrupt */
    request_irq(GPIO_EVEN_IRQ_NUMBER, (irq_handler_t)gpio_interrupt_handler, 0, DRIVER_NAME, &my_cdev);
    request_irq(GPIO_ODD_IRQ_NUMBER, (irq_handler_t)gpio_interrupt_handler, 0, DRIVER_NAME, &my_cdev);
    
    
   /* initialize the cdev structure to the file_oprations structure and passes it to the kernel */
    cdev_init(&my_cdev, &gamepad_fops);
    my_cdev.owner = THIS_MODULE;
    cdev_add(&my_cdev, devno, DEV_NR_COUNT);
    
    /*make the driver visible to User Space */
    cl = class_create(THIS_MODULE, DRIVER_NAME);
    device_create(cl, NULL, devno, NULL, DRIVER_NAME);
    printk(KERN_ALERT "Loading complited \n");
    return 0;

    
}

/*
 * gamepad_cleanup - function to cleanup this module from kernel space
 *
 * This is the second of two exported functions to handle cleanup this
 * code from a running kernel
 */

static void __exit gamepad_cleanup(void)
{
	 printk(KERN_ALERT "unloading gamepad driver module\n");
	 /*free the interrupt handler*/
	 free_irq(GPIO_EVEN_IRQ_NUMBER, &my_cdev);
	 free_irq(GPIO_ODD_IRQ_NUMBER, &my_cdev);
	 
	 /* release memory */
	 release_mem_region(GPIO_PC_MODEL, 1);
	 release_mem_region(GPIO_PC_DOUT, 1);
	 release_mem_region(GPIO_PC_DIN, 1);
	 /* make the driver uvisible to user space */
	 device_destroy(cl, devno);
     class_destroy(cl);
    
    /* uninitialize the cdev structure to the file_oprations structure */
	 cdev_del(&my_cdev);
	 unregister_chrdev_region(devno, DEV_NR_COUNT);
	 
}





