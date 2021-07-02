#include<linux/init.h>
#include<linux/kernel.h>
#include<linux/module.h>
#include<linux/proc_fs.h>
#include<linux/uaccess.h>

#define BUFFER_SIZE 128
#define PROC_NAME "hello"

static ssize_t proc_read(struct file *file, char *buf, size_t count, loff_t *pos);
static const struct proc_ops hello_proc_ops = {
	.proc_read = proc_read,
};

/* This function is called when the module is loaded */
static int proc_init(void)
{
	/* creates the /proc/hello entry */
	proc_create(PROC_NAME, 0, NULL, &hello_proc_ops);
	printk(KERN_INFO "/proc/%s create\n", PROC_NAME);
	return 0;
}

/* This function is called when the module is removed. */
static void proc_exit(void)
{
	/* remove the /pro/hello entry */
	remove_proc_entry(PROC_NAME, NULL);
}

/* This function is called each time /proc/hello is read */
static ssize_t proc_read(struct file *file, char *buf, size_t count, loff_t *pos)
{
	int rv = 0;
	char buffer[BUFFER_SIZE];
	static int completed = 0;

	if(completed){
		completed = 0;
		return 0;
	}

	completed = 1;

	rv = sprintf(buffer, "Hello World \n");
	
	/* copies kernel space buffer to user space usr_buf */
	copy_to_user(buf, buffer, rv);

	return rv;
}

module_init(proc_init);
module_exit(proc_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Hello Module");
MODULE_AUTHOR("LOYIO");