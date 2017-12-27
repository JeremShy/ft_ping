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

typedef struct s_data {
	int		opt;
	char	*rhost;
	int		ac;
	char	**av;
}								t_data;

int	create_socket(t_data *data);

#endif
