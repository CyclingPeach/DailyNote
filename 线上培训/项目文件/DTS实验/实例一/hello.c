#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/version.h>
#include <linux/fs.h>
#include <linux/fcntl.h>
#include <linux/ctype.h>
#include <linux/cdev.h>
#include <linux/slab.h>
#include <linux/vmalloc.h>
#include <linux/pagemap.h>
#include <linux/errno.h>
#include <linux/types.h>

#include <linux/platform_device.h>

#include <linux/of.h>
#include <linux/of_device.h>

#define BUF_SIZE 256

#define DEMO_MAJOR 236
#define DEMO_MINOR 0

#define COMMAND_A 'a'
#define COMMAND_B 'b'

struct demo_dev{
	struct cdev cdev;
};

static struct demo_dev *demo_devices;
static unsigned char demo_count = 0;
static u8 demo_buf[BUF_SIZE];

static struct device_node* dev_np = NULL;
static int probe_cnt = 0;
static const char *str = NULL;
static u8 value = 0;
static u16 value16 = 0;
static u32 value32 = 0;
static u32 arraydata[5] = {0,0,0,0,0};

static ssize_t demo_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos)
{
	ssize_t retval = 0;
	int ret = 0;
	loff_t pos = *f_pos;
	printk(KERN_ERR "DEMO:Line %d, function %s() has been invoked!\n", __LINE__, __func__);
	if (pos > BUF_SIZE) {
		printk(KERN_ERR "DEMO: Line %d, pos is overflow!\n", __LINE__);
		count = -EINVAL;
		goto out;
	}
	if (count >(BUF_SIZE - pos)) {
		count = BUF_SIZE - pos;
	}
	pos += count;
	if (copy_to_user(buf, demo_buf + *f_pos, count)) {
		count = -EFAULT;
		goto out;
	}
	*f_pos = pos;
	retval = count;
	
	printk(KERN_ERR "DEMO: dev_np=%p, probe_cnt=%d", dev_np, probe_cnt);
	if (dev_np != NULL) {
		printk(KERN_ERR "DEMO1: name=%s type=%s full=%s \n", dev_np->name, dev_np->type, dev_np->full_name);
		ret = of_property_read_string(dev_np, "status", &str);
		printk(KERN_ERR "DEMO1: of_property_read_string status: \"%s\", ret=%d \n",str, ret);
		ret = of_property_read_u8(dev_np, "value", &value);
		printk(KERN_ERR "DEMO1: of_property_read_u8 value: %d, ret=%d \n", value, ret);
		ret = of_property_read_u16(dev_np, "value16", &value16);
		printk(KERN_ERR "DEMO1: of_property_read_u16 value16: %d, ret=%d \n", value16, ret);
		ret = of_property_read_u32(dev_np, "value32", &value32);
		printk(KERN_ERR "DEMO1: of_property_read_u32: %d, ret=%d \n", value32, ret);
		ret = of_property_read_u32_array(dev_np, "value32array", arraydata, ARRAY_SIZE(arraydata));
		printk(KERN_ERR "DEMO1: of_property_read_u32_array array: %d %d %d, ret=%d \n",arraydata[0], arraydata[1], arraydata[2], ret);
	}

out:
	return retval;
}

static ssize_t demo_write(struct file *filp, const char __user *buf, size_t count, loff_t *f_pos)
{
	ssize_t retval = 0;
	loff_t pos = *f_pos;
	printk(KERN_ERR "DEMO: Line %d, function %s() has been invoked!\n", __LINE__, __func__);
	if(pos > BUF_SIZE){
		printk(KERN_ERR "DEMO: Line %d, pos is overflow!\n", __LINE__);
		count = -EINVAL;
		goto out;
	}
	if(count > (BUF_SIZE - pos)){
		count = BUF_SIZE - pos;
	}
	pos += count;
	if(copy_from_user(demo_buf + *f_pos, buf, count)){
		count = -EFAULT;
		goto out;
	}
	*f_pos = pos;
	retval = count;
out:
	return retval;
}

static int demo_open(struct inode *inode,struct file *filp)
{
	struct demo_dev *dev;
	printk(KERN_ERR "DEMO: Line %d,function %s()has been invoked!\n",__LINE__,__func__);
	if(demo_count >0)
	{
		return -EINVAL;
	}
	demo_count++;
	dev = container_of(inode->i_cdev,struct demo_dev,cdev);
	filp->private_data = dev;
	return 0;
}

