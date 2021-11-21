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


char tun[256];
char inip[256];
char inport[256];
char inopt[256];
char outip[256];
char outport[256];

int tun_alloc(char *dev)
{
  struct ifreq ifr;
  int fd, err;

  if( (fd = open("/dev/net/tun", O_RDWR)) < 0 ){
    perror("alloc tun");
    exit(-1);
  }

  memset(&ifr, 0, sizeof(ifr));

  /* Flags: IFF_TUN   - TUN device (no Ethernet headers) 
   *        IFF_TAP   - TAP device  
   *
   *        IFF_NO_PI - Do not provide packet information  
   */ 
  ifr.ifr_flags = IFF_TUN; 
  if( *dev )
    strncpy(ifr.ifr_name, dev, IFNAMSIZ);

  if( (err = ioctl(fd, TUNSETIFF, (void *) &ifr)) < 0 ){
    close(fd);
    return err;
  }
  strcpy(dev, ifr.ifr_name);
  return fd;
}
/*
char copyPacket(int src, int dst){

    char buffer[100];
    while(1){
          read(src, buffer, sizeof(buffer));
          write(1, buffer, sizeof(buffer));
      }
     return buffer;

}
*/
//Read du configuration.txt
void read_file_configuration(char *filename) {
	FILE *config_file = fopen(filename, "r");
	if (config_file == NULL) {
		perror("Erreur fichier");
		exit(0);
	}
	fscanf(config_file, "tun=%s\n", tun);
	fscanf(config_file, "inip=%s\n", inip);
	fscanf(config_file, "inport=%s\n", inport);
	fscanf(config_file, "outip=%s\n", outip);
	fscanf(config_file, "outport=%s\n", outport);
	printf("---CONFIGURATION---\ntun=%s\ninip=%s\ninport=%s\noptions=%s\noutip=%s\noutport=%s\n",
		tun, inip, inport, inopt, outip, outport);
}

int main (int argc, char** argv){
  //char response;
  int tunfd;
  printf("Création de %s\n",tun);
  printf("Faire la configuration de %s...\n",tun);
  printf("Appuyez sur une touche pour continuer\n");
  getchar();
  printf("Interface %s Configurée:\n",tun);
  system("ip addr");
  printf("Appuyez sur une touche pour terminer\n");
  getchar();

  //response = copyPacket(tunfd, 1);
  read_file_configuration(argv[1]);
    tunfd = tun_alloc(tun);
	int f = fork();
	if (f != 0) {
		//Serveur
		printf("-----SERVEUR-----");
		ext_out(inport, tunfd);
		kill(f, SIGKILL);
	}
	else {
		//Client
		printf("CLIENT : %s (port : %s)\n-----Entrer pour continuer-----\n", outip, outport);
		getchar();
		ext_in(outip, outport, tunfd);
		kill(getppid(), SIGKILL);
	}
  return 0;
}
