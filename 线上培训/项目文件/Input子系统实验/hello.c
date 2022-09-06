#include<linux/init.h>
#include<linux/module.h>
#include<linux/miscdevice.h>
#include<linux/fs.h>
#include<linux/input.h>

static struct input_dev *key_input;

int hello_opoen(struct inode* inode, struct file *filp) {
    printk(KERN_ERR "DEMO: Line %d, function %s()\n", __LINE__, __func__);
    return 0;
}

int hello_release(struct inode* inode, struct file* filp) {
    printk(KERN_ERR "DEMO:Line %d, function %s() \n", __LINE__, __func__);
    return 0;
}

static ssize_t hello_read(struct file* filp, char __user* buf, size_t, loff_t* ppos) {
    printk(KERN_ERR "DEMO:Line %d, function %s() \n", __LINE__, __func__);
    input_report_key(key_input, KEY_B, 1);
    input_sync(key_input);
    input_report_key(key_input, KEY_B, 0);
    input_sync(key_input);
    return 0;
}

static ssize_t hello_write(struct file* filp, const char __user*buf, size_t size, loff_t* ppos) {
    printk(KERN_ERR "DEMO:Line %d, function %s() \n", __LINE__, __func__);
    return 0;
}

struct file_operations misc_fops = {
    .owner = THIS_MODULE,
    .open = hello_open, 
    .release = hello_release,
    .read = hello_read,
    .write = hello_write,
};

struct miscdevice misc_dev = {
    .minor = MISC_DYNAMIC_MINOR,
    .name = "hello_misc",
    .fops = &misc_fops,
};

static int misc_init(void) {
    int ret;
    ret = misc_register(&misc_dev);
    if (ret < 0) {
        printk("DEMO: register is error\r\n");
        return -1;
    }
    /*input 子系统相关初始化*/
    key_input = devm_input_allocate_device(misc_dev.this_device);
    if (!key_input) {
        printk("DEMO: fail devm_input_allocate_device\r\n");
        misc_deregister(&misc_dev);
        return -ENOMEM;
    }

    /* 添加 input 子系统响应的事件*/
    input_set_capability(key_input, EV_KEY, KEY_B);

    /* 注册 input 子系统*/
    ret = input_register_device(key_input);
    if (ret) {
        printk("DEMO:Fail input_register_device\r\n");
        input_free_device(key_input);
        return ret;
    }

    printk("DEMO: misc register is succeed\r\n");
    return 0;
}

static void misc_exit(void) {
    input_unregister_device(key_input);
    misc_deregister(&misc_dev);
    printk("DEMO:misc deregister!\r\n");
}

module_init(misc_init);
module_exit(misc_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("EDU");