#include "../header/ft_traceroute.h"

static bool ft_check_option(char **av)
{
	if (!(ft_strcmp((const char *)av[1], "--help")))
		return (true);
	else
		error_and_exit("Error: Wrong arguments in command line.\nUsage: traceroute [--help]\nGeneral use: traceroute <destination>\n", 0);
	return (false);
}

static bool ft_check_option_without_msg(char **av)
{
	if (!(ft_strcmp((const char *)av[1], "--help")))
		return (true);
	return (false);
}

static void spread_help_option(struct s_bag **list)
{
	printf ("Usage: traceroute [--help]\nGeneral use: traceroute <destination>\n");
	free_list_error_and_exit(list, 0, 0);
}

// DNS lookup: is the process through which human-readable domain names (www.digicert.com) are 
// translated into a computer-readable IP address (216.168.246.55).
static void	ft_check_hostname(struct s_bag **list)
{
	int 			ret;
	struct 			addrinfo *res;
	struct addrinfo hints;
	char			*buf;

	(*list)->ip = (char *)malloc(sizeof(char) * INET_ADDRSTRLEN);
	if (!(*list)->ip)
		free_list_error_and_exit(list, "Error: memory allocation failed for hostname\n", 1);

	ft_memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_INET; // IPV4
	
	// IPV4 format checking
	ret = getaddrinfo((*list)->hostname, NULL, &hints, &res);
	if (ret != 0 || res == NULL)
	    {printf ("Cannot handle \"host\" cmdline arg \'%s\' on position 1 (argc 1)\n", (*list)->hostname); free_list_error_and_exit(list, "", 1);}
	
	struct sockaddr_in *ipv4 = (struct sockaddr_in *)res->ai_addr;
	buf = inet_ntoa(ipv4->sin_addr); // convert to str IP format, ex: "127.0.0.1"
	if (!buf)
		{freeaddrinfo(res); free_list_error_and_exit(list, "traceroute: problem converting address to string\n", 1);}
	ft_strcpy((*list)->ip, buf);	

	(*list)->addr_hostname.sin_family = res->ai_family;
	(*list)->addr_hostname.sin_port = htons(NO_PORT);
	// assignation de l'ip sous forme binaire à s_addr qui elle aussi prend une IP en binaire
	(*list)->addr_hostname.sin_addr = ipv4->sin_addr;
	freeaddrinfo(res);
}

// general parsing function
void	ft_parsing(int ac, char **av, struct s_bag **list)
{
	if (getuid() != 0)
		free_list_error_and_exit (list, "Error: You're not in root mode,\nthe program cannot run without root permissions.", 0);
	if (ac != 2)
	{
		if (ac <= 1)
			free_list_error_and_exit(list, "Usage: traceroute [--help]\nGeneral use: traceroute <destination>\n", 0);
		if (ft_check_option(av) == true)
			spread_help_option(list);
		else
			free_list_error_and_exit(list, "traceroute: Name or service not known\n", 0);
	}
	if (ft_check_option_without_msg(av) == true)
			spread_help_option(list);
	(*list)->hostname = av[1];
	ft_check_hostname(list);
}