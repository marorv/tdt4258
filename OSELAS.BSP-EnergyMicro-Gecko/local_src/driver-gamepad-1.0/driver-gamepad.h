#define DRIVER_NAME "gamepad"
#define DEV_NR_COUNT 1
#define GPIO_EVEN_IRQ_NUMBER 17
#define GPIO_ODD_IRQ_NUMBER 18

struct fasync_struct* async_queue;
struct cdev my_cdev;
struct class* cl;
dev_t devno;


static int gamepad_open(struct inode*, struct file*);
static int gamepad_release(struct inode*, struct file*);
static ssize_t gamepad_read(struct file*, char* __user, size_t, loff_t*);
static ssize_t gamepad_write(struct file*, char* __user, size_t, loff_t*);
static int gamepad_fasync(int fd, struct file* filp, int mode);
static irqreturn_t gpio_interrupt_handler(int, void*, struct pt_regs*);
static void __exit gamepad_cleanup(void);
static int __init gamepad_init(void);
