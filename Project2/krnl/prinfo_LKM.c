/*
 * Sample_LKM.c
 *
 *  Undated on: Jan 26, 2014
 *      Author: Craig Shue
 *      Updated: Hugh C. Lauer
 */

// We need to define __KERNEL__ and MODULE to be in Kernel space
// If they are defined, undefined them and define them again:
#undef __KERNEL__
#undef MODULE

#define __KERNEL__ 
#define MODULE

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/syscalls.h>
#include <linux/sched.h>
#include <linux/cred.h>
#include <linux/prinfo.h>
#include <linux/list.h>
#include <asm/current.h>
#include <asm/uaccess.h>

unsigned long **sys_call_table;

//Functions in sys_getprinfo
long get_state(struct task_struct *taskInfo);
pid_t get_pid2(struct task_struct *taskInfo);
pid_t get_parent_pid(struct task_struct *taskInfo);
pid_t get_youngest_child_pid(struct task_struct *taskInfo);
pid_t get_younger_sibling_pid(struct task_struct *taskInfo);
pid_t get_older_sibling_pid(struct task_struct *taskInfo);
unsigned long get_start_time(struct task_struct *taskInfo);
long get_user_time(struct task_struct *taskInfo);
long get_system_time(struct task_struct *taskInfo);
long get_cutime(struct task_struct *taskInfo);
long get_cstime(struct task_struct *taskInfo);
long get_uid2(struct task_struct *taskInfo);

asmlinkage long sys_getprinfo (struct prinfo *info);

asmlinkage long (*ref_sys_cs3013_syscall1)(void);

asmlinkage long new_sys_cs3013_syscall1(void) {
	printk(KERN_INFO "\"'Hello world?!' More like 'Goodbye, world!' EXTERMINATE!\" -- Dalek");
	return 0;
}	// asmlinkage long new_sys_cs3013_syscall1(void)

asmlinkage long sys_getprinfo(struct prinfo *info){
	
	struct prinfo newInfo;
	
	// Use task_struct to get current process info
	struct task_struct *taskInfo;
	taskInfo = get_current();
	// Store data from task_struct into newInfo
	newInfo.state = get_state(taskInfo);														
	newInfo.pid = get_pid2(taskInfo);									
	newInfo.parent_pid = get_parent_pid(taskInfo);					
	newInfo.youngest_child = get_youngest_child_pid(taskInfo);	
	newInfo.younger_sibling = get_younger_sibling_pid(taskInfo);	
	newInfo.older_sibling = get_older_sibling_pid(taskInfo);		
	newInfo.start_time = get_start_time(taskInfo);					
	newInfo.user_time = get_user_time(taskInfo);						
	newInfo.sys_time = get_system_time(taskInfo);					
	newInfo.cutime = get_cutime(taskInfo);							
	newInfo.cstime = get_cstime(taskInfo);							
	newInfo.uid = get_uid2(taskInfo);									
								

	
	
	/* copy info out of kernel to user */
	if (copy_to_user(info, &newInfo, sizeof(struct prinfo))){
		return -EFAULT;
	}
	/* return 0 if successful */
	return 0;

} 


static unsigned long **find_sys_call_table(void) {
	unsigned long int offset = PAGE_OFFSET;
	unsigned long **sct;

	while (offset < ULLONG_MAX) {
		sct = (unsigned long **)offset;

		if (sct[__NR_close] == (unsigned long *) sys_close) {
			printk(KERN_INFO "Interceptor: Found syscall table at address: 0x%02lX", (unsigned long) sct);
			return sct;
		}

	offset += sizeof(void *);
	}

	return NULL;
}	// static unsigned long **find_sys_call_table(void)


static void disable_page_protection(void) {
	/*
	Control Register 0 (cr0) governs how the CPU operates.

	Bit #16, if set, prevents the CPU from writing to memory marked as
	read only. Well, our system call table meets that description.
	But, we can simply turn off this bit in cr0 to allow us to make
	changes. We read in the current value of the register (32 or 64
	bits wide), and AND that with a value where all bits are 0 except
	the 16th bit (using a negation operation), causing the write_cr0
	value to have the 16th bit cleared (with all other bits staying
	the same. We will thus be able to write to the protected memory.

	It's good to be the kernel!
	*/

	write_cr0 (read_cr0 () & (~ 0x10000));

}	//static void disable_page_protection(void)


static void enable_page_protection(void) {
	/*
	See the above description for cr0. Here, we use an OR to set the
	16th bit to re-enable write protection on the CPU.
	*/

	write_cr0 (read_cr0 () | 0x10000);

}	// static void enable_page_protection(void)


