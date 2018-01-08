#include <ft_ping.h>

t_data g_data = {0};

void print_help(char *av)
{
	printf("usage: %s:\n\t\t[-h]: help\n\t\t[-v]: verbose\n\t\tdestination\n", av);
}

int init_data(t_data *data, int ac, char **av)
{
	int	i;
	int	j;
	char *rhost;
	int	options_ended;

	rhost = NULL;
	data->lst = NULL;
	data->ntransmitted = 0;
	data->nreceived = 0;
	data->opt = 0;
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
	long long diff;

	data = &g_data;
	gettimeofday(&stop_time, NULL);
	diff = ((stop_time.tv_sec * 1000  + stop_time.tv_usec / 1000) - (data->start_time.tv_sec * 1000  + data->start_time.tv_usec / 1000));
	printf("\n--- %s ping statistics ---\n", data->res->ai_canonname);
	printf("%d packets transmitted, %d received, %d%% packet loss, time %lldms\n", data->ntransmitted, data->nreceived, (int)(100 - ((float)data->nreceived / data->ntransmitted) * 100), diff);
	print_list(data->lst);
	exit(EXIT_SUCCESS);
}


int main(int ac, char **av) {
	if (ac == 1 || !init_data(&g_data, ac, av))
	{
		dprintf (2, "Error : %s: No destination.\n", av[0]);
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
	printf("pid : %d - remote host : [%s]\n", g_data.pid, g_data.rhost);
	if (init_socket(&g_data) == 0)
	{
		dprintf (2, "error in init_socket.\n");
		return (0);
	}
	signal(SIGALRM, sig_alarm);
	signal(SIGINT, sig_int);
	gettimeofday(&(g_data.start_time), NULL);
	do_send_echo_request(&g_data);
	alarm(1);

	while (1)
	{
		recv_echo_response(&g_data);
	}
}
