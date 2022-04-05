//  /usr/src/linux-headers-5.13.0-37-generic/include/
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include<linux/sched.h>
#include <linux/sched/signal.h>
#include <linux/list.h>


void DFS(struct task_struct *task){
	printk("pid:%d,status:%ld,comm=%s\n",task->pid,task->state,task->comm);
	struct task_struct *p;
    struct list_head *list;
	list_for_each(list,&task->children){
		p=list_entry(list,struct task_struct, sibling);
		DFS(p);
	}
}

int simple_init(void){
	printk(KERN_INFO "Loading Module\n");
	DFS(&init_task);
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