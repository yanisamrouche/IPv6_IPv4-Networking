#include <stdlib.h>
#include <stdio.h>
#include <string.h>


#include <netdb.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>

#include <unistd.h>
#include <sys/stat.h>
#include <sys/ioctl.h>

#include <fcntl.h>
#include <linux/if.h>
#include <linux/if_tun.h>
#include <stdbool.h>

int copySrcOnDst(int src, int dst){
    char buffer[2000]; /*taille doit être au min celle du MTU (1500) ???*/
    int nb_bytes_lues = 0;
    nb_bytes_lues = read(src, buffer, 2000);
    int i;
    for(i=0; i<2000; i++)
        printf("%c",buffer[i]);
    printf("\n");
    FILE* fichier = NULL;
    fichier = fopen("test.txt", "a");
    if (fichier != NULL)
    {
        fputs(buffer, fichier);
        if(dst==1)
            fprintf(stdout,"%s",buffer);
        fclose(fichier);
    }
    return nb_bytes_lues;
}

int copySrcOnDstv2(int sockfd, int src){
	unsigned char buff[2000];
	int n;
	for (;;) {
		bzero(buff, sizeof(buff));
		//printf("Enter the string : ");
		n = 0;
		read(src, buff, 2000);
		/*for (n=0; n<1900; n++)
			buff[n]='a';*/
		fprintf(stdout,"contenu du buffer \n");
		int i;
		for(i=0; i<2000; i++)
			printf("%c",buff[i]);
		printf("\n");
		printf("sockfd %d , src %d \n", sockfd, src);
		//while ((buff[n++] = getchar()) != '\n')
		//;
		write(sockfd, buff, sizeof(buff));
		bzero(buff, sizeof(buff));

	}

	return 0;
}
