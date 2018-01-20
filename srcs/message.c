#include <ft_ping.h>

void print_ok_msg(t_data *data, char buffer[200], struct timeval recvtime)
{
	float	diff;
	struct icmphdr *icmp;
	struct iphdr *ip;
	struct timeval *tv;
	char dst[40];

	ip = (struct iphdr*)buffer;
	icmp = (void*)buffer + ip->ihl * 4;
	tv = (void*)buffer + ip->ihl * 4 + sizeof(struct icmphdr);

	if (ip->version == 4)
	{
		struct in_addr *addr;

		addr = (void*)&(ip->saddr);
		inet_ntop(AF_INET, addr, dst, sizeof(dst));
	}
	else if (data->res->ai_family == AF_INET6)
	{
		struct in_addr6 *addr;

		addr = (void*)&(ip->saddr);
		inet_ntop(AF_INET6, addr, dst, sizeof(dst));
	}
	diff = ((recvtime.tv_sec * 1000  + recvtime.tv_usec / 1000.0) - (tv->tv_sec * 1000  + tv->tv_usec / 1000.0));
	printf("%d bytes from %s: icmp_seq=%d ttl=%d time=%.1f ms\n", 64, dst, ntohs(icmp->un.echo.sequence), ip->ttl, diff);
	data->lst = add_pckt(data->lst, create_pckt(ntohs(icmp->un.echo.sequence), diff));
	data->nreceived++;
}

void print_error_message(t_data *data, char buffer[200])
{
	struct icmphdr *icmp;
	struct iphdr *ip;
	char dst[40];

	ip = (struct iphdr*)buffer;
	icmp = (void*)buffer + ip->ihl * 4;
	if (ip->version == 4)
	{
		struct in_addr *addr;

		addr = (void*)&(ip->saddr);
		inet_ntop(AF_INET, addr, dst, sizeof(dst));
	}
	else if (data->res->ai_family == AF_INET6)
	{
		struct in_addr6 *addr;

		addr = (void*)&(ip->saddr);
		inet_ntop(AF_INET6, addr, dst, sizeof(dst));
	}
	printf("%d bytes from %s: type = %d, code = %d\n", 64, dst, icmp->type, icmp->code);
}
