#include <ft_ping.h>
#include <netdb.h>
#include <netinet/ip_icmp.h>
#include <netinet/ip.h>

int	create_socket(t_data *data)
{
	int	sock;
	struct addrinfo hints, *res;
	char dgram[4096];

	sock = socket(AF_INET, SOCK_RAW, IPPROTO_RAW); // proto ICMP : 1
	printf ("sock returned: %d\n", sock);

	if ((getaddrinfo(data->rhost, NULL, NULL, &res) != 0) || !res)
	{
		printf("getaddrinfo failed for rhost %s\n", data->rhost);
		return (0);
	}
	struct sockaddr_in saddr_in = *(struct sockaddr_in*)(res->ai_addr);
	struct sockaddr *saddr = (struct sockaddr*)(res->ai_addr);
	struct in_addr in_addr = saddr_in.sin_addr;
	char dst[20];
	printf("%s\n", inet_ntop(AF_INET, &in_addr, dst, 19));

	int on = 1;
	if(setsockopt(sock, IPPROTO_IP, IP_HDRINCL, &on, sizeof(on)) < 0)
	{
	 perror("setsockopt() for IP_HDRINCL error");
	 exit(1);
	}

	struct ip *ip = (struct ip*) dgram;
	struct icmphdr *icmp = (struct icmphdr *)(ip + 1);
	ip->ip_v = 4;
	ip->ip_hl = sizeof(*ip) >> 2;
	ip->ip_tos = 0x10;
	ip->ip_len = htons(sizeof(dgram));
	ip->ip_id = htons(666);
	ip->ip_off = htons(0);
	ip->ip_ttl = (u_int8_t)htons(60);
	ip->ip_p = (u_int8_t)htons(0x1);
	ip->ip_sum = htons(0);
	ip->ip_dst = in_addr;
	bzero(&(ip->ip_src), sizeof(struct in_addr));
	icmp->type = ICMP_ECHO;
	icmp->code = 0;



	if (connect(sock, saddr, sizeof(struct sockaddr)) != 0) {
		printf ("connect failed !");
	}


	return (sock);
}
