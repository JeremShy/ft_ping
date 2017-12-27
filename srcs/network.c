#include <ft_ping.h>
#include <netdb.h>
#include <netinet/ip_icmp.h>
#include <netinet/ip.h>

int	create_socket(t_data *data)
{
	int	sock;
	struct addrinfo hints, *res;
	char dgram[4096];

	sock = socket(AF_INET, SOCK_RAW, 1); // proto ICMP : 1
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

	struct icmphdr *icmp = (struct icmphdr *)dgram;
	icmp->type = ICMP_ECHO;
	icmp->code = 0;
	icmp->checksum = htons(~(ICMP_ECHO << 8));



	if (connect(sock, saddr, sizeof(struct sockaddr)) != 0) {
		printf ("connect failed !");
	}


	return (sock);
}
