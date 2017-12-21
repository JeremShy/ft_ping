#include <ft_ping.h>

void print_help(char *av)
{
	printf("usage: %s [-h] [-v]\n", av);
}

int init_data(t_data *data, int ac, char **av)
{
	int	i;
	int	j;

	data->opt = 0;
	i = 1;
	while (i < ac && !ft_strequ(av[i], "--"))
	{
		if (av[i][0] == '-')
		{
			j = 1;
			while (av[i][j])
			{
				if (av[i][j] == 'h')
					data->opt |= OPT_h;
				else if (av[i][j] == 'v')
					data->opt |= OPT_v;
				j++;
			}
		}
		i++;
	}
	return (1);
}

int main(int ac, char **av) {
	t_data data;

	if (ac == 1)
	{
		print_help(av[0]);
		return (1);
	}
	if (!init_data(&data, ac, av))
	{
		return (1);
	}
	if (data.opt & OPT_v)
	{
		printf("v\n");
	}
	if (data.opt & OPT_h)
	{
		printf("h\n");
	}
}
