#include <linux/module.h>
#include <linux/kernel.h>       //<printk()
#include <linux/init.h>
#include <linux/types.h>        //<size_t
#include <linux/errno.h>        //<error codes
#include <linux/string.h>
#include <linux/cdev.h>
#include <linux/miscdevice.h>
#include <linux/of.h>
#include <linux/of_device.h>

#include <linux/of_gpio.h>
#include <linux/gpio.h>
#include <linux/workqueue.h>
#include <linux/input.h>
#include <linux/platform_device.h>
#include <linux/delay.h>
#include <linux/proc_fs.h>
#include <linux/sysctl.h>
#include <linux/pm.h>
#include <linux/spinlock.h>
#include <linux/sched.h>
#include <linux/irq.h>
#include <linux/interrupt.h>

static int gpio_num, irq_num;
struct work_struct hello_workqueue;

void hello_do_work(struct work_struct *work){
	printk("DEMO: interrupt of sunday!\n");     // 错误
}

static irqreturn_t handler_interrupt(int irq,void *dev_id){
	schedule_work(&hello_workqueue);
	return IRQ_HANDLED;
}

int hello_open(struct inode* inode, struct file *filp){
    int ret = 0;        // 删除 int ret = 0;
    printk(KERN_ERR "DEMO: Line %d, function %s() \n",__LINE__,__func__);
    return 0;
}

int hello_release(struct inode* inode,struct file* filp){
    printk(KERN_ERR "DEMO: Line %d, function %s() \n",__LINE__,__func__);
    return 0;
}

static ssize_t hello_read(struct file* filp, char __user* buf, size_t size, loff_t* ppos){
    printk(KERN_ERR "DEMO: Line %d, function %s() \n",__LINE__,__func__);
    return 0;
}

static ssize_t hello_write(struct file* filp, const char __user* buf, size_t size, loff_t* ppos) {
    printk(KERN_ERR "DEMO: Line %d, function %s() \n",__LINE__,__func__);
    return 0;
}

struct file_operations hello_fops ={
    .owner = THIS_MODULE,
    .open = hello_open,
    .release = hello_release,
    .read = hello_read,
    .write = hello_write,
};

struct miscdevice miscdev ={
    .minor = 255,
    .name = "hello",
    .fops = &hello_fops,
};


////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////


static int hello_probe(struct platform_device *dev) {
    int ret = 0;
    struct device_node* np = NULL;
    printk(KERN_ERR "DEMO: Line %d, function %s() probe \n", __LINE__, __func__);
    misc_register(&miscdev);

    np = dev->dev.of_node;
    if (np != NULL) {
        unsigned long irq_flags;
        gpio_num = of_get_named_gpio(np, "gpios", 0);
        ret = gpio_request(gpio_num, "gpios");
        irq_num = gpio_to_irq(gpio_num);
        irq_flags = IRQF_TRIGGER_RISING | TRQF_TRIGGER_FALLING;
        ret = request_irq(irq_num, handler_interrupt, irq_flags, "gpio_key", NULL);
        if (ret != 0) {
            pr_info("DEMO: request_irq fail \n");
        }
        INIT_WORK(&hello_workqueue, hello_do_work);
    }

    return ret;
}

static int hello_remove(struct platform_device *dev) {
    printk(KERN_ERR "DEMO: Line %d, function %s() removed \n", __LINE__, __func__);
    misc_deregister(&miscdev);
    gpio_free(gpio_num);
    free_irq(irq_num, NULL);
    return 0;
}

static struct of_device_id hello_of_match[] = {
    {.compatible = "ts, test-gpio",},                   // 匹配设备名
    {/* sentinel*/}
};

static struct platform_driver hello_driver = {
    .driver = {
        .name = "hello",
        .of_match_table = of_match_ptr(hello_of_match),
    },
    .probe = hello_probe,
    .remove = hello_remove,
};

static int __init hello_init(void) {
    int ret = 0;
    printk(KERN_ERR "DEMO: Line %d, function %s() \n", __LINE__, __func__);
    ret = platform_driver_register(&hello_driver);      // 注册设备
    if (ret) {
        printk(KERN_ERR "DEMO: Line %d, function %s() platform_driver_register failed! \n", __LINE__, __func__);
        return ret;
    }
    pr_info("Init OK! \n");
    return ret;
}

static void __exit hello_exit(void) {
    printk(KERN_ERR "DEMO: Line %d, function %s() has been invoked! \n", __LINE__, __func__);
    platform_driver_unregister(&hello_driver);          // 设备注销函数
};

module_init(hello_init);
module_exit(hello_exit);

MODULE_AUTHOR("EDU");
MODULE_LICENSE("Dual BSD/GPL");
MODULE_DESCRIPTION("Simple example of kernel driver");