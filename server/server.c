#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include "homeapp.h"
#include "server.h"
//int main(){
void* server_func(void* arg){
	int server_sockfd = 0;
	int client_sockfd = 0;
	int server_len = 0;
	int client_len = 0;
	struct sockaddr_in server_address;
	struct sockaddr_in client_address;
	int result = 0;
	fd_set readfds, testfds;
	HOMEDATA data; 
	server_sockfd = socket(AF_INET, SOCK_STREAM, 0);
	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = htonl(INADDR_ANY);
	server_address.sin_port = htons(SERVER_PORT);
	server_len = sizeof(server_address);
	bind(server_sockfd, (struct sockaddr *)&server_address, server_len);
	listen(server_sockfd, 5);
	FD_ZERO(&readfds);
	FD_SET(server_sockfd, &readfds);
	while(1){
		char ch = 0;
		int fd = 0;
		int nread = 0;
		testfds = readfds;
		
		printf("server waiting\n");
		result = select(FD_SETSIZE, &testfds, (fd_set *)0, (fd_set *)0, (struct timeval *)0);
		if(result < 1){
			perror("select_svr");
			exit(1);
		}
		for (fd = 0; fd < FD_SETSIZE; fd++){
			if(FD_ISSET(fd, &testfds)){
				if(fd == server_sockfd){
					client_len = sizeof(client_address);
					client_sockfd = accept(server_sockfd, (struct sockaddr *)&client_address, &client_len);
					FD_SET(client_sockfd, &readfds);
					printf("adding client on fd %d\n", client_sockfd);
				}
				else{
					ioctl(fd, FIONREAD, &nread);
						if(nread == 0){
						close(fd);
						FD_CLR(fd, &readfds);
						printf("removing client on fd %d\n", fd);
					}
					else{
						read(fd, &data, sizeof(HOMEDATA));
						print_homedata(&data);
						get_homedata(&data);
						write(fd, &data, sizeof(HOMEDATA));
					}
				}
			}
		}
	}
	return (void*)NULL;
}
