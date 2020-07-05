#ifndef __HEAD_H
#define __HEAD_H

//extern char *pathvar;
//extern char *paths[20];
//extern int path_cnt;

void init_shell();

int scanfile(char *cmd);

void split(char *src, const char *separator, char **dest, int *num);

void delete_space(char *string);

void get_string(char *s, int size);

void background(char *cod, char **coms);

void write_history(char *pwd,char *command);

void get_history(char *pwd,int n);

void execute(char **argv,int argc,char *pwd);
#endif