static int demo_release(struct inode *inode,struct file *filp)
{
	printk(KERN_ERR "DEMO: Line %d,function %s() has been invoked!\n",__LINE__,__func__);
	demo_count--;
	return 0;
}

static struct file_operations demo_fops = {
	.owner	=THIS_MODULE,
	.read	=demo_read,
	.write	=demo_write,
	.open	=demo_open,
	.release=demo_release,
};

static int hello_probe(struct platform_device *dev){
       int ret = 0;
       struct device_node* np = dev->dev.of_node;
       printk(KERN_ERR "DEMO:Line %d,function %s() probe %p \n",__LINE__,__func__,np);
       probe_cnt += 1;
       if(np == NULL){
       pr_info("DEMO:of_node is NULL\n");
       return 0;
}
       dev_np = np;
       ret = of_property_read_string(np,"status",&str);
       printk(KERN_ERR "DEMO:of_property_read_string status:\"%s\",ret=%d \n",str,ret);
       ret = of_property_read_u8(np,"value",&value);
       printk(KERN_ERR "DEMO:of_property_read_u8 value: %d,ret=%d \n",value,ret);
       return ret;
}

static int hello_remove(struct platform_device *dev)
{
	printk(KERN_ERR "DEMO:Line %d, function %s() removed\n",__LINE__,__func__);
	return 0;
}

static struct of_device_id hello_of_match[]=
{
	{.compatible = "ts,hello",},
	{/*sentinel*/}
};
MODULE_DEVICE_TABLE(of,hello_of_match);

static struct  platform_driver hello_driver=
{
	.driver ={
		.name = "hello",
		.owner = THIS_MODULE,
		.of_match_table = hello_of_match,
	},
	.probe = hello_probe,
	.remove = hello_remove,
};

static int __init hello_init(void)
{
	int retval;
	dev_t devno = 0;
	printk(KERN_ERR "DEMO: Line %d,function %s() has been invoked!\n",__LINE__,__func__);

	devno = MKDEV(DEMO_MAJOR,DEMO_MINOR);
	printk(KERN_ERR "DEMO: Line %d,major = %d, minor = %d\n",__LINE__,MAJOR(devno),MINOR(devno));

	retval = register_chrdev_region(devno,1,"demo_test");
	if(retval)
	{
		printk(KERN_ERR "DEMO: Line %d,faild to register char_dev demo_test!\n",__LINE__);
		goto out;
	}

        demo_devices = kmalloc(sizeof(struct demo_dev),GFP_KERNEL);
	if(!demo_devices)
	{
		retval = -ENOMEM;
		goto error_cdev;
	}
	memset(demo_devices,0,sizeof(struct demo_dev));
	cdev_init(&demo_devices->cdev,&demo_fops);
	demo_devices->cdev.owner = THIS_MODULE;
	demo_devices->cdev.ops = &demo_fops;

	retval = cdev_add(&demo_devices->cdev,devno,1);
	if(retval)
	{
		printk(KERN_ERR "DEMO: Line %d,faild to add char_dev demo_test!\n",__LINE__);
		goto error_cdev;
	}

	retval = platform_driver_register(&hello_driver);
	if(retval)
	{
		printk(KERN_ERR "DEMO: Line %d,function %s() platform_driver_register failed!\n",__LINE__,__func__);
                goto error_cdev;
	}
out:
	return retval;
error_cdev:
	if(demo_devices)
	{
		cdev_del(&demo_devices->cdev);
		kfree(demo_devices);
	}
	unregister_chrdev_region(devno,1);
	return retval;
}

static void __exit hello_exit(void)
{
	dev_t devno = MKDEV(DEMO_MAJOR, DEMO_MINOR);
	printk(KERN_ERR "DEMO: Line %d,function %s() has been invoked!\n", __LINE__, __func__);
	if(demo_devices){
		cdev_del(&demo_devices->cdev);
		kfree(demo_devices);
	}
	unregister_chrdev_region(devno, 1);
	platform_driver_unregister(&hello_driver);
}

module_init(hello_init);
module_exit(hello_exit);

MODULE_AUTHOR("EDU");
MODULE_LICENSE("Dual BSD/GPL");
MODULE_DESCRIPTION("Kernel module");
MODULE_ALIAS("platform:hello");






















