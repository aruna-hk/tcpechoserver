#include "main.h"
#include <errno.h>
//echo client
#include <signal.h>
#include <stdbool.h>
#include <time.h>
#include <sys/time.h>
#include <sys/select.h>
/**
int select(int maxfd1, fd_ser *readset, fd_set *write_set, const struct timeval);
//timeva
//NULL FOREVER until one happends
struct timeval {
  long tv_sec;
  long tv_usec;
}
*/
void piperr(int sig) {
	printf("pip error conection close exitine ...");
	exit(0);
}
struct n {
  long j;
  long v;
};
int main(int argc, char **argv) {
	int socketfd, i = 0;
        struct n num;
	num.j  = 3435676;
	num.v = 2000000;
        long p;
	struct sockaddr_in servaddr;
	fd_set rset;
	struct timeval timer;
	timer.tv_sec = 10;
	timer.tv_usec = 0;
	if (argc != 2) {
		printf("usage: a.out <IPADDRESS>");
		exit(0);
	}
	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(7);//echo server port
	if (inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0) {
		perror("inet_pton");
		exit(0);
	}
	//for (i = 0; i < 10; i++) {
	if ((socketfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
		perror("socket_error");
		exit(0);
	}
	if (connect(socketfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0) {
		perror("connection error");
		exit(0);
	}
//		close(socketfd[i]);
	char sendline[MAXLINE], recvline[MAXLINE];
	int n_read, n_write, nn_write = 0;
	signal(SIGPIPE, SIG_IGN);

	FD_ZERO(&rset);
	label:
		FD_SET(0, &rset);
		FD_SET(socketfd, &rset);
		if (select(1024, &rset, NULL, NULL, NULL) == 0) {
			printf("connection timeout");
			exit(0);
		}
		if (FD_ISSET(0, &rset) == true) {
			n_read = read(0, sendline, MAXLINE);
			write(socketfd, sendline, n_read);
		}
		if (FD_ISSET(socketfd, &rset) == true) {
			n_read = read(socketfd, recvline, MAXLINE);
			if (n_read == 0) {
				printf("server terminated conn ..\n");
				return (0);
			}
			write(1, recvline, n_read);

		}
	goto label;
	return (0);
}
