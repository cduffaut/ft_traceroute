[Before_final_push]
- faire une VM Linux et tester le code (nottament les err de UDP checksum)
- tester des addr avec la vraie et la reproduction
- relire le sujet voir que tout soit bien respecté

[Traceroute]:
Traceroute permet de suivre les chemins qu'un paquet de données (paquet IP) va prendre pour aller de la machine locale à une autre machine connectée au réseau IP.

Il va permettre d'identifier les routeurs empruntés, indiquer le délai entre chacun des routeurs et les éventuelles pertes de paquets.

Le principe de fonctionnement de Traceroute consiste à envoyer des paquets UDP

Une fois le paquet sonde arrivé à sa destination finale, il reçoit un paquet réponse ayant pour adresse IP source celle de l'interface de l'équipement sondé à travers laquelle est émis le paquet ICMP.

src: https://fr.wikipedia.org/wiki/Traceroute

Traceroute envoie des petits paquets de données vers un port cible via un paramètre Time-to-Live (TTL). TTL ne traite pas d’unité de temps mais des nombre de sauts (hops) qu’un paquet IP peut couvrir sur Internet. Chaque routeur emprunté décrémente le TTL automatiquement de 1.

En tout, trois paquets sont envoyés à chaque port, ce qui explique pourquoi Traceroute affiche pour chaque routeur aussi trois données de temps de réponse.

src: https://www.ionos.fr/digitalguide/serveur/outils/traceroute-et-les-chemins-de-paquets-de-donnees/

Explanation in C of an traceroute program (Windows):
https://www.codeproject.com/Articles/18807/Traceroute-Using-RAW-Socket-UDP

[IP_Packet]
Chaque paquet IP est un message composé de 3 éléments :

L'adresse de l'expéditeur.
L'adresse du destinataire.
Les données du message.

[UDP_Packet]
UDP ajoute simplement des numéros de port sur les paquets IP.

[recvfrom()-sendto()]
recvfrom & sendto:	Obtenir/définir également l'adresse distante (basée sur le paquet).

src: https://build-your-own.org/redis/02_intro_sockets

[savoir_quand_un_packet_UDP_a_atteint_sa_dest]
Lorsqu'un paquet UDP atteint sa destination finale et que le port de destination est fermé (c'est-à-dire qu'aucun service n'écoute sur ce port), la machine de destination renvoie un message ICMP "Port Unreachable

[Checksum_UDP]
https://datatracker.ietf.org/doc/html/rfc1071

[trame_projet]

a. Initialisation et analyse des arguments
b. Création de la socket et configuration
c. Envoi de paquets avec TTL croissant
d. Réception et traitement des réponses ICMP
e. Affichage des résultats
f. Gestion des erreurs et des cas spéciaux

[launch_the_program]
docker build -t ft_traceroute .
docker run --rm -it --cap-add=ALL --name prog_traceroute ft_traceroute /bin/bash

[leaks]
valgrind --leak-check=full ./ft_traceroute google.com

[check_exchanged_packet]
tcpdump -vvv -i any icmp or udp port