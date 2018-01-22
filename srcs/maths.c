#include <ft_ping.h>

float calc_min(t_pckt *lst)
{
	float min;

	if (lst)
		min = lst->time;

	while (lst)
	{
		if (lst->time < min)
			min = lst->time;
		lst = lst->next;
	}
	return (min);
}

float calc_max(t_pckt *lst)
{
	float max;

	if (lst)
		max = lst->time;

	while (lst)
	{
		if (lst->time > max)
			max = lst->time;
		lst = lst->next;
	}
	return (max);
}

float calc_avg(t_pckt	*lst)
{
	int	count;
	float	tot;

	count = 0;
	tot = 0;
	while (lst)
	{
		tot += lst->time;
		count++;
		lst = lst->next;
	}
	return (tot / count);
}

float abso(float val)
{
	if (val > 0)
		return (val);
	return (val * -1);
}

float calc_mdev(t_pckt *lst)
{
	float avg;
	float	tot;
	int	count;

	avg = calc_avg(lst);
	tot = 0;
	count = 0;
	while (lst)
	{
		tot = tot + abso(lst->time - avg);
		count++;
		lst = lst->next;
	}
	return (tot / count);
}

uint16_t invert_s_byte_order(uint16_t nbr)
{
	return ((nbr << 8) | (nbr >> 8));
}

uint16_t ft_ntohs(uint16_t netshort)
{
	return (invert_s_byte_order(netshort));
}

uint16_t ft_htons(uint16_t hostshort)
{
	return (invert_s_byte_order(hostshort));
}
