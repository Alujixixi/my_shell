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



