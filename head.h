#ifndef __HEAD_H
#define __HEAD_H

void split(char *src, const char *separator, char **dest, int *num);

void delete_space(char *string);

void get_string(char *s, int size);

void background(char *cod, char **coms);

void write_history(char *pwd,char *command);

void get_history(char *pwd,int n);
#endif
