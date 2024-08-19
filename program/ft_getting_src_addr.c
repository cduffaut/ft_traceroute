#include "../header/ft_traceroute.h"

// WARNING: memory allocation here!
t_udp_pckt	*create_udp_packet(struct s_bag *list)
{
	// cette variable est utilisée plusieurs fois dans le code, il est important de la free et NULL av chaque utilisation
	ft_free_and_null((void **)&list->udp_pckt);

	list->udp_pckt = malloc(sizeof(struct udphdr) + DATA_SIZE);
	if (!list->udp_pckt)
		free_list_error_and_exit(&list, "traceroute: Something went wrong with allocation memory of udp_pckt\n", 1);

	ft_memset(list->udp_pckt->data, 0, sizeof(list->udp_pckt->data));
	list->udp_pckt->udph.uh_sport = htons(S_PORT);							/* source port */
	list->udp_pckt->udph.uh_dport = htons(TRACE_PORT);						/* destination port */
	list->udp_pckt->udph.uh_ulen = htons(sizeof(t_udp_pckt));				/* udp length */
	list->udp_pckt->udph.uh_sum = 0;  										/* clear checksum before calculation */
	list->udp_pckt->udph.uh_sum = ft_checksum_udp(list, list->udp_pckt);	/* udp checksum */
	return (list->udp_pckt);
}

static void	send_udp_packet(struct s_bag *list, t_udp_pckt *udp_pckt)
{
	if (sendto(list->fd, udp_pckt, PACKET_SIZE + DATA_SIZE, 0, (struct sockaddr *)&list->addr_hostname, sizeof(list->addr_hostname)) <= 0)
	{
		fprintf(stderr, "traceroute: socket: %s\n", strerror(errno));
		free_list_error_and_exit(&list, "", 1);
	}
}

// WARNING: memory allocation here!
static char	*receive_icmp_packet(struct s_bag *list)
{
	struct sockaddr_storage sender;
	socklen_t				sender_len = sizeof(sender);
	
	ft_free_and_null((void **)&list->res);

	list->res = malloc(ICMP_SIZE + sizeof(struct ip));  // Allocate enough space for IP header + ICMP
	if (!list->res)
		free_list_error_and_exit(&list, "traceroute: Something went wrong with allocation memory of res icmp packet", 1);
	
	ft_set_socket(list);
	if (recvfrom(list->fd, list->res, ICMP_SIZE + sizeof(struct ip), 0, (struct sockaddr *)&sender, &sender_len) <= 0)
	{
		fprintf(stderr, "traceroute: request timeout for hops: %s\n", strerror(errno));
		free_list_error_and_exit(&list, "", 1);
	}
	return (list->res);
}

static void	treating_icmp_response(char *res, struct s_bag **list)
{
	struct 	ip 		*ip_hdr = (struct ip *)res;
	int 			ip_hdr_len = ip_hdr->ip_hl * 4;
	struct 	icmphdr *recv_icmph = (struct icmphdr *)(res + ip_hdr_len);

	if (recv_icmph->type != ICMP_TIME_EXCEEDED && recv_icmph->type != ICMP_DEST_UNREACH)
	{
		fprintf(stderr, "traceroute: Unexpected ICMP type received: %d\n", recv_icmph->type);
		free_list_error_and_exit(list, "", 1);
	}
	if (ip_hdr->ip_v != 4 || ip_hdr->ip_hl < 5 || ip_hdr->ip_hl > 15)
	{
		fprintf(stderr, "traceroute: Invalid IP header received.\n");
		free_list_error_and_exit(list, "", 1);
	}
	(*list)->source_addr = ip_hdr->ip_src.s_addr;
}

void getting_src_addr(struct s_bag **list)
{
	// créer le paquet UDP à envoyer
	t_udp_pckt	*udp_pckt = create_udp_packet(*list);
	// lancer la requête
	send_udp_packet(*list, udp_pckt);
	// récuperer l'IP sous format binaire de la src (toi <3)
	char *res = receive_icmp_packet(*list);
	treating_icmp_response(res, list);
}