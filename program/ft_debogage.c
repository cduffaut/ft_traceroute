#include "../header/ft_traceroute.h"

void	print_udp_packet(t_udp_pckt	*pckt)
{
	printf (YELLOW "uh_sport: " RESET); printf ("%d\n", ntohs(pckt->udph.uh_sport));
	printf (YELLOW "uh_dport: " RESET); printf ("%d\n", ntohs(pckt->udph.uh_dport));
	printf (YELLOW "uh_ulen: " RESET); printf ("%d\n", ntohs(pckt->udph.uh_ulen));
	printf (YELLOW "uh_sum: " RESET); printf ("%d\n", ntohs(pckt->udph.uh_sum));
	printf (YELLOW "data: " RESET); printf ("%s\n", pckt->data);
}

void	print_icmp_packet(struct icmphdr *pckt)
{
    printf("ICMP Type: %u\n", pckt->type);
    printf("ICMP Code: %u\n", pckt->code);
    printf("ICMP Checksum: %u\n", ntohs(pckt->checksum));	
}