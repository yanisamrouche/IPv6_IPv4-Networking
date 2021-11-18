/*
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/ioctl.h>

#include <unistd.h>
#include <fcntl.h>
#include <linux/if.h>
#include <linux/if_tun.h>

int tun_alloc(char *dev)
{
  struct ifreq ifr;
  int fd, err;

  if( (fd = open("/dev/net/tun", O_RDWR)) < 0 ){
    perror("alloc tun");
    exit(-1);
  }

  memset(&ifr, 0, sizeof(ifr));

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
*/
/*
void copyPacket(char *src, int dst){
    int tunfd;
    char buffer[2000];
    int response;
    tunfd = tun_alloc(src);
    while(1){
        response = read(tunfd, buffer, 2000);
        if(response < 0 ){
            perror("Reading from interface");
            close(tunfd);
            exit(1);
        }
        printf("Lire %d octets depuis le peripherique %s\n", response, src);
        write(dst, buffer, response);
    }
}
*/
/*
int main (int argc, char** argv){
   copyPacket(argv[1], 1);
   getchar();
   return 0;
}
*/