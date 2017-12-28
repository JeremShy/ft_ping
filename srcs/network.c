#include <ft_ping.h>
#include <netdb.h>
#include <netinet/ip_icmp.h>
#include <netinet/ip.h>

u_int16_t checksum(void *dgram, size_t size)
{
	u_int16_t *tmp;
	size_t sum;

	tmp = dgram;
	sum = 0;
	while (size > 1)
	{
		sum += *tmp;
		tmp++;
		size -= 2;
	}
	if ( size == 1 )
		sum += *(unsigned char*)tmp;
	sum = (sum >> 16) + (sum & 0xFFFF);
	sum += (sum >> 16);
	return (~sum);
}

int	create_socket(t_data *data)
{
	int	sock;
	struct addrinfo hints, *res;
	char dgram[64];

	char *msg;
	bzero(dgram, sizeof(dgram));
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
	msg = dgram + sizeof(struct icmphdr);
	icmp->type = ICMP_ECHO;
	icmp->code = 0;
	icmp->un.echo.id = htons(data->pid);
	icmp->un.echo.sequence = 0;
	strcpy(msg, "coucou");
	icmp->checksum = htons(checksum(dgram, sizeof(dgram)));
	sendto(sock, dgram, sizeof(dgram), 0, saddr, sizeof(struct sockaddr));


	return (sock);
}
