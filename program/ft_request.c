#include "../header/ft_traceroute.h"

unsigned short ft_checksum(void *ele, int len)
{
	unsigned short	*buf = (unsigned short *)ele;
	unsigned short	res = 0;
	unsigned int	sum = 0;

	while (len > 1)
	{
		sum += *buf++;
		len -= 2;
	}
	if (len == 1)
		sum += *(unsigned char *)buf;
	sum = (sum >> 16) + (sum & 0xFFFF);
	sum += (sum >> 16);
	res = (unsigned short)~sum;
	return (res);
}

// return the hostname of the ip or NULL
static char	*ft_return_hostname(const struct sockaddr *sender)
{
	int 	res = 0;
	char	buf[255]; // max size fo an FQDN
	res = getnameinfo(sender, sizeof(struct sockaddr_in), buf, NI_MAXHOST, NULL, 0, 0);
	if (res) // Something went wrong
		return (NULL);

	char *hostname = ft_strdup(buf);
	if (hostname == NULL)
		return (NULL);
	return (hostname);
}

u_short	ft_checksum_udp(struct s_bag *list, t_udp_pckt *send_pckt)
{
	t_pseudo_header pheader;
	// On enleve le sizeof du tableau et on ne garde que la taille réelle du tableau (DATA_SIZE)
	int size_t_pseudo_header = sizeof(t_pseudo_header) - sizeof(pheader.data) + DATA_SIZE;

	ft_memset(&pheader, 0, sizeof(pheader));
	ft_memcpy(&pheader.udph, &send_pckt->udph, sizeof(struct udphdr));
	ft_memcpy(pheader.data, send_pckt->data, DATA_SIZE);
	
	uint32_t ip_big_endian = 0;
	int ret = inet_pton(AF_INET, list->ip, &ip_big_endian);
	if (ret <= 0)
	{
		printf ("traceroute: unknown host %s\n", list->hostname);
		free_list_error_and_exit(&list, NULL, 1);
	}

	pheader.source_address = list->source_addr;
	pheader.dest_address = ip_big_endian;
	pheader.placeholder = 0;
	pheader.protocol = IPPROTO_UDP;
	pheader.udp_length = htons(sizeof(struct udphdr) + DATA_SIZE); // le udpheader + la data
	
	return (ft_checksum(&pheader, size_t_pseudo_header));
}

void	ft_set_socket(struct s_bag	*list)
{
	if (setsockopt(list->fd, SOL_IP, IP_TTL, &list->ttl, sizeof(list->ttl)) < 0)
		error_and_exit(RED "traceroute: Something went wrong with setsockopt IP_TTL\n", 1);
	if (setsockopt(list->fd, SOL_SOCKET, SO_RCVTIMEO, &list->t_timeout, sizeof(list->t_timeout)) < 0)
		error_and_exit(RED "traceroute: Something went wrong with setsockopt SO_RCVTIMEO\n", 1);
}

static void ft_update_udph(t_udp_pckt **send_pckt, struct s_bag	*list, int i)
{
	(*send_pckt)->udph.uh_sport = htons(S_PORT + i);
	(*send_pckt)->udph.uh_sum = 0;
	(*send_pckt)->udph.uh_sum = ft_checksum_udp(list, *send_pckt);
}

static void	ft_send_udp_packet(struct s_bag **list, t_udp_pckt *send_pckt)
{
	if (sendto((*list)->fd, send_pckt, PACKET_SIZE + DATA_SIZE, 0, (struct sockaddr *)&(*list)->addr_hostname, sizeof((*list)->addr_hostname)) <= 0)
	{
		fprintf(stderr, "traceroute: socket: %s\n", strerror(errno));
		(*list)->sended = false;
	}
	else
	{
		(*list)->msg_sended += 1;
		(*list)->sended = true;
	}
}

