#include "../includes/ft_traceroute.h"

int set_upd_socket()
{
    int fd = socket(PF_INET, SOCK_DGRAM, 0); // SOCK_DGRAM to specifiy UDP comunication
    if (fd < 0)
    {
        printf ("traceroute: socket() function failed for UDP\nStopping the program...\n");
        exit (EXIT_FAILURE);
    }
    return (fd);
}

int set_icmp_socket()
{
    int fd = socket(PF_INET, SOCK_RAW, IPPROTO_ICMP); // set the socket for a ICMP communication (response type waited for traceroute)
    if (fd < 0)
    {
        printf ("traceroute: socket() function failed for ICMP\nStopping the program...\n");
        exit (EXIT_FAILURE);        
    }
    return (fd);
}

void    print_ttl(int ttl)
{
    printf ("%d ", ttl);
    if (ttl < 10)
        printf (" ");
}

bool    display_hops_name(char *hostname, char *ip)
{
    printf ("%s (%s) ", hostname, ip);
    return (false);
}

bool    print_timeout(char *timeout, int i)
{
    printf ("%s", timeout);
    if (i == 0)
        return (true);
    else
        return (false);
}