#include <stdio.h>
#include <time.h>
void init_daemon(void); 
/* 守护进程初始化函数  */   
main()   
{   
    FILE *fp;   
    time_t t;   
    init_daemon();
    /* 初始化为Daemon  */
    while(1)  
    {   
        printf("test\n");
    }
}

