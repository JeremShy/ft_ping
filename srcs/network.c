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
	if (size == 1)
		sum += *(unsigned char*)tmp;
	sum = (sum >> 16) + (sum & 0xFFFF);
	sum += (sum >> 16);
	return (~sum);
}

void analyse_icmp_response(t_data *data, char buffer[200], struct timeval recvtime)
{
	struct icmphdr *icmp;
	struct iphdr *ip;
	struct timeval *tv;
	float diff;
	char *msg;
	u_int16_t check;

	ip = (struct iphdr*)buffer;
	icmp = (void*)buffer + ip->ihl * 4;
	tv = (void*)buffer + ip->ihl * 4 + sizeof(struct icmphdr);
	msg = (void*)buffer + ip->ihl * 4 + sizeof(struct icmphdr) + sizeof(struct timeval);

	if (ip->protocol != 1)
		return ;
	if (icmp->type != 0 || icmp->code != 0)
		return ;
	if (ntohs(icmp->un.echo.id) != data->pid)
		return ;
	if (!ft_strequ(msg, "coucou"))
		return ;

	check = icmp->checksum;
	icmp->checksum = 0;
	if (checksum(icmp, 64) != check)
		return ;

	printf ("icmp seq : %d\n", ntohs(icmp->un.echo.sequence));

	diff = ((recvtime.tv_sec * 10000  + recvtime.tv_usec / 100) - (tv->tv_sec * 10000  + tv->tv_usec / 100)) / 10.0f;
	printf ("diff : %.1fms\n", diff);

	data->lst = add_pckt(data->lst, create_pckt(ntohs(icmp->un.echo.sequence), diff));
	data->nreceived++;
}

void recv_echo_response(t_data *data)
{
	struct msghdr msghdr;
	char buffer[200];
	struct timeval recvtime;
	struct iovec iov;

	ft_bzero(buffer, sizeof(buffer));
	ft_bzero(&msghdr, sizeof(msghdr));

	msghdr.msg_name = data->res->ai_addr;
	msghdr.msg_namelen = data->res->ai_addrlen;

	iov.iov_base = buffer;
	iov.iov_len = sizeof(buffer);
	msghdr.msg_iov = &iov;
	msghdr.msg_iovlen = 1;

	int r = recvmsg(data->sock, &msghdr, 0);
	gettimeofday(&recvtime, NULL);

	printf("r : %d\n", r);
	buffer[r] = 0;

	if (r != 64 + sizeof(struct iphdr))
		dprintf(2, "Error !\n");
	analyse_icmp_response(data, buffer, recvtime);
}

int send_echo_request(t_data *data)
{
	char dgram[64];
	struct icmphdr *icmp;
	char *msg;
	struct timeval *tv;

	bzero(dgram, sizeof(dgram));
	icmp = (struct icmphdr *)dgram;
	msg = dgram + sizeof(struct icmphdr) + sizeof(struct timeval);
	tv = (struct timeval*)(dgram + + sizeof(struct icmphdr));
	icmp->type = ICMP_ECHO;
	icmp->code = 0;
	icmp->un.echo.id = htons(data->pid);
	icmp->un.echo.sequence = htons(data->seq);
	data->seq++;
	strcpy(msg, "coucou");
	if (gettimeofday(tv, NULL) == -1)
	{
		dprintf(2, "Error while calling gettimeofday.\n");
		return (0);
	}
	icmp->checksum = checksum(dgram, sizeof(dgram));
	sendto(data->sock, dgram, sizeof(dgram), 0, data->res->ai_addr, data->res->ai_addrlen);
	data->ntransmitted++;
	return (1);
}

int	init_socket(t_data *data)
{
	struct addrinfo hints;

	ft_bzero(&hints, sizeof(struct addrinfo));
	hints.ai_flags = AI_CANONNAME;
	data->sock = socket(AF_INET, SOCK_RAW, 1); // proto ICMP : 1

	if ((getaddrinfo(data->rhost, NULL, &hints, &(data->res)) != 0) || !(data->res))
	{
		dprintf(2, "getaddrinfo failed for rhost %s\n", data->rhost);
		return (0);
	}
	printf("cannon : %s\n", data->res->ai_canonname);

	if (data->res->ai_family == AF_INET)
	{
		if (inet_ntop(data->res->ai_family, &((struct sockaddr_in*)(data->res->ai_addr))->sin_addr, data->rp, sizeof(data->rp)) == 0)
		{
			dprintf(2, "inet_ntop failed.\n");
			return (0);
		}
	}
	else if (data->res->ai_family == AF_INET6)
	{
		if (inet_ntop(data->res->ai_family, &((struct sockaddr_in6*)(data->res->ai_addr))->sin6_addr, data->rp, sizeof(data->rp)) == 0)
		{
			dprintf(2, "inet_ntop failed.\n");
			return (0);
		}
	}
	printf("rp : %s\n", data->rp);
	return (1);
}
