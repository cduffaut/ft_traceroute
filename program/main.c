#include "../includes/ft_traceroute.h"

int main(int ac, char **av)
{
    parser(ac, av);

    struct addrinfo hints;
    struct addrinfo *res;
    ft_memset(&hints, 0, sizeof(hints));

    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM; // to specify type of comunication for the socket (UDP)

    char    *target = av[1];
    if (getaddrinfo(target, NULL, &hints, &res) != 0)
    {
        printf("%s: Name or service not known\nCannot handle \"host\" cmdline arg '%s' on position 1 (argc 1)\n", target, target);
        exit (EXIT_FAILURE);
    }

    struct sockaddr_in  addr_target;
    ft_memset(&addr_target, 0, sizeof(addr_target));
    
    addr_target.sin_family = AF_INET;
    addr_target.sin_port = htons(TRACE_PORT);
    ft_memcpy(&addr_target.sin_addr, &((struct sockaddr_in *)res->ai_addr)->sin_addr, sizeof(struct in_addr));
    freeaddrinfo(res);

    inet_pton(PF_INET, target, &(addr_target.sin_addr)); // convert IPv4 address from text to binary

    int fd_send = set_upd_socket();
    int fd_recv = set_icmp_socket();

    printf ("traceroute to %s (%s), %d hops max, %d byte packets\n", target, inet_ntoa(addr_target.sin_addr), MAX_HOPS, PACKET_SIZE - 8);

    int ttl = 0;
    while (ttl++ < MAX_HOPS)
    {
        struct  timeval     start[3];
        struct  timeval     end[3];
        struct sockaddr_in  hops_addr;
        ft_memset(&hops_addr, 0, sizeof(hops_addr));

        if (setsockopt(fd_send, IPPROTO_IP, IP_TTL, &ttl, sizeof(ttl)) != 0) // set the ttl option to the UDP socket
            print_and_exit ("traceroute: setsockopt() function failed for UDP\nStopping the program...\n", EXIT_FAILURE);

        print_ttl(ttl);
        bool not_printed = false;
        for (int i = 0; i < 3; i++)
        {
            struct timeval  timeout;
            char            packet[PACKET_SIZE];

            ft_memset(&packet, 0, sizeof(packet));
            gettimeofday(&start[i], NULL);
            addr_target.sin_port = htons(TRACE_PORT + ttl + i); // we avoid error by confusing the host with 3 packets sended to the same addr

            if (sendto(fd_send, packet, sizeof(packet), 0, (struct sockaddr *)&addr_target, sizeof(struct sockaddr)) < 0)
                print_and_exit ("traceroute: sendto() function failed for UDP\nStopping the program...\n", EXIT_FAILURE);

            timeout.tv_sec = 1;
            timeout.tv_usec = 0;

            fd_set  monitor;
            FD_ZERO(&monitor); // empty the fd_set, put it to NULL
            FD_SET(fd_recv, &monitor); // add an fd to the set to monitor it

            int tmp_res = select(fd_recv + 1, &monitor, NULL, NULL, &timeout); // first arg is the nbr of fd to look at so we add recv to include it in the monitoring
            if (tmp_res < 0)
                print_and_exit ("traceroute: select() function failed for ICMP\nStopping the program...\n", EXIT_FAILURE);
            else if (tmp_res == 0)
            {
                if (i != 0)
                    printf (" ");
                not_printed = print_timeout ("* ", i);
            }
            else
            {
                if (FD_ISSET(fd_recv, &monitor))
                {
                    char                buf[PACKET_SIZE];
                    struct sockaddr_in  res_addr;
                    socklen_t           res_addr_len = sizeof(res_addr);

                    ssize_t recvfrom_len = recvfrom(fd_recv, buf, sizeof(buf), 0, (struct sockaddr *)&res_addr, &res_addr_len);
                    if (recvfrom_len < 0)
                        print_and_exit ("traceroute: recvfrom() function failed for ICMP\nStopping the program...\n", EXIT_FAILURE);

                    gettimeofday(&end[i], NULL);
                    struct hostent *get_host = gethostbyaddr(&res_addr.sin_addr, sizeof(struct in_addr), AF_INET); // returns a structure of type hostent for the given host name
                    char    *hops_hostname = NULL;
                    if (get_host != 0)
                        hops_hostname = get_host->h_name; // official name of host
                    else
                        hops_hostname = inet_ntoa(res_addr.sin_addr); // converts the Internet host address to a string in IPv4 dotted-decimal notation
                    if (i == 0 || not_printed == true)
                        not_printed = display_hops_name(hops_hostname, inet_ntoa(res_addr.sin_addr));
                    printf (" %.3Lf ms ", (long double)(end[i].tv_sec - start[i].tv_sec) * 1000.0 + (end[i].tv_usec - start[i].tv_usec) / 1000.0);
                    hops_addr = res_addr;
                }
                else
                    not_printed = print_timeout (" * ", i);
            }
        }
        printf ("\n");
        if (hops_addr.sin_addr.s_addr == addr_target.sin_addr.s_addr)
            close_fd_and_exit(fd_send, fd_send, EXIT_SUCCESS);
    }
    close_fd_and_exit(fd_send, fd_send, 777);
    return (0);
}