#include <ft_ping.h>

void print_help(char *av)
{
	printf("usage: %s:\n\t\t[-h]: help\n\t\t[-v]: verbose\n", av);
}

int init_data(t_data *data, int ac, char **av)
{
	int	i;
	int	j;
	char *rhost;
	int	options_ended;

	rhost = NULL;
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
	if (!rhost)
		return (0);
	data->rhost = rhost;
	return (1);
}

int main(int ac, char **av) {
	t_data data;

	if (ac == 1 || !init_data(&data, ac, av))
	{
		print_help(av[0]);
		return (1);
	}
	if (data.opt & OPT_h)
	{
		print_help(av[0]);
		return (1);
	}
	printf("remote host : [%s]\n", data.rhost);
	create_socket(&data);
}
