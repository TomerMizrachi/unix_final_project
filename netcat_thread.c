#include <stdio.h> 
#include <strings.h> 
#include <sys/types.h> 
#include <arpa/inet.h> 
#include <sys/socket.h> 
#include<netinet/in.h> 
#include<unistd.h> 
#include<stdlib.h> 
  
#define PORT 10000
#define IP_ADDR 0x7f000001
  

void* udp( void *arg ) 
{   
    char* buffer = (char *) arg;
    int sockfd, n; 
    struct sockaddr_in servaddr; 
      
    // clear servaddr 
    bzero(&servaddr, sizeof(servaddr)); 
    servaddr.sin_addr.s_addr = htonl(IP_ADDR); 
    servaddr.sin_port = htons(PORT); 
    servaddr.sin_family = AF_INET; 
      
    // create datagram socket 
    sockfd = socket(AF_INET, SOCK_DGRAM, 0); 
    while (1){
        // connect to server 
        if(connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) 
        { 
            printf("\n Error : Connect Failed \n"); 
            return; 
        } 
        printf("udp client connected");    

        if((n = send(sockfd, buffer, sizeof(buffer), 0)) < 0){
            perror("UDP sending ERROR");
            return;
        }
        // close the descriptor 
        close(sockfd);    
    }
    return;   
} 