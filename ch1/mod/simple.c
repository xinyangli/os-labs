#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>

int simple_init(void){
	printk(KERN_INFO "Loading Module");
	printk(KERN_CONT "Loading Module2\n");
	return 0;
}

void simple_exit(void){
	printk(KERN_INFO "Removing Module\n");
}

module_init(simple_init);
module_exit(simple_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("SImple Module");
MODULE_AUTHOR("SGG");


