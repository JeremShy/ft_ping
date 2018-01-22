#ifndef FILLER_H
# define FILLER_H

# include <libft.h>
# include <stdio.h>
# include <sys/types.h>
# include <sys/socket.h>
# include <sys/time.h>
# include <netdb.h>
# include <unistd.h>
# include <arpa/inet.h>
# include <stdlib.h>
# include <signal.h>
# include <netinet/ip_icmp.h>


# define NO_OPT	0
# define OPT_v	1
# define OPT_h	2

# ifdef __APPLE__
	struct icmphdr
	{
	  u_int8_t type;                /* message type */
	  u_int8_t code;                /* type sub-code */
	  u_int16_t checksum;
	  union
	  {
	    struct
	    {
	      u_int16_t id;
	      u_int16_t sequence;
	    } echo;                     /* echo datagram */
	    u_int32_t   gateway;        /* gateway address */
	    struct
	    {
	      u_int16_t __glibc_reserved;
	      u_int16_t mtu;
	    } frag;                     /* path mtu discovery */
	  } un;
	};
	struct iphdr
	  {
	#if __BYTE_ORDER == __LITTLE_ENDIAN
	    unsigned int ihl:4;
	    unsigned int version:4;
	#elif __BYTE_ORDER == __BIG_ENDIAN
	    unsigned int version:4;
	    unsigned int ihl:4;
	#else
	# error "Please fix <bits/endian.h>"
	#endif
	    u_int8_t tos;
	    u_int16_t tot_len;
	    u_int16_t id;
	    u_int16_t frag_off;
	    u_int8_t ttl;
	    u_int8_t protocol;
	    u_int16_t check;
	    u_int32_t saddr;
	    u_int32_t daddr;
	    /*The options start here. */
	  };
# endif

typedef struct s_pckt {
	float 		time;
	int				seq;
	struct s_pckt	*next;
}				t_pckt;


typedef struct s_data {
	struct timeval start_time;
	int		opt;
	char	*rhost;
	char	rp[20];
	int		ac;
	char	**av;
	pid_t	pid;
	struct addrinfo *res;
	int	sock;
	int seq;
	t_pckt *lst;
	int	ntransmitted;
	int	nreceived;
	int	ttl;
}								t_data;


extern t_data g_data;

int	init_socket(t_data *data);
int send_echo_request(t_data *data);
void recv_echo_response(t_data *data);

t_pckt *create_pckt(int seq, float time);
t_pckt	*add_pckt(t_pckt *lst, t_pckt *pckt);
void	print_list(t_pckt	*lst);
void print_infos(t_data *data, struct timeval stop_time);

float calc_min(t_pckt *lst);
float calc_max(t_pckt *lst);
float calc_avg(t_pckt *lst);
float calc_mdev(t_pckt *lst);
uint16_t invert_s_byte_order(uint16_t nbr);
uint16_t ft_htons(uint16_t hostshort);
uint16_t ft_ntohs(uint16_t netshort);


void	print_memory(char *start, size_t size);
void	print_icmp_hdr(struct icmphdr *hdr);

void print_ok_msg(t_data *data, char buffer[200], struct timeval recvtime);
void print_error_message(t_data *data, char buffer[200]);


#endif
