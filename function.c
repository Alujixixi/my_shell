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
#include<time.h>
#include "errno.h"

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


