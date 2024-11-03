#include "../includes/ft_traceroute.h"

void    parser(int ac, char **av)
{
    if (getuid() != 0)
    {
        printf ("traceroute: you are not in root mode. Can't execute the program.\n");
        exit (EXIT_FAILURE);
    }
    if (ac < 2)
    {
        printf ("traceroute: wrong number of arguments.\nUsage: traceroute [--help]\nGeneral use: traceroute <destination>\n");
        exit (EXIT_FAILURE);
    }
    if (!ft_strcmp(av[1], "--help"))
    {
        printf ("Usage: traceroute [--help]\nGeneral use: traceroute <destination>\n");
        exit (EXIT_SUCCESS);
    }
}