// UDP packet reached a router or the final point, displaying the infos...
static void ft_display_stats_traceroute(struct s_bag **list, long double total_trace_time, struct sockaddr_storage sender, char *ip_str, int i)
{
	if (sender.ss_family == AF_INET) // IPv4
	{
		struct sockaddr_in *addr_in = (struct sockaddr_in *)&sender;
		inet_ntop(AF_INET, &(addr_in->sin_addr), ip_str, INET_ADDRSTRLEN); // IP under the str format
	}
	if (i == 0)
		printf ("%d\t", (*list)->ttl);
	if (total_trace_time >= MS_TIMEOUT)
		printf ("* ");
	else
	{
		if (i == 0)
		{
			char *hostname = ft_return_hostname((const struct sockaddr *)&sender);
			if (hostname && ip_str)
				printf ("%s (%s)", hostname, ip_str);
			else if (ip_str)
				printf ("%s (%s)", ip_str, ip_str);
			else if (hostname)
				printf ("%s", hostname);
			printf ("\t%.3Lf ms", total_trace_time);
			if (hostname)
				free(hostname);
		}
		else if (i == 1 || i == 2)
			printf ("\t%.3Lf ms", total_trace_time);
	}
	if (i == 2)
		printf ("\n");
}

// static void	handle_recvfrom_error(struct s_bag *list)
// {
// 	//EAGAIN= il n'y a pas de données disponibles pour le moment, réessayez plus tard
// 	//EWOULDBLOCK= l'opération aurait dû être bloquée, mais le descripteur a été placé en mode non bloquant.
// 	if (errno != EAGAIN && errno != EWOULDBLOCK)
// 	{
// 		fprintf(stderr, "traceroute: socket: %s\n", strerror(errno));
// 		free_list_error_and_exit(&list, "", 1);
// 	}	
// }

static	void treating_icmp_response(struct s_bag **list, char *res, struct sockaddr_storage sender, int i)
{
	gettimeofday(&(*list)->p_end[i], NULL);

	double 			trace_time = ((double)((*list)->p_end[i].tv_usec - (*list)->p_start[i].tv_usec) / 1000);
	long double 	total_trace_time = (long double)((((*list)->p_end[i].tv_sec - (*list)->p_start[i].tv_sec) * 1000) + trace_time);

	struct 	ip 		*ip_hdr = (struct ip *)res;
	int 			ip_hdr_len = ip_hdr->ip_hl * 4;
	struct 	icmphdr *recv_icmph = (struct icmphdr *)(res + ip_hdr_len);
	char 			ip_str[INET_ADDRSTRLEN] = {0}; // récup de l'ip du routeur

	if (recv_icmph->type == ICMP_TIME_EXCEEDED)
		ft_display_stats_traceroute(list, total_trace_time, sender, ip_str, i);
	// ---- Here to treat other cases like !X !P etc ----
	else if (recv_icmph->type == ICMP_DEST_UNREACH && recv_icmph->code == ICMP_PORT_UNREACH && i == 2)
	{
		ft_display_stats_traceroute(list, total_trace_time, sender, ip_str, i);
		(*list)->dest_reached = true;
	}
}

void	ft_traceroute(struct s_bag	**list)
{
	t_udp_pckt				*send_pckt;
	char 					res[ICMP_SIZE + sizeof(struct ip)];  // Allocate enough space for IP header + ICMP
	struct sockaddr_storage sender;
	socklen_t				sender_len = sizeof(sender);

	// WARNING: here memory allocation for send_pckt
	send_pckt = create_udp_packet(*list);
	printf ("traceroute to %s (%s), %d hops max, %d byte packets\n", (*list)->hostname, (*list)->ip, MAX_HOPS, PACKET_SIZE);

	while (!(*list)->dest_reached && (*list)->ttl <= MAX_HOPS)
	{
		ft_set_socket(*list);

		int i = 0;
		while (i < 3 && !(*list)->dest_reached)
		{
			ft_update_udph(&send_pckt, *list, i);
			ft_send_udp_packet(list, send_pckt);
			gettimeofday(&(*list)->p_start[i], NULL); 

			recvfrom((*list)->fd, res, sizeof(res), 0, (struct sockaddr *)&sender, &sender_len);
			if ((*list)->sended)
				treating_icmp_response(list, res, sender, i);
			i += 1;
		}
		if ((*list)->dest_reached)
			break ;  
		(*list)->ttl += 1;
	}
	if (send_pckt)
	{
		free (send_pckt);
		(*list)->udp_pckt = NULL; // (*list)->udp_pckt == send_pckt
	}
}