# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <netinet/in.h>
# include <sys/socket.h>
# include <sys/types.h>
# include <linux/if_ether.h>
# include "../hdr/func.h"

# define PCKT_LEN 1024
# define SRV_PORT 4444
# define CLNT_PORT 5555

void main ()
{
	int raw_sock, from_clnt_buf, len;
	struct sockaddr_in s_addr, clnt_addr;
	char *datagram;
	char buf[PCKT_LEN];
	struct udp_header *udp_hdr;
	struct ipv4_header *ip_hdr;
/*
 * Allocate memory for sending packet.
 */
	datagram = malloc (sizeof (char) * PCKT_LEN);
	bzero (datagram, PCKT_LEN);
/*
 * Fabricate socket and set socket options.
 */
	s_addr.sin_family = AF_INET;
	s_addr.sin_port = htons (SRV_PORT);
	s_addr.sin_addr.s_addr = htonl (INADDR_LOOPBACK);
	raw_sock = socket (AF_INET, SOCK_RAW, IPPROTO_UDP);
	if (raw_sock < 0)
	{
		perror ("create socket");
	}
	len = sizeof (struct sockaddr_in); /* for recvfrom */
	while (1)
	{
		while (2)
		{
			from_clnt_buf = recvfrom (raw_sock, datagram, PCKT_LEN, 0, 
                (struct sockaddr *)&clnt_addr, &len);
			if (from_clnt_buf > 0)
			{
				printf ("I take a message... this is...\n");
				ip_hdr = (struct ipv4_header*)(datagram); /* extraction ip-header */
				udp_hdr = 
                   (struct udp_header*)(datagram + sizeof(struct ipv4_header)); /* extraction udp-header */
				if ((udp_hdr -> uh_dport) == htons (SRV_PORT))
				{
					printf ("frendly message!\n");
					printf ("%.2x ", datagram + sizeof(*udp_hdr));
				}
				else
				{
					printf ("enemy message!\n");
				}
				break;
			}
			perror ("Packet recieve error:");
		}
	}

}
