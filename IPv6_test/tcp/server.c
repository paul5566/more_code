/*
 * Here is TCP ipV6 server.c
 *
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>

#define PORT "8000"
#define BACKLOG 10
int main(void)
{
    int sock_fd, recv_fd;
    int len, rv;
    int opt = 1;
    struct addrinfo sa, *resault, *p;	//server
	struct sockaddr_storage acp_addr;	//client
	socklen_t addrlen;	//client addr len
	char buf[20];
	char ip[INET6_ADDRSTRLEN];
	
	
    memset(&sa, 0, sizeof(sa));
    sa.ai_family = AF_INET6;	//ipv6	
    sa.ai_socktype = SOCK_STREAM;	//stream type
    sa.ai_flags = AI_PASSIVE;	//auto fill in IP addr	
    sa.ai_protocol = 0;		//any protocol
    
    rv = getaddrinfo(NULL, PORT, &sa, &resault);
    if(rv != 0) {
        printf("<server> getaddrinfo : %s\n", gai_strerror(rv));
        exit(0);
    }
	
    /* find all of useful resault */
    for (p = resault; p != NULL; p = p->ai_next)	{
		/* get socket FD */
		if ((sock_fd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1)	{
	    	perror("<server> socket");
            continue;
        } else {
			printf("<server> sock_fd = %d\n", sock_fd);
		}
        /* socket operation : reuse address */
        if (setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1)	{
            perror("<server> setsockopt : SO_REUSEADDR");
	    	close(sock_fd);
		}
		/* socket operation : IPv6 only */
		if (setsockopt(sock_fd, IPPROTO_IPV6, IPV6_V6ONLY, &opt, sizeof(opt)) == -1)	{
			perror("<server> setsockopt : IPV6_V6ONLY");
			close(sock_fd);
		}

		/* bind*/
		if (bind(sock_fd, p->ai_addr, p->ai_addrlen) == -1)		{
			perror("<server> bind");
			close(sock_fd);
			continue;
		} else {
			printf("bind success....\n");
		}
		break;
	}
	if (p == NULL)	{
		printf("<server> Fail to create \n");
		close(sock_fd);
		return 0;
	}	
	/* listen */
	if (listen(sock_fd, BACKLOG) == -1)		{
		perror("listen");
		return 0;
	}
	freeaddrinfo(resault); 
	printf("server watiing for connect.......\n");
	
	/* accept loop */

	/* accept */
	addrlen = sizeof(acp_addr);
	if ((recv_fd = accept(sock_fd, (struct sockaddr *)&acp_addr, &addrlen)) == -1)	{
		perror("<server> accept");
		return 0;
	} else {
		printf("accept success...recv_fd: %d\n", recv_fd);
	}
	struct sockaddr_in6 *s = (struct sockaddr_in6 *)&acp_addr;
	inet_ntop(AF_INET6, &s->sin6_addr, ip, sizeof(ip));
	printf("<server> recv from IP: %s\n", ip);
	/* send & recv loop */
	while(1)	{
		/* init send buf */
		memset(buf, 0, sizeof(buf));	
		printf("send message : ");
		scanf("%s", buf);	
		/* send  */
		if ((len = send(recv_fd, buf, strlen(buf), 0)) < 0)	{
			perror("<server> send");
			continue;
		} else {
			printf("send success !!\n");
		}
		/* recv */
		memset(buf, 0, sizeof(buf));
		if ((len = recv(recv_fd, buf, sizeof(buf), 0)) < 0)	{
			perror("<server> recv");
			continue;
		} else {
			printf("recv: %s\n", buf);
		}
	}
	close(sock_fd);
	return 0;
}




    

