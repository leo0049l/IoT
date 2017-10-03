#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdbool.h>
#include <strings.h>
#include <string.h>
#include <stdlib.h> 
#include "coap.h"

#define PORT 5683

int main(int argc, char **argv)
{
    int fd; char buff='3';
    size_t lenn=sizeof(buff);
#ifdef IPV6
    struct sockaddr_in6 servaddr, cliaddr;
#else /* IPV6 */
    struct sockaddr_in servaddr, cliaddr;
#endif /* IPV6 */
    uint8_t buf[4096];
    uint8_t scratch_raw[4096];
    coap_rw_buffer_t scratch_buf = {scratch_raw, sizeof(scratch_raw)};

#ifdef IPV6
    fd = socket(AF_INET6,SOCK_DGRAM,0);
#else /* IPV6 */
    fd = socket(AF_INET,SOCK_DGRAM,0);
#endif /* IPV6 */

    bzero(&servaddr,sizeof(servaddr));
#ifdef IPV6
    servaddr.sin6_family = AF_INET6;
    servaddr.sin6_addr = in6addr_any;
    servaddr.sin6_port = htons(PORT);
#else /* IPV6 */
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);
#endif /* IPV6 */
    bind(fd,(struct sockaddr *)&servaddr, sizeof(servaddr));

    endpoint_setup();
/////////
    int     sockfd;
    struct sockaddr_in serv_addr;
    struct sockaddr cli_addr;
    socklen_t       cli_len;
    socklen_t       serv_len;
    cli_len = sizeof(cli_addr);
    serv_len = sizeof(serv_addr);
    int m;
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if(sockfd == -1)
    {
        printf("cannot create socket.\n");
        exit(0);
    }
    bzero(&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));
    
    if((bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) == -1))
    {
        printf("cannot bind.\n");
        exit(0);
    }

//////////////
    while(1)
    {
        int n, rc;
        socklen_t len = sizeof(cliaddr);
        coap_packet_t pkt;

        n = recvfrom(fd, buf, sizeof(buf), 0, (struct sockaddr *)&cliaddr, &len);
#ifdef DEBUG
        printf("Received: ");
        coap_dump(buf, n, true);
        printf("\n");
#endif

        if (0 != (rc = coap_parse(&pkt, buf, n)))
	{
		char newbuf[4096];
		for(int iii=0;iii<7;iii++)
		{
			newbuf[iii]= (char)buf[iii];
		}
            if(buf[7]=='0')
            newbuf[7]= '\0';
            else
            {
                newbuf[7]='_';newbuf[8]='w';newbuf[9]='a';newbuf[10]='r';newbuf[11]='n';newbuf[12]='i';
                newbuf[13]='n';newbuf[14]='g';newbuf[15]='\0'; 
            }
		printf("%s\n",newbuf);
        //char* k=malloc(sizeof(char));
        changelight(buf);
        
        char* ack=malloc(sizeof(char));

        //printf("%s",k);
        m=recvfrom(sockfd,ack, 1,0, &cli_addr, &cli_len);

        sendto(sockfd, newbuf, 16, 0, &cli_addr, cli_len);
        //sendto(connfd, k, strlen(k), 0, (struct sockaddr *) &server_addr, servlen);

            //printf("Bad packet rc=%d\n", rc);
        }
	else
        {
            size_t rsplen = sizeof(buf);
            coap_packet_t rsppkt;
#ifdef DEBUG
            coap_dumpPacket(&pkt);
#endif
            coap_handle_req(&scratch_buf, &pkt, &rsppkt);

            if (0 != (rc = coap_build(buf, &rsplen, &rsppkt)))
                printf("coap_build failed rc=%d\n", rc);
            else
            {
#ifdef DEBUG
                printf("Sending: ");
                coap_dump(buf, rsplen, true);
                printf("\n");
#endif
#ifdef DEBUG
                coap_dumpPacket(&rsppkt);
#endif

                sendto(fd, buf, rsplen, 0, (struct sockaddr *)&cliaddr, sizeof(cliaddr));
            }
        }
    }
}

