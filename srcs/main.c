#include <ft_ping.h>

t_data g_data = {0};

void print_help(char *av)
{
	printf("usage: %s:\n\t\t[-h]: help\n\t\t[-v]: verbose\n\t\t[-t]: Time to live\n\t\tdestination\n", av);
}

int init_data(t_data *data, int ac, char **av)
{
	int	i;
	int	j;
	char *rhost;
	int	options_ended;
	char	*s;

	rhost = NULL;
	data->lst = NULL;
	data->ntransmitted = 0;
	data->nreceived = 0;
	data->opt = 0;
	data->av = av;
	data->ttl = 60;
	options_ended = 0;
	i = 1;
	while (i < ac)
	{
		if (!options_ended && ft_strequ(av[i], "--"))
		{
			options_ended = 1;
		}
		if (!options_ended && av[i][0] == '-')
		{
			j = 1;
			while (av[i][j])
			{
				if (av[i][j] == 'h')
					data->opt |= OPT_h;
				else if (av[i][j] == 'v')
					data->opt |= OPT_v;
				else if (av[i][j] == 't') // L'enfer sur terre, mais pas de norme et pas envie de mieux faire.
				{
					if (!av[i + 1])
						return (0);
					if (av[i][j + 1] != '\0')
						s = av[i] + j + 1;
					else
					{
						s = av[i + 1];
						i++;
					}
					data->ttl = ft_atoi(s);
					if (data->ttl <= 0 || data->ttl > 255)
					{
						dprintf(2, "%s: ttl %d out of range", av[0], data->ttl);
						return (-1);
					}
					break;
				}
				else
					return (0);
				j++;
			}
		}
		else
			rhost = av[i];
		i++;
	}
	if (!rhost && !(data->opt & OPT_h))
		return (0);
	data->rhost = rhost;
	return (1);
}

void do_send_echo_request(t_data *data)
{
	if (send_echo_request(data) == 0)
	{
		dprintf (2, "Error while trying to send echo request.\n");
		exit(EXIT_FAILURE);
	}
}

void sig_alarm(int sig)
{
	(void)sig;
	do_send_echo_request(&g_data);
	alarm(1);
}

void sig_int(int sig)
{
	(void)sig;
	t_data *data;
	struct timeval	stop_time;


	gettimeofday(&stop_time, NULL);
	data = &g_data;
	print_infos(data, stop_time);
	exit(EXIT_SUCCESS);
}


int main(int ac, char **av) {
	int	help;

	if (ac == 1 || (help = init_data(&g_data, ac, av)) != 1)
	{
		if (help == 0)
			print_help(av[0]);
		return (1);
	}
	if (g_data.opt & OPT_h)
	{
		print_help(av[0]);
		return (0);
	}
	if (getuid() != 0)
	{
		dprintf(2, "Error: %s: You have to be root in order to use this command.\n", av[0]);
		return (2);
	}
	g_data.pid = getpid();
	g_data.seq = 1;
	if (init_socket(&g_data) == 0)
		return (0);
	printf("PING %s (%s) %d(%zu) bytes of data.\n", g_data.res->ai_canonname, g_data.rp, 64 - 8, 64 + sizeof(struct iphdr));
	signal(SIGALRM, sig_alarm);
	signal(SIGINT, sig_int);
	gettimeofday(&(g_data.start_time), NULL);
	do_send_echo_request(&g_data);
	alarm(1);

	while (1)
		recv_echo_response(&g_data);
}
