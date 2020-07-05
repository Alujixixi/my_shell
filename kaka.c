
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
int main() {
	char *pathvar;
	pathvar = getenv("PATH");
	printf("pathvar=%s\n", pathvar);
	char *paths[20]= {0};
	int num;
	split(pathvar, ":", paths, &num);
	int i;
	for(i = 0; i < num; i++) {
		printf("%s\n", paths[i]);
		char fp[100];
		strcat(strcpy(fp, paths[i]), "/ls");
		printf("%s\n", fp);
		if(access(fp, 0)==0)
			printf("access!\n");
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

