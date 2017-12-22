#include <ft_ping.h>
#include <netdb.h>

int	create_socket(t_data *data)
{
	int	sock;

		sock = socket(AF_INET, SOCK_RAW, 1); // proto ICMP : 1
}
