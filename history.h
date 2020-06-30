#include<stdio.h>
#include<string.h>
#include<time.h>
#include<stdlib.h>
void write_history(char command[100]) // Send commands here
{
	time_t rawtime;
	struct tm* info;
	char buffer[80];
	char history_path[30] = ".bash_history";
	FILE* fp = NULL;

	time(&rawtime);
	
	fp = fopen(history_path, "r+");
	//if (number%5==0){
	//	fseek(fp, 0, SEEK_SET);
	//} else {
	//	fseek(fp, 0, SEEK_END);
	//}
	//fp = fopen(history_path, "r+");
	fseek(fp, 0, SEEK_END);

	fprintf(fp, "#%ld\r\n%s",rawtime,command);
	fclose(fp);
}
void get_history(int n){  // Get n commands in history.
	struct {
		int no;
		char time[80];
		char command[100];
	}his[1000];

	FILE* fp;
	char load_file[20] = ".bash_history";
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
