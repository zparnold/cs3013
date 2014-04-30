//zparnold: Zach Arnold

// We need to define __KERNEL__ and MODULE to be in Kernel space
// If they are defined, undefined them and define them again:
#undef __KERNEL__
#undef MODULE
#define __KERNEL__
#define MODULE
//Kernel modules must include module.h to get module functions
//and kernel.h to define the KERN_INFO variable for
// writing to the syslog. Finally, init.h provides the macros
// for module initialization and destruction (__init and __exit)

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

static int __init hello_init(void) {

// Note the use of "printk" rather than "printf"
// This is important for kernel-space code because it prints
// reliably to the system log, even if the kernel is crashing.
// See "Linux Kernel Development," page 17 & Chapter 18 

printk(KERN_INFO "Hello World!\n");

// We now must return. A value of 0 means that
// the module loaded successfully. A non-zero
// value indicates the module did not load.

return 0;
}

static void __exit hello_cleanup(void) {

// We are not going to do much here other than
// print a syslog message.

printk(KERN_INFO "Cleaning up module.\n");
}

// We have to indicate what functions will be run upon
// module initialization and removal.

module_init(hello_init);
module_exit(hello_cleanup);
