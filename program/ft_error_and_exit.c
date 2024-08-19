#include "../header/ft_traceroute.h"

void	error_and_exit(char *str, int code)
{
	printf ("%s\n", str);
	exit(code);
}

char 	*free_str(char *str)
{
	if (str)
	{
		free (str);
		str = NULL;
	}
	return (NULL);
}

void	free_str_error_and_exit(char *str, char *msg, int code)
{
	if (str)
	{
		free(str);
		str = NULL;
	}
	printf ("%s\n", msg);
	exit(code);
}

void	free_list_error_and_exit(struct s_bag **list, char *str, int code)
{
	free_s_bag(list);
	printf ("%s", str);
	exit (code);
}

void	ft_free_and_null(void **ele)
{
	if (*ele)
	{
		free (*ele);
		*ele = NULL;
	}
}

// closer la socket!
void	free_s_bag(struct s_bag **list)
{
	if (*list)
	{
		if ((*list)->ip)
			free ((*list)->ip);
		if ((*list)->udp_pckt)
			free ((*list)->udp_pckt);
		if ((*list)->res)
			free ((*list)->res);
		if ((*list)->fd)
			close ((*list)->fd);
		free (*list);
	}
}