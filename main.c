#include "stdio.h"
#include "unistd.h"
#include "head.h"
#include "string.h"
#include "sys/wait.h"
#include "stdlib.h"

int main() {
	char currentPath[80];
	char commend[100];
	char *argv[8] = {0};
	int argc = 0;
	getcwd(currentPath,sizeof(currentPath));
	printf("%s >", currentPath);
	while(1) {
		get_string(commend, sizeof(commend));	
		commend[strlen(commend)-1] = '\0';	// delete \n in the end	
		split(commend, " ", argv, &argc);
		argv[argc] = NULL;
		printf("长度为%d\n", argc);
printf("%s\n", argv[argc-1]);
		pid_t pid = fork();
		if(pid < 0) {
			printf("error creating child process!\n");
			exit(0);
		}
		if(pid == 0) {
			if(strcmp(argv[argc-1],"&") == 0){
				FILE *fp;     
    				init_daemon();
			}
			execvp(argv[0], argv);
			//exit(1);
		}
		wait(NULL);
			
		printf("%s >", currentPath);
	}	
	return 0;
}
