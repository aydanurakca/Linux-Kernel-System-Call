#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <linux/init.h>
#include <linux/syscalls.h>
#include <linux/fcntl.h>
#include <linux/string.h>
#include <linux/sched.h>
#include <linux/cred.h>
#include <linux/slab.h>

//syscall_64.tbl	335		64		processinfo		__x64_sys_processinfo 
//syscalls.h    asmlinkage long sys_processinfo(char * all_processes, char* process_id, int kill_id, char* commandline, char* argument)

void list_all_info(char process_info[]) // this function is used to list all the processes.
{
	struct task_struct *task;

	for_each_process (task) { //this block iterates all the processes (tasks).
		char process[100] = "asd";

		char uid_str[10];
		uid_t uid = __kuid_val(task_uid(task));
		snprintf(uid_str, 10, "%d", uid);

		strcpy(process, uid_str);
		strcat(process, "\t\t");

		char ppid[10];
		int value_ppid = task_ppid_nr(task);
		snprintf(ppid, 10, "%d", value_ppid);

		strcat(process, ppid);
		strcat(process, "\t\t");

		char pid[10];
		int value_pid = task->pid;
		snprintf(pid, 10, "%d", value_pid);

		strcat(process, pid);
		strcat(process, "\t\t");
		strcat(process, task->comm);
		strcat(process, "\t\t");

		char state[10];
		int value_state = task->state;
		snprintf(state, 10, "%d", value_state);

		strcat(process, state);
		strcat(process, ",");
		strcat(process_info, process);
	}
}

void display_commandline(char *process_id, char cmdline[]) //this function displays the commandline of given process id.
{
	char path[30];
	char id[10];

	strcpy(id, process_id);
	strcpy(path, "/proc/");
	strcat(path, id);
	strcat(path, "/cmdline");

	struct file *f;
	mm_segment_t fs;

	f = filp_open(path, O_RDONLY, 0); // this block reads the file.

	if (f == NULL) {
		printk(KERN_ALERT "filp_open error!!.\n");
	} else {
		fs = get_fs();
		set_fs(get_ds());
		f->f_op->read(f, cmdline, 200, &f->f_pos);
		set_fs(fs);
	}

	filp_close(f, NULL);
}

void kill_process_by_pid(struct pid *pid) //this function kills the process by given pid.
{
	int kill = kill_pid(pid, SIGKILL, 1);
}

SYSCALL_DEFINE5(processinfo, char *, all_processes, char *, process_id, int, kill_id, char *, commandline, char *, argument)
{
	if (strcmp(argument, "-all") == 0) {
		char process_info[4000];
		list_all_info(process_info);
		copy_to_user(all_processes, process_info, 4000); //this command transfers process_info to userspace as all_processes.
		memset(process_info, 0, 4000);
	} else if (strcmp(argument, "-p") == 0) {
		char cmdline[200];
		display_commandline(process_id, cmdline);
		copy_to_user(commandline, cmdline, 200); // this command transfers cmdline to userspace as commandline.
		memset(cmdline, 0, 200);

	} else if (strcmp(argument, "-k") == 0) {
		struct pid *killpid = find_get_pid(kill_id);
		kill_process_by_pid(killpid);
	}

	return 0;
}
