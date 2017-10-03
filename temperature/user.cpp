#include <errno.h>
#include <signal.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <vector>
#include <math.h>
#define MAX 256
#define COMMAND_MAX 1400
using namespace std;
int main(int argc, char **argv)
{
	struct sockaddr_in server_addr;
	int socket_fd;
    struct sockaddr cliaddr;
    socklen_t 		clilen;
    clilen = sizeof(cliaddr);
    socklen_t 		servlen;
    servlen = sizeof(server_addr);

    
    bzero(&server_addr, sizeof(server_addr));//initial server_addr
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(atoi(argv[2]));//host to network long (2 bytes)
    inet_pton(AF_INET,argv[1],&server_addr.sin_addr);
	if((socket_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)//SOCK_DGRAM for UDP
	{
		return 0;
	}
    
    
    
    char* ack=new char;

    while(1)//read(fd,line,COMMAND_MAX)
    {
 
        while(1)
        {
           char line[16];
           line[8]='0';
           sendto(socket_fd, ack,1, 0, (struct sockaddr *) &server_addr, servlen);//write(socket_fd,line,n);

           int n=recvfrom(socket_fd,line,16,0,  &cliaddr, &clilen);
           if(line[0]>'9')
            continue;
           cout<<line<<endl;
           if(line[7]==95)
		{
			printf("請拔掉插頭，溫度過高囉！\n");
		}
             
        }
    }
    
    
   
    return 0;
}
