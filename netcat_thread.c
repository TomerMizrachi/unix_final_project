#include <stdio.h> 
#include <strings.h> 
#include <sys/types.h> 
#include <arpa/inet.h> 
#include <sys/socket.h> 
#include <netinet/in.h>
#include <sys/wait.h> 
#include <netdb.h>
#include <unistd.h> 
#include <stdlib.h> 
#include "declerations.h"

#define PORT 10000
// #define IP_ADDR 0x7f000001
  

void* udp( void *arg ) 
{   
    char* buffer = ((struct args*)arg)->buffer;
    char* ip_addr = ((struct args*)arg)->ip_addr;

    int sockfd, n; 
    struct sockaddr_in servaddr; 
      
    // clear servaddr 
    bzero(&servaddr, sizeof(servaddr)); 
    servaddr.sin_port = htons(PORT); 
    servaddr.sin_family = AF_INET; 
    if (inet_aton(ip_addr , &servaddr.sin_addr) == 0) 
	{
		fprintf(stderr, "inet_aton() failed\n");
		exit(1);
	}
      
    // create datagram socket 
    sockfd = socket(AF_INET, SOCK_DGRAM, 0); 
  
    // connect to server 
    if(connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) 
    { 
        printf("\n Error : Connect Failed : there was no server to connect to \n"); 
    } 
    printf("udp client connected\n");    

    if((n = send(sockfd, buffer, strlen(buffer), 0)) < 0){
        printf("UDP sending ERROR");
    }
    printf("udp client sent inotify information\n");
    // close the descriptor 
    close(sockfd);   
} 