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

int i=0;
int j=0;

int main() {
	char currentPath[80];
	char commend[100];
	char *argv[8] = {0};
	char *argv1[8] = {0};
    char *argv2[8] = {0};
	int argc = 0;
	getcwd(currentPath,sizeof(currentPath));
	printf("%s >", currentPath);
	char *pwd = currentPath;
	while(1) {
		get_string(commend, sizeof(commend));	
		delete_space(commend);
		write_history(pwd,commend);
		if(strncmp(commend,"exit",4) == 0 ) {
            exit(0);
        }
		split(commend, " ", argv, &argc);
		argv[argc] = NULL;
		if(strncmp(argv[0],"history",7)==0){
			if (argv[1] == NULL) get_history(pwd,200); else {
				char tmp_number = *(argv[1]);
				int tmp_n = tmp_number - '0';
				get_history(pwd,tmp_n);
			}
			printf("%s >", currentPath);
			continue;
		}

		int pipeHave = 0; // Does it contain pipes
		int redirectHave = 0; // Does it contain redirects
		char* file; // 重定向所指向的文件

		for(i = 0; i < argc ;++i){
            if(strcmp(argv[i], ">") == 0){  // 输出重定向
            	redirectHave = 1;
                file = argv[i+1];
                argv[i] = NULL;
                // int fd = open(file,O_WRONLY | O_RDONLY,0666);//打开文件
                // close(1);//关闭文件描述符1
                // dup(fd);//将打开的文件描述符复制过来
                break;
            } else if (strcmp(argv[i], "|") == 0){  // 含有管道
            	pipeHave = 1;
            	
            	for(j = 0;j < i;j ++)
            		argv1[j] = argv[j];
       			for(j = i+1;j < argc;j ++)
       				argv2[j-(i+1)] = argv[j];

       			break;
            }
        }


        if(!pipeHave){  // 如果不含有管道
			if(strcmp(argv[0],"cd") == 0){
				chdir(argv[1]);
				getcwd(currentPath,sizeof(currentPath));
			}
			else{
				pid_t pid = fork();
				if(pid < 0) {
					printf("error creating child process!\n");
					exit(0);
				}
				if(pid == 0) {
					if(redirectHave){
						int fd = open(file,O_WRONLY | O_RDONLY,0666);//打开文件
			        close(1);//关闭文件描述符1
			        dup(fd);//将打开的文件描述符复制过来
					}
					if(strcmp(argv[argc-1],"&") == 0){
						argv[argc-1] = NULL;
						background(argv[0], argv);
					}else
						if(execvp(argv[0], argv)==-1)
							printf("%s\n",strerror(errno));
					//exit(1);
				}
		}
			wait(NULL);
        } else {  // 如果含有管道
        	int pipe_fd[2];
		    int status;
		    pipe(pipe_fd);
		    pid_t child1,child2;
		    if ((child1=fork())!=0){ //父进程
		        if ( (child2 = fork()) == 0 ){ //子进程 
		            close ( pipe_fd[1] );
		            close ( fileno ( stdin ) );
		            dup2 ( pipe_fd[0] , fileno(stdin));
		            close ( pipe_fd[0] );
		            execvp ( argv2[0] , argv2 );
		        } else  {
		            close ( pipe_fd[0]);
		            close ( pipe_fd[1]);
		            waitpid ( child2 , &status , 0 );
		        }
		        waitpid ( child1 , &status , 0 );
		    } else {
		        printf ( "subshell 3cmd %d\n" , getpid() );
		        close ( pipe_fd[0] );
		        close ( fileno(stdout));
		        dup2 ( pipe_fd[1] , fileno(stdout));

		        close ( pipe_fd[1] );
		        execvp ( argv1[0] , argv1 );
		    }
        }
		printf("%s >", currentPath);
	}	
	return 0;
}
