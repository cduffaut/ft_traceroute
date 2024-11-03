# ifndef FT_TRACEROUTE_H
# define FT_TRACEROUTE_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>
#include <sys/time.h>
#include <netinet/ip_icmp.h>
#include <netinet/ip.h>
#include <netinet/udp.h>

#define TRACE_PORT 33434
#define MAX_HOPS 30
#define PACKET_SIZE 68

// libft.c
int	    ft_strcmp(const char *s1, const char *s2);
void	*ft_memset(void *s, int c, size_t n);
void	*ft_memcpy(void *dest, const void *src, size_t n);

// parser.c
void    parser(int ac, char **av);

// exit.c
void    close_fd_and_exit(int send, int recv, int code);
void    print_and_exit(char *str, int code);

// utils.c
int     set_upd_socket();
int     set_icmp_socket();
void    print_ttl(int ttl);
bool    display_hops_name(char *hostname, char *ip);
bool    print_timeout(char *timeout, int i);

#endif