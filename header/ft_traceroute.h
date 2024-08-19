# ifndef FT_TRACEROUTE_H
# define FT_TRACEROUTE_H

#pragma once

#define DEBOG 	printf("\n\e[1;32m OK: %d, in %s, in the file: %s\n\e[0m\n", __LINE__, __func__, __FILE__);
#define NO_PORT 33434
#define PACKET_SIZE 8
#define ICMP_SIZE 32
#define DATA_SIZE 32
#define MAX_HOPS 30 // By default, the upper limit is 30 hops
#define TRACE_PORT 33434
#define S_PORT 6969
#define MS_TIMEOUT 5000

// Colors Set
#define BLACK "\e[1;30m"
#define RED "\e[1;31m"
#define GREEN "\e[1;32m"
#define YELLOW "\e[1;33m"
#define BLUE "\e[1;34m"
#define PINK "\e[1;35m"
#define CYAN "\e[1;36m"
#define RESET "\e[0m"

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

// indispensable pour le calcul du checksum UDP !
typedef struct s_pseudo_header
{
    u_int32_t 		source_address;
    u_int32_t 		dest_address;
    u_int8_t 		placeholder;
    u_int8_t 		protocol;
    u_int16_t 		udp_length;
	struct udphdr	udph;
	char			data[DATA_SIZE];
}				t_pseudo_header;

// total UDP packet
typedef struct s_udp_pckt
{
	struct udphdr	udph;
	char			data[DATA_SIZE];
} 				t_udp_pckt;

// main list of the program
typedef struct s_bag
{
	struct	sockaddr_in	addr_hostname;

	int					len_addr_hostname;
	int					fd;
	int					msg_sended;
	int 				ttl;

	char				*ip; 			// adresse sous format string: "127.0.0.1"
	char				*hostname;
	char 				ip_str[INET_ADDRSTRLEN];
	char 				*res;

	t_udp_pckt			*udp_pckt;

	uint32_t			source_addr;	// binary format of the src addr (you <3)

	struct timeval		t_timeout;
	struct timeval		t_sleep; // à supp ?
	struct timeval		p_start[3];
	struct timeval		p_end[3];

	bool 				dest_reached;
	bool 				sended;
} t_bag;

// ft_request.c
void	ft_traceroute(struct s_bag	**list);
u_short ft_checksum_udp(struct s_bag *list, t_udp_pckt *send_pckt);
void	ft_set_socket(struct s_bag	*list);

// ft_traceroute.c
void	free_s_bag(struct s_bag **list);

// ft_parsing.c
void	ft_parsing(int ac, char **av, struct s_bag **list);

// ft_error_and_exit.c
void	free_list_error_and_exit(struct s_bag **list, char *str, int code);
void	free_str_error_and_exit(char *str, char *msg, int code);
void	error_and_exit(char *str, int code);
char 	*free_str(char *str);
void	ft_free_and_null(void **ele);

// ft_getting_src_addr.c
void getting_src_addr(struct s_bag **list);
t_udp_pckt	*create_udp_packet(struct s_bag *list);

// ft_utils_libft.c
void	*ft_memset(void *s, int c, size_t n);
int		ft_strcmp(const char *s1, const char *s2);
char	*ft_strcpy(char *dest, char *src);
char	*ft_strdup(const char *s);
void	*ft_memcpy(void *dest, const void *src, size_t n);
size_t	ft_strlen(const char *s);

// ft_debogage.c
void	print_udp_packet(t_udp_pckt	*pckt);
void	print_icmp_packet(struct icmphdr *pckt);

#endif