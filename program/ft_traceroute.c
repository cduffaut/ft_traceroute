#include "../header/ft_traceroute.h"

// init the list with null values when its needed or other important starting values.
static void	to_null(struct s_bag **list)
{
	(*list)->ip = NULL;
	(*list)->hostname = NULL;
	(*list)->res = NULL;
	(*list)->udp_pckt = NULL;

	(*list)->t_timeout.tv_sec = 5;
	(*list)->t_timeout.tv_usec = 0;
	(*list)->t_sleep.tv_sec = 1;
	(*list)->t_sleep.tv_usec = 0;

	(*list)->dest_reached = false;
	(*list)->sended = false;
	
	(*list)->ttl = 1;
	(*list)->msg_sended = 0;
	(*list)->source_addr = 16777343; // 127.0.0.1
}

// Le code ne check pas si IPV4 ou pas il me semble
int	main(int ac, char **av)
{
	struct s_bag	*list;

	list = (struct s_bag *)malloc(sizeof(struct s_bag));
	if (!list)
		error_and_exit("Error: Failed to allocate memory for the list.", ENOMEM);
	to_null(&list);
	list->len_addr_hostname = sizeof(list->addr_hostname);
	ft_parsing(ac, av, &list);
	list->fd = socket(AF_INET, SOCK_RAW, IPPROTO_UDP);
	if (list->fd < 0)
	{
    	fprintf(stderr, "traceroute: socket: %s\n", strerror(errno));
    	free_list_error_and_exit(&list, "", 1);
	}
	getting_src_addr(&list); // end of parsing and init
	ft_traceroute(&list);
	free_s_bag(&list);
	return (0);
}