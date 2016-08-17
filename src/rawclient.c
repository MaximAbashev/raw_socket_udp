# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <netinet/in.h>
# include <sys/socket.h>
# include <linux/if_ether.h>
# include "../hdr/func.h"

# define PCKT_LEN 1024
# define SRV_PORT 4444
# define CLNT_PORT 5555

void main ()
{
	int raw_sock, from_clnt_buf, i;
	const int on = 1;
	struct sockaddr_in s_addr, clnt_addr;
	char *datagram;
	struct ps_header *ps_hdr;
	struct eth_header *eth_hdr;
	struct ipv4_header *ip_hdr;
	struct udp_header *udp_hdr;
/*
 * Allocate memory to fabricate packet.
 */
	datagram = malloc (sizeof(char) * PCKT_LEN);
	bzero (datagram, PCKT_LEN);
/*
 * Fabricate socket and set socket options.
 */
	s_addr.sin_family = AF_INET;
	s_addr.sin_addr.s_addr = htonl (INADDR_LOOPBACK);
	s_addr.sin_port = htons (SRV_PORT);
	raw_sock = socket (AF_INET, SOCK_RAW, IPPROTO_UDP);
	if (raw_sock < 0)	
	{
		printf ("Client socket initiate error\n");
		exit (1);
	}
/* 
 * Fabricate UDP header and data.
 */
	udp_hdr = malloc (sizeof (struct udp_header));
	udp_hdr -> uh_sport = htons (CLNT_PORT);
	udp_hdr -> uh_dport = s_addr.sin_port;
	udp_hdr -> uh_ulen = htons (sizeof (struct udp_header) / 4);
	udp_hdr -> uh_sum = 0x0000;
	memcpy (datagram, udp_hdr, sizeof (*udp_hdr));
	memcpy (datagram + sizeof (*udp_hdr), "Hello\n", 5);
	sendto (raw_sock, datagram, sizeof (datagram), 0, 
            (struct sockaddr *)&s_addr, sizeof(s_addr));
	close (raw_sock);
}
