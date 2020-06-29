#include "stdio.h"
#include "unistd.h"
#include "head.h"

int main() {
	char currentPath[80];
	char commend[100];
	getcwd(currentPath,sizeof(currentPath));
	printf("%s >", currentPath);
	while(1) {
		get_string(commend, sizeof(commend));	

		//在这里处理命令
		printf("the commend you key in is: ");
		fputs(commend, stdout);
		

		printf("%s >", currentPath);
	}	
	return 0;
}
