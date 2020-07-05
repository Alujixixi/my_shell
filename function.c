#include "stdlib.h"
#include "string.h"
#include "head.h"
#include <stdio.h>
#include <ctype.h>
#include <unistd.h>
#include <signal.h>
#include <sys/param.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include "errno.h"
#include "fcntl.h"
#include <sys/wait.h>

char pathvar[100];
char *system_paths[20] = {0};
int path_cnt = -1;

void init_pathvar() {
	strcpy(pathvar, getenv("PATH"));
	split(pathvar, ":", system_paths, &path_cnt);
	return;
}

int scanfile(char *cmd) {
	if(path_cnt < 0)
			init_pathvar();
	int i;
	for( i = 0; i < path_cnt; i++) {
		char fp[100];
		char cmdstr[10];
		strcpy(fp, system_paths[i]);
		strcpy(cmdstr, cmd);
		strcat(strcat(fp, "/"), cmdstr);
		if(access(fp, 0) == 0) {
			return 1;
		}
	}
	return 0;
}

void split(char *src,const char *separator,char **dest,int *num) {
     char *pNext;
     int count = 0;
     if (src == NULL || strlen(src) == 0) //如果传入的地址为空或长度为0，直接终止 
        return;
     if (separator == NULL || strlen(separator) == 0) //如未指定分割的字符串，直接终止 
        return;
     pNext = (char *)strtok(src,separator); //必须使用(char *)进行强制类型转换(虽然不写有的编译器中不会出现指针错误)
     while(pNext != NULL) {
          *dest++ = pNext;
          ++count;
         pNext = (char *)strtok(NULL,separator);  //必须使用(char *)进行强制类型转换
    }
    *num = count;

}

void delete_space(char *string) {  
    	int i = 0;
	int j = 0;  
	while(string[j] == ' ')
		j++;
    	while(string[j]!='\0'){  
        	if(string[j]!=' '||string[j+1]!=' '){  
            		string[i]=string[j];  
           		i++;  
          	  	j++;  
        	}  else {  
            		string[i]=string[j+1];  
            		j++;  
        	}  
    	}
	if(i>=1 && isspace(string[i-1]))
		string[i-1] = '\0';
	else  
    		string[i]='\0';  
}	 

void get_string(char *s, int size) {
	fgets(s, size, stdin);
}

void background(char *cod, char **coms) {   //后台运行
	pid_t pid = fork();
	if(pid < 0) {
			printf("error creating child process!\n");
			exit(0);
	}
	if(pid == 0) {
			if(execvp(cod, coms)==-1) printf("%s\n",strerror(errno));
			exit(0);
	}
	if(pid > 0) {
			printf("The background proccess is %d\n", pid);
			
			kill(getpid(), SIGTERM);
	}
}

void write_history(char *pwd,char command[100]) // Send commands here
{
    time_t rawtime;
    struct tm* info;
    char buffer[80];
    char history_path[30] = "";
    strcat(history_path,pwd);
    strcat(history_path,"/.bash_history");

    FILE* fp = NULL;

    time(&rawtime);
    
    fp = fopen(history_path, "r+");
    fseek(fp, 0, SEEK_END);

    fprintf(fp, "#%ld\r\n%s\n",rawtime,command);
    fclose(fp);
}
void get_history(char *pwd,int n){  // Get n commands in history.
    struct {
        int no;
        char time[80];
        char command[100];
    }his[1000];

    FILE* fp;
    // char load_file[20] = ".bash_history";
    char load_file[30] = "";
    strcat(load_file,pwd);
    strcat(load_file,"/.bash_history");
    char line[1000];
    int no = 0;
    struct tm* info;
    char tmp[80];
    long long tmp_time = 0;
    int i;
    time_t tmp_t;

    fp = fopen(load_file, "r");
    if (fp == NULL)
    {
        printf("can not load file!");
        return ;
    }
    while (!feof(fp))
    {
        fgets(line, 1000, fp);
        if (line[0] == '#') {
            no = no + 1;
            his[no - 1].no = no;
            tmp_time = 0;
            for (i = 1; i < (strlen(line)-2); i++){
                int tmp_a = line[i];
                int tmp_b = '0';
                tmp_time = tmp_time * 10 + (tmp_a-tmp_b);
            }
            tmp_t = tmp_time;
            info = localtime(&tmp_t);
            strftime(his[no - 1].time, 80, "%Y-%m-%d %H:%M:%S", info);

        }
        else {
            strcpy(his[no - 1].command, line);
        }
    }
    i = 0;
    if (n < no){
        i = no-n;
    }
    for (; i < no; i++)
        printf(" %3d %s %s", his[i].no, his[i].time, his[i].command);
    fclose(fp);
}

void do_execute_pipeNotHave(char **argv,int argc,int redirectHave,char *file){
    
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
        }else {
		//int i;
		//for(i = 0; i < argc; i++)
			//printf("%s\n", argv[i]);
            if(execvp(argv[0], argv)==-1)
                printf("%s\n",strerror(errno));
        //exit(1)
	}
    }

    wait(NULL);
}

void do_execute_pipeHave(char **argv1, char **argv2){

    int pipe_fd[2];
    int status;
    pipe(pipe_fd);
    pid_t child1,child2;
    if ((child1=fork())!=0) //父进程
    { 
        if ( (child2 = fork()) == 0 )
        { //子进程 
            close ( pipe_fd[1] );
            close ( fileno ( stdin ) );
            dup2 ( pipe_fd[0] , fileno(stdin));
            close ( pipe_fd[0] );
            execvp ( argv2[0] , argv2 );
        } 
        else  
        {
            close ( pipe_fd[0]);
            close ( pipe_fd[1]);
            waitpid ( child2 , &status , 0 );
        }
        waitpid ( child1 , &status , 0 );
    } 
    else 
    {
        //printf ( "subshell 3cmd %d\n" , getpid() );
        close ( pipe_fd[0] );
        close ( fileno(stdout));
        dup2 ( pipe_fd[1] , fileno(stdout));

        close ( pipe_fd[1] );
        execvp ( argv1[0] , argv1 );
    }
}


void execute(char **argv,int argc,char *pwd){

    if(strncmp(argv[0],"history",7)==0){ // 如果说是查询历史命令
        if (argv[1] == NULL) get_history(pwd,200); else { // 无参数
            int tmp_n = 0;
            while(*argv[1] != '\0'){
                tmp_n = tmp_n*10 + (*argv[1] - '0');
                ++argv[1];
            }
            get_history(pwd,tmp_n);
        }
        return;
    }

    char *argv1[8] = {0};
    char *argv2[8] = {0};
    int pipeHave = 0; // Does it contain pipes
    int redirectHave = 0; // Does it contain redirects
    char* file; // 重定向所指向的文件

    int i=0;
    int j=0;
    for(i = 0; i < argc ;++i){
        if(strcmp(argv[i], ">") == 0){  // 输出重定向
            redirectHave = 1;
            file = argv[i+1];
            argv[i] = NULL;
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
    if(!pipeHave) do_execute_pipeNotHave(argv,argc,redirectHave,file); 
        else do_execute_pipeHave(argv1,argv2);
}

