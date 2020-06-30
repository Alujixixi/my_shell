#include <stdio.h>
#include <string.h>
#include "head.h"
#include "unistd.h"
#include <stdlib.h>
#include <sys/wait.h>

int main() {
	char s[100];
	char *argv[8] = {0};
	int argc = 0;
	while(1) {
		get_string(s,sizeof(s));
		s[strlen(s)-1] = '\0';
	      	split(s, " ", argv, &argc);
		printf("argc = %d\n", argc);
       		int i = 0;
		argv[argc] = NULL;
 		for(i = 0; i < argc; i++)
			printf("%ld\n", sizeof(argv[i]));
		printf("before execvp\n");
		pid_t pid = fork();
		if(pid == 0) {
			execvp(argv[0], argv);
			exit(1);
		}
		wait(NULL);
		printf("after execvp\n");
	}
	return 0;
}	
