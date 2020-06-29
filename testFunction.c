#include <stdio.h>
#include "function.h"
#include <string.h>

int main() {
	char s[100];
	fgets(s, sizeof(s), stdin);
	char *res[8] = {0};
	int num = 0;
	split(s, " " , res, &num);
	printf("%d\n", num);
	int i = 0;
	for(i = 0; i < num; i++) {
		printf("%s\n", res[i]);
	}
	return 0;
}
