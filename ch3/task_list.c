//  /usr/src/linux-headers-5.13.0-37-generic/include/linux
#include<linux/sched.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/sched/signal.h>
int simple_init(void){
	printk(KERN_INFO "Loading Module\n");
	struct task_struct *task;
	for_each_process(task){
		printk("pid:%d,status:%ld,comm=%s\n",task->pid,task->state,task->comm);
	}
	return 0;
}

void simple_exit(void){
	printk(KERN_INFO "Removing Module\n");
}

module_init(simple_init);
module_exit(simple_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Simple Module");
MODULE_AUTHOR("SGG");