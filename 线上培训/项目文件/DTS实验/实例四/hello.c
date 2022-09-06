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

int hello_open(struct inode* inode, struct file *filp){
	int ret = 0;
	printk(KERN_ERR "DEMO: dev_np=%p", dev_np);
	if (dev_np != NULL) {
		printk(KERN_ERR "DEMO1: name=%s type=%s full=%s \n", dev_np->name, dev_np->type, dev_np->full_name);
	}
	if (ret == 0) {
		struct device_node* bl_nd = NULL;
		struct property* bl_prop = NULL;
		u32 value32 = 0;
		bl_nd = of_find_node_by_path("/demo_test2");
		if (bl_nd != NULL) {
			printk(KERN_ERR "DEMO31:/demo_test2 = %p\n", bl_nd);
			bl_prop = of_find_property(bl_nd, "compatible", NULL);
			if (bl_prop != NULL) {
				printk(KERN_ERR "DEMO31: name=%s compatible=%s \n", bl_nd->name, (char*)bl_prop->value);
			}
			ret = of_property_read_u32(bl_nd, "value32", &value32);
			printk(KERN_ERR "DEMO31: of_property_read_u32:%d, ret=%d \n", value32, ret);
		}
	}
	return 0;
}

int hello_release(struct inode* inode, struct file* filp){
	printk(KERN_ERR "DEMO:Line %d, function %s()\n", __LINE__ , __func__);
	return 0;
}

static ssize_t hello_read(struct file *filp, char __user* buf, size_t size, loff_t* ppos) {
	printk(KERN_ERR "DEMO:Line %d, function %s()\n", __LINE__, __func__);
	return 0;
}

static ssize_t hello_write(struct file *filp,const char __user* buf, size_t size, loff_t* ppos) {
	printk(KERN_ERR "DEMO:Line %d, function %s()\n", __LINE__, __func__);
	return 0;
}

struct file_operations hello_fops = {
	.owner = THIS_MODULE,
	.open = hello_open,
	.release = hello_release,
	.read = hello_read,
	.write = hello_write,
};

struct miscdevice miscdev = {
	.minor = 255,
	.name = "hello",
	.fops = &hello_fops,
};

static int hello_probe(struct platform_device *dev) {
	int ret = 0;
	struct device_node* np = NULL;
	printk(KERN_ERR "DEMO:Line %d, function %s() probe \n", __LINE__, __func__);
	misc_register(&miscdev);

	np = dev->dev.of_node;
	if (np == NULL) {
		pro_info("DEMO:of_node is NULL\n");
		return 0;
	}
	dev_np = np;

	return ret;
}

static int hello_remove(struct platform_device *dev) {
	printk(KERN_ERR "DEMO:Line %d, function %s() removed \n", __LINE__, __func__);
	misc_deregister(&miscdev);
	return 0;
}
///////////////////////////////////////////////////////////////////////
static struct of_device_id hello_of_match[] = {
	{.compatible = "ts.hello1",},
	{/*semtinel*/}
};

static struct paltform_driver hello_driver = {
	.driver = {
		.name = "hello",
		.of_match_table = of_match_ptr(hello_of_match),
	},
	.probe = hello_probe,
	.remove = hello_remove,
};

static int__init hello_init(void) {
	int ret = 0;
	printk(KERN_ERR "DEMO:Line %d, function %s() \n", __LINE__, __func__);
	ret = platform_driver_register(&hello_driver);
	if (ret) {
		printk(KERN_ERR "DEMO:Line %d, function %s() failed \n", __LINE__, __func__);
		return ret;
	}
	pr_info("Init OK!\n");
	return ret;
}

static void__exit hello_exit(void) {
	printk(KERN_ERR "DEMO:Line %d, function %s() has been invoked! \n", __LINE__, __func__);
	platform_driver_unregister(&hello_driver);
};

module_init(hello_init);
module_exit(hello_exit);
MODULE_AUTHOR("EDU");
MODULE_LICENSE("Dual BSD/GPL");
MODULE_DESCRIPTION("Simple example of kernel driver");
