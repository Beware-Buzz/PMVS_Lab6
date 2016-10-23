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
MODULE_AUTHOR("Nagibator 777");
MODULE_VERSION("1");

static struct timer_list my_timer;
static struct kobject *mytimer_kobject;
static int times_left = 0;
static int delay = 5;




static void mytimer_exit(void)
{	
	kobject_put(mytimer_kobject);
	del_timer(&my_timer);
}

module_init(mytimer_init);
module_exit(mytimer_exit);
