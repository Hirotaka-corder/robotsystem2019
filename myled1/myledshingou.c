#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/uaccess.h>
#include <linux/io.h>
#include <linux/delay.h>

MODULE_AUTHOR("Hirotaka Sugiura");
MODULE_DESCRIPTION("driver for LED control");
MODULE_LICENSE("GPL");
MODULE_VERSION("0.0.1");

static dev_t dev;
static struct cdev cdv;
int ZZ;

static struct class *cls = NULL;
static volatile u32 *gpio_baseA = NULL;
static volatile u32 *gpio_baseB = NULL;


static ssize_t led_write(struct file* filp, const char* buf, size_t count, loff_t* pos)
{
	char c;
        if(copy_from_user(&c,buf,sizeof(char)))
		return -EFAULT;	
	int EX = c - '0';
	if(c!='\n')
	{
	        	gpio_baseA[7] = 1 << 23;
			ssleep(EX);
			gpio_baseA[10] = 1 << 23;
	        	gpio_baseB[7] = 1 << 24;
			ssleep(EX);
			gpio_baseB[10] = 1 << 24;
	}

        return 1;
}	

static struct file_operations led_fops = {
	.owner = THIS_MODULE,
	.write = led_write
};


static int __init init_mod(void)//カーネルモジュールの初期化
{
	int retval;

	gpio_baseA = ioremap_nocache(0x3f200000, 0xA0);

	const u32 ledA = 23;
	const u32 indexA = ledA/10;
	const u32 shiftA = (ledA%10)*3;
	const u32 maskA = ~(0x7 << shiftA);
	gpio_baseA[indexA] = (gpio_baseA[indexA] & maskA) | (0x1 << shiftA);


	gpio_baseB = ioremap_nocache(0x3f200000, 0xA0);

	const u32 ledB = 24;
	const u32 indexB = ledB/10;
	const u32 shiftB = (ledB%10)*3;
	const u32 maskB	= ~(0x7 << shiftB);
	gpio_baseB[indexB] = (gpio_baseB[indexB] & maskB) | (0x1 << shiftB);

	retval = alloc_chrdev_region(&dev, 0, 1, "myled");
	if(retval < 0){
		printk(KERN_ERR "alloc_chrdev_region failed.\n");
		return retval;
	}

	printk(KERN_INFO "%s is loaded. major:%d\n",__FILE__,MAJOR(dev));
        cdev_init(&cdv, &led_fops);

        cdv.owner = THIS_MODULE;

	retval = cdev_add(&cdv, dev, 1);
	if(retval < 0){
		printk(KERN_ERR "cdev_add failed. major:%d, minor:%d",MAJOR(dev),MINOR(dev));
		return retval;
	}

	printk(KERN_INFO "%s is loaded.\n",__FILE__);

	cls = class_create(THIS_MODULE,"myled");
	if(IS_ERR(cls)){
		printk(KERN_ERR "class_create failed.");
		return PTR_ERR(cls);
	}
	device_create(cls, NULL, dev, NULL, "myled%d",MINOR(dev));

	return 0;
}

static void __exit cleanup_mod(void)//後始末
{
	cdev_del(&cdv);
	device_destroy(cls, dev);
	class_destroy(cls);
	unregister_chrdev_region(dev, 1);
	printk(KERN_INFO "%s is unloaded. major:%d\n",__FILE__,MAJOR(dev));

	iounmap(gpio_baseA);
	iounmap(gpio_baseB);
}

module_init(init_mod);//マクロで関数を登録
module_exit(cleanup_mod);//同↑
