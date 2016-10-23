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
