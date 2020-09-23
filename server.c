#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>     // read, write
#include <arpa/inet.h>
#include <sys/types.h>  // socket, bind, accept, open
#include <sys/socket.h> // socket, bind, listen, accept
#include <sys/stat.h>   // open
#include <fcntl.h>      // open
#include <errno.h>


//STB_IMAGE
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image/stb_image.h"



#define PORT    5500
#define MAXBUF  1024

//Valores Globales
long red, green, blue = 0;


//Algoritmo Clasificador de Color
void get_image(char *image)
{
	int width, height, channels;
    printf("The image path is: %s\n",image);
	unsigned char *img = stbi_load(image,&width, &height, &channels, 3);

	if (img == NULL){
		printf("Error loading the image\n");
		return;
	}

	printf("Loaded image with a width of %dpx, a heigh of %dpx and %d channels\n", width, height, channels);

    unsigned bytePerPixel = channels;

    for (int i=0; i<height; i++){
        for (int j=0; j<width; j++){
            unsigned char* pixelOffset = img + (i+ height*j)*bytePerPixel;
            unsigned char r = pixelOffset[0];
            unsigned char g = pixelOffset[1];
            unsigned char b = pixelOffset[2];
            greater((int)r,(int)g,(int)b);
        }
    }
    selector(red,green,blue);

    stbi_image_free(img);
}

int greater(int r, int g, int b)
{
    if (r >= g && r >= b){red += 1; return;}
    if (g > r && g > b){green += 1; return;}
    if (b > r && b > g){blue += 1; return;}
}

void selector(long r, long g, long b)
{
    if (r >= g && r >= b){
        printf("La imagen es mayoritariamente roja\n");
        return;
    }
    if (g > r && g > b){
        printf("La imagen es mayoritariamente verde\n");
        return;
    }
    if (b > r && b > g){
        printf("La imagen es mayoritariamente azul\n");
        return;
    }
}


//SERVER INSIDE MAIN
int main() {
    int server_sockfd;
    int client_sockfd;
    int des_fd; // file num
    struct sockaddr_in serveraddr, clientaddr;
    int client_len, read_len, file_read_len;    // length
    char buf[MAXBUF];

    int check_bind;
    client_len = sizeof(clientaddr);

    /* socket() */
    server_sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(server_sockfd == -1) {
        perror("socket error : ");
        exit(0);
    }

    /* bind() */
    bzero(&serveraddr, sizeof(serveraddr));
    serveraddr.sin_family       = AF_INET;
    serveraddr.sin_addr.s_addr  = htonl(INADDR_ANY);
    serveraddr.sin_port         = htons(PORT);

    if(bind(server_sockfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) > 0) {
        perror("bind error : ");
        exit(0);
    }

    /* listen */
    if(listen(server_sockfd, 5) != 0) {
        perror("listen error : ");
    }

    while(1) {
        char file_name[MAXBUF]; // local val
        memset(buf, 0x00, MAXBUF);

        /* accept() */
        client_sockfd = accept(server_sockfd, (struct sockaddr *)&clientaddr, &client_len);
        printf("New Client Connect : %s\n", inet_ntoa(clientaddr.sin_addr));

        /* file name */
        read_len = read(client_sockfd, buf, MAXBUF);
        if(read_len > 0) {
            strcpy(file_name, buf);
            printf("%s > %s\n", inet_ntoa(clientaddr.sin_addr), file_name);
        } else {
            close(client_sockfd);
            break;
        }


        // algoritmo_color
        get_image(file_name);


        /* create file */

        des_fd = open(file_name, O_WRONLY | O_CREAT | O_EXCL, S_IRWXU & (~S_IXUSR));
        if(!des_fd) {
            perror("file open error : ");
            break;
        }   
        /* file save */
        while(1) {
            memset(buf, 0x00, MAXBUF);
            file_read_len = read(client_sockfd, buf, MAXBUF);
            write(des_fd, buf, file_read_len);
            if(file_read_len == 0) {
                printf("finish file\n");
                get_image(des_fd);
                break;
            }


        }


        close(client_sockfd);
        close(des_fd);
    }
    close(server_sockfd);
    return 0;
}