/*
 * Here is UDP ipv6 client
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
	struct addrinfo sa, *resault, *p;
	char ip[INET6_ADDRSTRLEN];

	printf("enter a IPv6 address if you want to connect :");
	scanf("%s", ip);
	printf("you enter ip = %s\n", ip);
	
	memset(&sa, 0, sizeof(sa));
	sa.ai_family = AF_INET6;
	sa.ai_socktype = SOCK_DGRAM;	//DataGram type
	sa.ai_protocol = 0;
	
	if (getaddrinfo(ip, PORT, &sa, &resault) != 0)	{
		perror("<client> getaddrinfo");
		return 0;
	}

	for (p = resault; p != NULL; p = p->ai_next)	{
		/* get socket fd */
		if ((sock_fd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1)	{
			perror("<client> socket");
			continue;
		} else {
			printf("sock_fd = %d\n", sock_fd);
		}
		break;
	}
	/* fail to connect */
	if (p == NULL)	{
		printf("<client> Fail to connect\n");
		close(sock_fd);
		return 0;
	} else {
		printf("<client> connect to : %s\n", ip);
	}
	freeaddrinfo(resault);
	/* send loop */
	if (sendto(sock_fd, "Hello World!!", 14, 0, p->ai_addr, p->ai_addrlen) == -1)	{
		perror("<client> sendto");
		return 0;
	} else {
		printf("<client> sendto success...\n");
	} 
	close(sock_fd);
	return 0;
}
	








