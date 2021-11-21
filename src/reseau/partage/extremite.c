#include "extremite.h"
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <linux/if.h>
#include <linux/if_tun.h>

void ext_out(char *port, int tunnel) {
	int s; /* descripteur de socket */
	struct addrinfo *resol; /* résolution */

	struct addrinfo indic = { AI_PASSIVE, /* Toute interface */
						   PF_INET6,   SOCK_STREAM, 0, /* IP mode connecté */
						   0,          NULL,        NULL, NULL };
	struct sockaddr_in6 client; /* adresse de socket du client */
	client.sin6_addr = in6addr_any;

	fprintf(stderr, "En écoute sur le port %s\n", port);
	int err = getaddrinfo(NULL, port, &indic, &resol);
	if (err < 0) {
		fprintf(stderr, "Resolution: %s\n", gai_strerror(err));
		exit(2);
	}

	/* Creation de la socket, de type TCP / IP */
	if ((s = socket(resol->ai_family, resol->ai_socktype, resol->ai_protocol)) < 0) {
		perror("Socket allouée");
		exit(3);
	}
	fprintf(stderr, "Socket numéro : %i\n", s);

	/* On rend le port reutilisable rapidement /!\ */
	int on = 1;
	if (setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0) {
		perror("option socket");
		exit(4);
	}
	fprintf(stderr, "Option(s) OK!\n");

	/* Association de la socket s a l'adresse obtenue par resolution */
	if (bind(s, resol->ai_addr, sizeof(struct sockaddr_in6)) < 0) {
		perror("bind");
		exit(5);
	}
	freeaddrinfo(resol); /* /!\ Liberation mémoire */
	fprintf(stderr, "bind!\n");

	/* la socket est prete a recevoir */
	if (listen(s, SOMAXCONN) < 0) {
		perror("listen");
		exit(6);
	}
	fprintf(stderr, "listen!\n");

	int len = sizeof(struct sockaddr_in6);

	int n;
	if ((n = accept(s, (struct sockaddr *)&client, (socklen_t *)&len)) < 0) {
		perror("accept");
		exit(7);
	}

	/* Nom reseau du client */
	char hoteclient[NI_MAXHOST];
	char portclient[NI_MAXSERV];
	err = getnameinfo((struct sockaddr *)&client, len, hoteclient, NI_MAXHOST, portclient, NI_MAXSERV, 0);
	if (err < 0) {
		fprintf(stderr, "résolution client (%i): %s\n", n, gai_strerror(err));
	}
	else {
		fprintf(stderr, "accept! (%i) ip=%s port=%s\n", n, hoteclient, portclient);
	}
	/* ajout du code de la copie standard */
	int response_read;
	char buffer[1024];
	while(1){
	    response_read = recv(n, buffer, sizeof(buffer), 0);
	    if(response_read < 0){
	        printf("%d",response_read);
	        break;
	    }
	    for(int i=0; i < response_read; i++){
	        printf("%c",buffer[i]);
	    }
	    printf("\n");
	    write(tunnel, buffer, response_read);
	}
	close(n);
}

//Client
void ext_in(char* addr, char* port, int tunnel) {
	char ip[NI_MAXHOST];    /* adresse IPv4 en notation pointee */
	struct addrinfo *resol; /* struct pour la resolution de nom */
	int s;                  /* descripteur de socket */

	if (port == NULL || addr == NULL) {
		printf("Usage: client + hote + port\n");
		exit(1);
	}

	/* Resolution de l'hote */

	if (getaddrinfo(addr, port, NULL, &resol) < 0) {
		perror("résolution adresse");
		exit(2);
	}

	/* On extrait l'addresse IP */
	// sprintf(ip, "%s",
		 // inet_ntoa(((struct sockaddr_in *)resol->ai_addr)->sin_addr));
	/* Convertir des adresses IPv4 et IPv6 sous forme binaire en texte  */
	sprintf(ip, "%s",
		inet_ntop(resol->ai_family, resol->ai_addr, ip, NI_MAXHOST));

	/* Creation de la socket, de type TCP / IP */
	/* On ne considere que la premiere adresse renvoyee par getaddrinfo */
	if ((s = socket(resol->ai_family, resol->ai_socktype, resol->ai_protocol)) < 0) {
		perror("allocation de socket");
		exit(3);
	}
	fprintf(stderr, "le n° de la socket est : %i\n", s);
	/* Connexion */
	fprintf(stderr, "Essai de connexion à %s (%s) sur le port %s\n\n", addr,ip, port);
	if (connect(s, resol->ai_addr, sizeof(struct sockaddr_in6)) < 0) {
		perror("connexion");
		exit(4);
	}
	freeaddrinfo(resol); /* /!\ Liberation memoire */

	/* Session */

    /* partie modifiée */
	char buffer[1024];
	int response_read;

	while (1) {
		response_read = read(tunnel, buffer, sizeof(buffer));
		if (response_read <= 0) {
			printf("%d",response_read);
			break;
		}
		for (int i = 0; i < response_read; i++) {
			printf("%c", buffer[i]);
		}
		printf("\n");
        send(s, buffer, response_read, 0);
	}
	close(s);

}