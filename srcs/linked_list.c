#include <ft_ping.h>

t_pckt *create_pckt(int seq, float time)
{
	t_pckt *ret;

	ret = malloc(sizeof(t_pckt));
	ret->seq = seq;
	ret->time = time;
	ret->next = NULL;
	return (ret);
}

t_pckt	*add_pckt(t_pckt *lst, t_pckt *pckt)
{
	pckt->next = lst;
	return (pckt);
}

void	print_list(t_pckt	*lst)
{
	int	i;

	i = 0;
	while (lst)
	{
		printf ("[%d] : seq = %d and time = %.1f\n", i, lst->seq, lst->time);
		lst = lst->next;
		i++;
	}
	printf("\n");
}

void print_infos(t_data *data, struct timeval stop_time)
{
	long long diff;

	diff = ((stop_time.tv_sec * 1000  + stop_time.tv_usec / 1000) - (data->start_time.tv_sec * 1000  + data->start_time.tv_usec / 1000));
	printf("\n--- %s ping statistics ---\n", data->res->ai_canonname);
	printf("%d packets transmitted, %d received, %d%% packet loss, time %lldms\n", data->ntransmitted, data->nreceived, (int)(100 - ((float)data->nreceived / data->ntransmitted) * 100), diff);
	printf("rtt min/avg/max/mdev = %.3f/%.3f/%.3f/%.3f ms\n", calc_min(data->lst), calc_avg(data->lst), calc_max(data->lst), calc_mdev(data->lst));
}
