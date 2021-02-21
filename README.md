# Linux-Kernel-System-Call

This is the implementation of a system call which named as processinfo to the linux kernel.
There are four different usages.

1) Without an argument 
This prints the right usage of the program.

Right Usage:
-all prints some information (process id and its argument/s) about all processes
-p takes process id and prints the details of it
-k takes process id and kill the process

Example: ./processinfo.o

2) "-all"
This lists all the processes and some information about each process.

Example: ./processinfo.o -all

3) "-p processId"
This shows the commandline of the given process.

Example: ./processinfo.o -p 1425

4) "-k processId"
This kills the given process.

Example: ./processinfo.o -k 1425


In syscall_64.tbl, the line	335		64		processinfo		__x64_sys_processinfo is added.

In syscalls.h, the line    asmlinkage long sys_processinfo(char * all_processes, char* process_id, int kill_id, char* commandline, char* argument) is added.
