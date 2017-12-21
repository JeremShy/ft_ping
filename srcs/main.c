#include <ft_ping.h>

void print_help(char *av)
{
	printf("usage: %s [-h] [-v]\n", av);
}

int init_data(t_data *data, char **av)
{
	data->opt = 0;
	return (1);
}

int main(int ac, char **av) {
	t_data data;

	if (ac == 1)
	{
		print_help(av[0]);
		return (1);
	}
	if (!init_data(&data, av))
	{
		return (1);
	}
	if (data.opt & OPT_v)
	{
		printf("v\n");
	}
}
