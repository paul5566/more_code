/*
 *	This is IPV6 UDP multicast reciver, just excute it 	
 *				
 */
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>
#include <string.h>
#include <stdio.h>


#define HELLO_PORT 8000
#define HELLO_GROUP "ff02::1"
#define MSGBUFSIZE 65536

int main()
{
	struct sockaddr_in6 addr6;
	int sockfd, num;
	socklen_t addrlen;
    struct ipv6_mreq mreq6;
    char buf[MSGBUFSIZE];

    u_int yes = 1;	//set socket opt agrum      

     /* create UDP socket */
     if ((sockfd = socket(AF_INET6, SOCK_DGRAM, 0)) == -1)	{
	 	perror("socket");
	 	return 0;
     }

    /* allow multiple sockets to use the same PORT number */
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) < 0)	{
    	perror("Reusing ADDR failed");
    	return 0;
    }

    /* set up destination address */
    memset(&addr6, 0, sizeof(addr6));
    addr6.sin6_family = AF_INET6;
    addr6.sin6_addr = in6addr_any; 
    addr6.sin6_port = htons(HELLO_PORT);
     
    /* bind to receive address */
    if (bind(sockfd, (struct sockaddr *)&addr6, sizeof(addr6)) < 0)	{
	 	perror("bind");
	 	return 0;
    }
     
    /* use setsockopt() to request that the kernel join a multicast group */
	inet_pton(AF_INET6, HELLO_GROUP, &(mreq6.ipv6mr_multiaddr));
	mreq6.ipv6mr_interface = htons(INADDR_ANY);
	/* set socket operation to join the multicast group in IPV6 way */
    if (setsockopt(sockfd, IPPROTO_IPV6, IPV6_JOIN_GROUP, &mreq6, sizeof(mreq6)) < 0)	{
	 	perror("setsockopt");
	 	return 0;
    }
	
    /* read & print loop */
    while (1) {
		addrlen = sizeof(addr6);
		if ((num = recvfrom(sockfd, buf, MSGBUFSIZE,0, (struct sockaddr *)&addr6, &addrlen)) < 0) {
	    	perror("recvfrom");
	        return 0;
	  	} else {
	 		buf[num] = '\0';
	 		printf("recv %d byte\ncontent : %s\n", num, buf);
		}
    }
	return 0;
}
