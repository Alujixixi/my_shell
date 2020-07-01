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

void init_daemon(void) {   //daemon模式的后台运行程序
    int pid;
    int i;
    
    /* 是第一子进程，后台继续执行 */
    /* 第一子进程成为新的会话组长和进程组长 */
    setsid();
    /* 并与控制终端分离  */
    if(pid=fork()) {
        exit(0);
        /* 是第一子进程，结束第一子进程 */
    }else if(pid< 0) {
        exit(1);
        /* fork失败，退出 */
    }
    /* 是第二子进程，继续 */
    /* 第二子进程不再是会话组长 */
    /* 关闭打开的文件描述符 */
    for(i=0;i< NOFILE;++i) {
        close(i);
    }
    /* 改变工作目录到/tmp */
    chdir("/tmp");
   /* 重设文件创建掩模 */
    umask(0);
    return;
}



