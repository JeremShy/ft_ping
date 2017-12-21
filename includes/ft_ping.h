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

# define NO_OPT	0
# define OPT_v	1
# define OPT_h	2

typedef struct s_data {
	int	opt;
}								t_data;

#endif
