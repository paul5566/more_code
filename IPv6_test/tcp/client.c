/*
 * Here is TCP ipv6 client
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define PORT "8000"

int main()
{
	int sock_fd;
	char buf[20];
	struct addrinfo sa, *resault, *p;
	char ip[INET6_ADDRSTRLEN];

	printf("enter a IPv6 address if you want to connect :");
	scanf("%s", ip);
	printf("you enter ip = %s\n", ip);
	
	memset(&sa, 0, sizeof(sa));
	sa.ai_family = AF_INET6;
	sa.ai_socktype = SOCK_STREAM;
	sa.ai_protocol = 0;
	
	if (getaddrinfo(ip, PORT, &sa, &resault) != 0)	{
		perror("<client> getaddrinfo");
		return 0;
	}

	for(p = resault; p != NULL; p = p->ai_next)	{
		/* get socket fd */
		if ((sock_fd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1)	{
			continue;
		} else {
			printf("sock_fd = %d\n", sock_fd);
		}
		/* connect */
		if (connect(sock_fd, p->ai_addr, p->ai_addrlen) == -1)	{
			perror("<client> connect");
		//	continue;
		}
		break;
	}
	/* fail to connect */
	if (p == NULL)	{
		printf("<client> Fail to connect\n");
		close(sock_fd);
		exit(0);
	} else {
		printf("<client> connect to : %s\n", ip);
	}
	freeaddrinfo(resault);
	/* send & recv loop */
	while(1)	{
		/* init send & recv buf */
		memset(buf, 0, sizeof(buf));
		/* recv */
		if (recv(sock_fd, buf, sizeof(buf)-1, 0) < 0)	{
			perror("<client> recv");
			continue;
		} else {
			printf("<client> recv: %s\n", buf);
		}
		/* send buf setting */
		memset(buf, 0, sizeof(buf));
		printf("enter message:");
		scanf("%s", buf);
		/* send */
		if (send(sock_fd, buf, strlen(buf), 0) < 0)	{
			perror("<client> send");
			continue;
		} else {
			printf("send success\n");
		}
	}
	close(sock_fd);
	return 0;
}
	