static int __init interceptor_start(void) {
	/* Find the system call table */
	if(!(sys_call_table = find_sys_call_table())) {
		/* Well, that didn't work.
		Cancel the module loading step. */
		return -1;
	}


	/* Store a copy of all the existing functions */
	ref_sys_cs3013_syscall1 = (void *)sys_call_table[__NR_cs3013_syscall1];

	/* Replace the existing system calls */
	disable_page_protection();

	sys_call_table[__NR_cs3013_syscall1] = (unsigned long *)sys_getprinfo;

	enable_page_protection();

	/* And indicate the load was successful */
	printk(KERN_INFO "Loaded interceptor!");

	return 0;
}	// static int __init interceptor_start(void)


static void __exit interceptor_end(void) {
	/* If we don't know what the syscall table is, don't bother. */
	if(!sys_call_table)
		return;

	/* Revert all system calls to what they were before we began. */
	disable_page_protection();
	sys_call_table[__NR_cs3013_syscall1] = (unsigned long *)ref_sys_cs3013_syscall1;
	enable_page_protection();

	printk(KERN_INFO "Unloaded interceptor!");
}	// static void __exit interceptor_end(void)

// Current state of process 
long get_state(struct task_struct *taskInfo){
	printk("Current state of process: %ld\n", taskInfo->state);
	return taskInfo->state;
}

// Process id
pid_t get_pid2(struct task_struct *taskInfo){
	printk("Process id: %d\n", taskInfo->pid);
	return taskInfo->pid;
}

// Process id of parent
pid_t get_parent_pid(struct task_struct *taskInfo){
	printk("Process id of parent: %d\n", taskInfo->parent->pid);
	return taskInfo->parent->pid;
}

// Pid of youngest child
pid_t get_youngest_child_pid(struct task_struct *taskInfo){

struct task_struct *youngest_child;

	// check if taskInfo has any children
	if (&taskInfo->children == NULL)	return-1;
	// look for youngest child
	
	youngest_child = list_entry(taskInfo->children.prev, struct task_struct, children);
	printk("Pid of youngest child: %d\n", youngest_child->pid);
	return youngest_child->pid;
}

// Pid of younger sibling
pid_t get_younger_sibling_pid(struct task_struct *taskInfo){

struct task_struct *younger_sibling;


	// check if taskInfo has any sibling
	if (&taskInfo->sibling == NULL)	return -1;
	// look for the younger sibling
	
	younger_sibling = list_entry(taskInfo->sibling.prev, struct task_struct, sibling);
	printk("Pid of younger sibling: %d\n", younger_sibling->pid);
	return younger_sibling->pid;
}

// Pid of older sibling
pid_t get_older_sibling_pid(struct task_struct *taskInfo){

struct task_struct *older_sibling;

	// check if taskInfo has any sibling
	if (&taskInfo->sibling == NULL)
	{	return -1;
	}
	// look for the older sibling
	else{
	older_sibling = list_entry(taskInfo->sibling.next, struct task_struct, sibling);
	printk("Pid of older sibling: %d\n", older_sibling->pid);
	return older_sibling->pid;
	}
}

// Process start time
unsigned long get_start_time(struct task_struct *taskInfo){
	struct timespec start_time = taskInfo->start_time;
	unsigned long return_time;
	return_time = (long)start_time.tv_sec*1000000 + start_time.tv_nsec/1000; //change time to ms
	printk("Return start time value of: %ld\n", return_time);
	return return_time;
}

// CPU time spent in user mode
long get_user_time(struct task_struct *taskInfo){
	printk("CPU time spent in user mode: %ld\n", taskInfo->utime);
	return taskInfo->utime;
}

// CPU time spend in system mode
long get_system_time(struct task_struct *taskInfo){
	printk("CPU time spend in system mode: %ld\n", taskInfo->stime);
	return taskInfo->stime;
}

// Total user time of childern
long get_cutime(struct task_struct *taskInfo){

long user_time = 0;
	struct task_struct *task;
	struct list_head *list;
	
	// check if taskInfo has any children
	if (&taskInfo->children == NULL)	return 0;
	
	// interate children list to add user time
	list_for_each(list, &taskInfo->children){
		task = list_entry(list, struct task_struct, sibling);
		user_time += get_user_time(task);
	}
	printk("Total user time of childern: %ld\n", user_time);
	return user_time;
}

// Total system time of childern
long get_cstime(struct task_struct *taskInfo){

long system_time = 0;
	struct task_struct *task;
	struct list_head *list;
	
	// check if taskInfo has any children
	if (&taskInfo->children == NULL)	return 0;
	
	// interate children list to add system time
	list_for_each(list, &taskInfo->children){
		task = list_entry(list, struct task_struct, sibling);
		system_time += get_system_time(task);
	}
	printk("Total system time of childern: %ld\n", system_time);
	return system_time;
}

// User id of process owner
long get_uid2(struct task_struct *taskInfo){
	printk("User id of process owner: %d\n", taskInfo->cred->uid);
	return taskInfo->cred->uid;
}


MODULE_LICENSE("GPL");
module_init(interceptor_start);
module_exit(interceptor_end);


