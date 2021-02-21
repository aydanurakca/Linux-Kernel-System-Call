#include <stdio.h>
#include <linux/kernel.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
	char *all_processes = (char *)malloc(4000);
	int kill_id;
	char *process_id = malloc(150);
	char *commandline = (char *)malloc(200);
	char *argument = (char *)malloc(10);

	if (argc == 1) // if there is no argument
	{
		printf("\nRight Usage:\n");
		printf("\t-all prints some information (process id and its argument/s) about all processes\n");
		printf("\t-p takes process id and prints the details of it\n");
		printf("\t-k takes process id\n");
	}
	else if (argc == 2 && strcmp(argv[1], "-all") == 0) // if -all argument is given
	{
		argument = argv[1];
		long int amma = syscall(335, all_processes, process_id, kill_id, commandline, argument);
		printf("Process id and the argument list for the processes:\n");
		printf("uid\t\tppid\t\tpid\t\tname\t\tstate\n");

		char *token = strtok(all_processes, ",");
		while (token != NULL)
		{
			printf(" %s\n", token);
			token = strtok(NULL, ",");
		}
	}
	else if (argc == 3 && strcmp(argv[1], "-p") == 0) // if -p argument and process id are given
	{
		argument = argv[1];
		process_id = argv[2];
		long int amma = syscall(335, all_processes, process_id, kill_id, commandline, argument);
		printf("Argument#[%s]-Process Information: \n", argv[2]);
		printf("pid\t\tcommandline\n", process_id, commandline);
		printf("%s\t\t%s\n", process_id, commandline);
	}
	else if (argc == 3 && strcmp(argv[1], "-k") == 0) // if -k argument and process id are given
	{
		argument = argv[1];
		kill_id = atoi(argv[2]);
		long int amma = syscall(335, all_processes, process_id, kill_id, commandline, argument);
		printf("Process %s was killed... ", argv[2]);
	}
}
