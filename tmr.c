#include <linux/module.h> 
#include <linux/printk.h> 
#include <linux/kobject.h> 
#include <linux/sysfs.h> 
#include <linux/init.h> 
#include <linux/fs.h> 
#include <linux/string.h>
#include <linux/kmod.h>
#include <linux/timer.h>
#include <linux/jiffies.h>
#include <asm/param.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Nagibator 7777");
MODULE_VERSION("1");

static struct timer_list my_timer;
static struct kobject *mytimer_kobject;
static int times_left = 0;
static int delay = 5;


static void print_hello(unsigned long data)
{
	if (times_left > 0) {
		printk(KERN_INFO "Hello, world!\n");
		times_left--;
	} 	
	mod_timer(&my_timer, jiffies + delay * HZ);
}

static ssize_t mytimer_read(struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{
	return sprintf(buf, "%d\n", times_left);
}

static ssize_t mytimer_write(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count)
{
	sscanf(buf, "%du", &times_left);
	return count;
}

static struct kobj_attribute times_left_attribute =__ATTR(times_left, 0777, mytimer_read, mytimer_write);

static int mytimer_init(void)
{
	int error = 0;	
	mytimer_kobject = kobject_create_and_add("mytimer", kernel_kobj);
	if (!mytimer_kobject) {
		return -ENOMEM;
	}
	error = sysfs_create_file(mytimer_kobject, &times_left_attribute.attr);
	if (error) {
		printk(KERN_INFO "failed to create the times_left file in /sys/kernel/mytimer \n");
		return error;
	}
	init_timer_on_stack(&my_timer);
	my_timer.expires = jiffies + delay * HZ;
	my_timer.data = 0;
	my_timer.function = print_hello;
	add_timer(&my_timer);
	return 0;
}

static void mytimer_exit(void)
{	
	kobject_put(mytimer_kobject);
	del_timer(&my_timer);
}

module_init(mytimer_init);
module_exit(mytimer_exit);
