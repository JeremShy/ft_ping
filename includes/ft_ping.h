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
# endif


typedef struct s_data {
	int		opt;
	char	*rhost;
	int		ac;
	char	**av;
	pid_t	pid;
}								t_data;



int	create_socket(t_data *data);

#endif
