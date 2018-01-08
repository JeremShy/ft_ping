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
