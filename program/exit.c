#include "../includes/ft_traceroute.h"

void    close_fd_and_exit(int send, int recv, int code)
{
    close (send);
    close (recv);
    if (code == EXIT_SUCCESS)
        exit (code);
}

void    print_and_exit(char *str, int code)
{
    printf ("%s", str);
    exit (code);
}