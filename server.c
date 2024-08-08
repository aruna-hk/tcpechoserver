#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <sys/select.h>
#define PORT 7
#define IP "127.0.0.1"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define MAXLINE 1024
#define BACKLOG 10


/*****************************************************************************
*****************ECHO SERVER*************************************************
One process handling all the clients using IO multiplexing
probably not recommended server since many client may end up exhausting
allowable file descriptors,(to somehow mitigate this whenever there is a read of
client connected socket and it returns 0 the descripto is closed to so that kernel can
reuse it when we request on)

best approach in server programming may be to pre-fork a fixed number of workers(children)
and let each server clients using different techniques one of which is IO multiplexing

*****************************************************************************/
int main()
{
	struct sockaddr_in client, server;//client and server ip storage
	char appbuffer[MAXLINE];//app buffer
	int listenfd, connfd;//file desciptore

	//socket creation
	if ( (listenfd = socket(AF_INET, SOCK_STREAM, IPPROTO_IP)) < 0) {
		perror("socket");
		exit(0);
	}
	//socket setup
	memset(&server, 0, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_port = htons(PORT);
	inet_pton(AF_INET, IP, &server.sin_addr);//conversion to network order

	//bind fd to ip
	if (bind(listenfd, (struct sockaddr *)&server, sizeof(server)) < 0) {
		perror("bind");
		exit(0);
	}

	//listen -- BACKLOG - max queue to be passed to kernel
	listen(listenfd, BACKLOG);

	int selected; //hold selected count --- in select not useful
		      //using -1 position to denote list of connected client ends

	fd_set readset;//select read set
	FD_ZERO(&readset);
	FD_SET(listenfd, &readset);//insert listening fd to readsocket

	//used to queue clients, maximum file descriptors posible
	//1024 - (0, 1,2 ,3) stdin, stdout, stderr and listening file descripto

	int clients[MAXLINE - 4];//0,1,2,3 excluded already taken

	for (int i = 0; i < MAXLINE - 4; i++)
		clients[i] = -1;//init entries to -1, unlikely file descripto

	int addr_size = sizeof(client);//to be passed by reference to te kernel in call to accept
	int maxfds = listenfd + 1;//1st arg to select, to be incremented whenever ne clients connect

	int n_read;//read data

	serverloop:
		//select listening fd
		selected = select(maxfds, &readset, NULL, NULL, NULL);
		if (FD_ISSET(listenfd, &readset)) {
			//accept client and add connection descriptor to readset
			connfd = accept(listenfd, (struct sockaddr *)&client, &addr_size);
			FD_SET(connfd, &readset);
			for (int i = 0; i < MAXLINE - 4; i++) {
				if (clients[i] == -2) {
					clients[i] = connfd;
					break;
				}
				if (clients[i] == -1) {
					maxfds++;
					clients[maxfds - 5] = connfd;
					break;
				}
			}
			if (selected == 1)
				goto serverloop;
		}
		int i = 0;
		while (clients[i] != -1) {
			if (FD_ISSET(clients[i], &readset)) {
				if ((n_read = read(clients[i], appbuffer, MAXLINE)) == 0) {
					FD_CLR(clients[i], &readset);
					close(clients[i]);
					clients[i] = -2;
					i++;
					continue;
				}
				write(clients[i], appbuffer, n_read);
			}
			if (clients[i] != -2)
				FD_SET(clients[i], &readset);
			i++;
		}
		FD_SET(listenfd, &readset);//if select turned off listening descriptor
	goto serverloop;
	return (0);
}
