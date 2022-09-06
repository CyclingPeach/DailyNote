#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/types.h>
#include <linux/errno.h>
#include <linux/string.h>
#include <linux/cdev.h>
#include <linux/miscdevice.h>
#include <linux/of.h>
#include <linux/of_device.h>



static struct device_node* dev_np = NULL;
const char *str = NULL;
u8 value = 0;

int hello_open(struct inode* inode, struct file *filp) {
	int ret = 0;
	printk(KERN_ERR "DEMO: Line %d, function %s() \n", __LINE__,__func__);
	printk(KERN_ERR "DEMO: dev_np=%p", dev_np);
	if (dev_np != NULL) {
		printk(KERN_ERR "DEMO1: name=%s type%s full=%s \n", dev_np->name, dev_np->type, dev_np->full_name);
		ret = of_property_read_string(dev_np, "status", &str);
		printk(KERN_ERR "DEMO1: of_property_read_string status: \"%s\", ret=%d \n", str, ret);
		ret = of_property_read_u8(dev_np, "value", &value);
		printk(KERN_ERR "DEMO1: of_property_read_u8 value:%d, ret=%d \n", value, ret);
	}
	if (ret == 0) {
		struct device_node * bl_nd = NULL;
		struct property * bl_prop = NULL;
		bl_nd = of_find_node_by_path("/demo_test1");
		if (bl_nd != NULL) {
			printk(KERN_ERR "DEMO31: /demo_test1=%p \n", bl_nd);
			bl_prop = of_find_property(bl_nd, "compatible", NULL);
			if (bl_prop != NULL) {
				printk(KERN_ERR "DEMO31: name=%s compatible=%s \n", bl_nd->name, (char*)bl_prop->value);
			}
		}
	}
	return 0;
}

int hello_release(struct inode* inode, struct file* filp) {
	printk(KERN_ERR "DEMO: Line %d, function %s() \n", __LINE__,__func__);
	return 0;
}

static ssize_t hello_read(struct file* filp, char __user* buf, size_t, size, loff_t* ppos) {
	printk(KERN_ERR "DEMO: Line %d, function %s() \n", __LINE__, __func__);
	return 0;
}

static ssize_t hello_write(struct file* filp, const char __user* buf, size_t size, loff_t* ppos) {
	printk(KERN_ERR "DEMO: Line %d, function %s() \n", __LINE__, __func__);
	return 0;
}

static struct file_operations hello_fops = {
	.owner	= THIS_MODULE,
	.open	= hello_open,
	.release = hello_release,
	.read	= hello_read,
	.write	=hello_write,
};

struct miscdevice miscdev = {
	.minor = 255,
	.name = "hello",
	.fops = &hello_fops,
};

static int hello_probe(struct platform_device *dev){
	int ret = 0;
	struct device_node* np = dev->dev.of_node;
	printk(KERN_ERR "DEMO:Line %d,function %s() probe\n",__LINE__,__func__);
	if(np == NULL){
		pr_info("DEMO:of_node is NULL\n");
		return 0;
	}
	dev_np = np;
	ret = of_property_read_string(np, "status", &str);
	printk(KERN_ERR "DEMO: of_property_read_string status: \"%s\", ret=%d \n", str, ret);

	misc_register(&miscdev);	// 注册杂项设备
	return ret;
}


static int hello_remove(struct platform_device *dev)
{
	printk(KERN_ERR "DEMO:Line %d, function %s() removed\n",__LINE__,__func__);
	misc_deregister(&miscdev);
	return 0;
}

static struct of_device_id hello_of_match[]=
{
	{.compatible = "ts,hello",},	// 匹配设备名
	{/*sentinel*/}
};

static struct  platform_driver hello_driver=
{
	.driver ={
		.name = "hello",
		.of_match_table = of_match_ptr(hello_of_match),
	},
	.probe = hello_probe,
	.remove = hello_remove,
};

static int __init hello_init(void)
{
	int ret = 0;
	printk(KERN_ERR "DEMO: Line %d, function %s() \n", __LINE__, __func__);
	ret = platform_driver_register(&hello_driver);	// 注册设备
	if(ret) {
		printk(KERN_ERR "DEMO: Line %d, function %s() platform_driver_register failed!\n", __LINE__, __func__);
		return ret;
	}
	pr_info("lnit OK!\n");
	return ret;
}

static void __exit hello_exit(void)
{
	printk(KERN_ERR "DEMO: Line %d,function %s() has been invoked!\n", __LINE__, __func__);
	platform_driver_unregister(&hello_driver);		// 设备注销函数
}


module_init(hello_init);
module_exit(hello_exit);

MODULE_AUTHOR("EDU");
MODULE_LICENSE("Dual BSD/GPL");
MODULE_DESCRIPTION("Simple example of kernel driver");





















