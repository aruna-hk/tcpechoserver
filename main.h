#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#define MAXLINE 1024
#define MAXCON 5
void err_quit(char *errmsg);
void err_sys(char *errmsg);
