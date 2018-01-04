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

void analyse_icmp_response(t_data *data, char buffer[200], int r)
{
	struct icmphdr *icmp;
	struct iphdr *ip;
	struct timeval *tv;
	char *msg;

	ip = (struct iphdr*)buffer;
	icmp = (void*)buffer + ip->ihl * 4;
	tv = (void*)buffer + ip->ihl * 4 + sizeof(struct icmphdr);
	msg = (void*)buffer + ip->ihl * 4 + sizeof(struct icmphdr) + sizeof(struct timeval);

	printf ("icmp type : %d\n", icmp->type);
	printf ("icmp code : %d\n", icmp->code);
	printf ("icmp id : %d\n", ntohs(icmp->un.echo.id));
	printf ("icmp seq : %d\n", ntohs(icmp->un.echo.sequence));
	printf ("icmp message : [%s]\n", msg);

	printf ("%ld - %ld\n", tv->tv_sec, tv->tv_usec);
}

void recv_echo_response(t_data *data)
{
	struct msghdr msghdr;
	char buffer[200];
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
	printf("r : %d\n", r);
	buffer[r] = 0;

	if (r == -1)
		dprintf(2, "Error !\n");
	analyse_icmp_response(data, buffer, r);
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
	strcpy(msg, "coucou");
	if (gettimeofday(tv, NULL) == -1)
	{
		dprintf(2, "Error while calling gettimeofday.\n");
		return (0);
	}
	icmp->checksum = checksum(dgram, sizeof(dgram));
	sendto(data->sock, dgram, sizeof(dgram), 0, data->res->ai_addr, data->res->ai_addrlen);
	printf ("%ld - %ld\n", tv->tv_sec, tv->tv_usec);
	return (1);
}

int	init_socket(t_data *data)
{
	data->sock = socket(AF_INET, SOCK_RAW, 1); // proto ICMP : 1

	if ((getaddrinfo(data->rhost, NULL, NULL, &(data->res)) != 0) || !(data->res))
	{
		dprintf(2, "getaddrinfo failed for rhost %s\n", data->rhost);
		return (0);
	}

	if (data->res->ai_family == AF_INET)
	{
		if (inet_ntop(data->res->ai_family, &((struct sockaddr_in*)(data->res->ai_addr))->sin_addr, data->rp, sizeof(data->rp)) == 0)
		{
			dprintf(2, "inet_ntop failed.\n", data->rhost);
			return (0);
		}
	}
	else if (data->res->ai_family == AF_INET6)
	{
		if (inet_ntop(data->res->ai_family, &((struct sockaddr_in6*)(data->res->ai_addr))->sin6_addr, data->rp, sizeof(data->rp)) == 0)
		{
			dprintf(2, "inet_ntop failed.\n", data->rhost);
			return (0);
		}
	}
	return (1);
}
