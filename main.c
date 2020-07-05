#include "stdio.h"
#include "unistd.h"
#include "head.h"
#include "string.h"
#include "sys/wait.h"
#include "stdlib.h"
#include "sys/types.h"
#include "sys/stat.h"
#include "fcntl.h"
#include "errno.h"

int main() {
	char currentPath[80];
	char commend[100];
	char *argv[8] = {0};
	int argc = 0;
	getcwd(currentPath,sizeof(currentPath));
	char pwd[80];
	strcat(pwd,currentPath); // 保存用户主目录
	while(1) {
		printf("%s >", currentPath);
		get_string(commend, sizeof(commend));	
		delete_space(commend);
		write_history(pwd,commend);
		if(strncmp(commend,"exit",4) == 0 ) {
            exit(0);
        }
		split(commend, " ", argv, &argc);
		argv[argc] = NULL;
		if(strcmp(argv[0],"cd") == 0){
	        chdir(argv[1]);
	        getcwd(currentPath,sizeof(currentPath));
	        continue;
	    }
		execute(argv,argc,pwd);
	}	
	return 0;
}
