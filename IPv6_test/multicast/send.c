/*
 *  sender.c -- multicasts "hello, world!" to a multicast group once a second
 */
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>
#include <string.h>
#include <stdio.h>


#define HELLO_PORT  8000
#define HELLO_GROUP "ff02::1"

int main()
{
	struct sockaddr_in6 addr6;
    int sockfd;
//	struct ip_mreq mreq;
    char *message = "Hello, World!";

    /* create UDP socket */
    if ((sockfd = socket(AF_INET6, SOCK_DGRAM, 0)) == -1) {
		perror("socket");
		return 0;
    }

    /* set up destination address */
    memset(&addr6, 0, sizeof(addr6));
    addr6.sin6_family = AF_INET6;
	inet_pton(AF_INET6, HELLO_GROUP, &(addr6.sin6_addr));
    addr6.sin6_port = htons(HELLO_PORT);
		
    /* now just sendto() our destination! */
    while (1) {
		if (sendto(sockfd, message, 14, 0, (struct sockaddr *) &addr6, sizeof(addr6)) == -1)		{
	    	perror("sendto");
	    	return 0;
		}
		sleep(1);
    }
	return 0;
}
