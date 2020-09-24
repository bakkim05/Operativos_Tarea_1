#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <pthread.h>
#define SIZE 1024

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image/stb_image_write.h"

//Valores Globales
long red, green, blue = 0;

void get_image(char *image)
{
	int width, height, channels;
	unsigned char *img = stbi_load(image,&width, &height, &channels, 3);

	if (img == NULL){
		printf("Error loading the image\n");
		return;
	}

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
    selector(red,green,blue, width, height, channels, img);

    red, green, blue = 0;

    stbi_image_free(img);

    return;
}

int greater(int r, int g, int b)
{
    if (r >= g && r >= b){red += 1; return;}
    if (g > r && g > b){green += 1; return;}
    if (b > r && b > g){blue += 1; return;}
}

void selector(long r, long g, long b, int width, int height, int channels, unsigned char* img)
{
    
    if (r >= g && r >= b){
        printf("La imagen es mayoritariamente roja\n");
        stbi_write_jpg("red/red.jpg",width,height,channels,img,100);
        return;
    }
    if (g > r && g > b){
        printf("La imagen es mayoritariamente verde\n");
        stbi_write_jpg("blue/blue.jpg",width,height,channels,img,100);
        return;
    }
    if (b > r && b > g){
        printf("La imagen es mayoritariamente azul\n");
        stbi_write_jpg("blue/blue.jpg",width,height,channels,img,100);
        return;
    }
}

// void *recieve_image (int new_sock){
//     printf("[ ]Reading Picture Size\n");
//     int size;
//     read(new_sock, &size, sizeof(int));

//     printf("[ ]Reading Picture Byte Array\n");
//     char p_array[size];
//     read(new_sock, p_array, size);

//     printf("[ ]Converting Byte Array to Picture\n");
//     FILE *image;
//     image = fopen("temp.png", "w");
//     fwrite(p_array, 1, sizeof(p_array),image);
//     fclose(image);

//     printf("[+]Data written in the file successfully.\n");

//     return NULL;
// }



int main(){
    char *ip = "127.0.0.1";
    int port = 8080;
    int e;

    int sockfd, new_sock;
    struct sockaddr_in server_addr, new_addr;
    socklen_t addr_size;

    // pthread_t tid;


    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0) {
        perror("[-]Error in socket");
        exit(1);
    }
    printf("[+]Server socket created successfully.\n");

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = port;
    server_addr.sin_addr.s_addr = inet_addr(ip);

    e = bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr));
    if(e < 0) {
        perror("[-]Error in bind");
        exit(1);
    }
    printf("[+]Binding successfull.\n");

    if(listen(sockfd, 10) == 0){
        printf("[+]Listening....\n");
    }else{
        perror("[-]Error in listening");
        exit(1);
    }

    addr_size = sizeof(new_addr);
    new_sock = accept(sockfd, (struct sockaddr*)&new_addr, &addr_size);

    // while((new_sock = accept(sockfd, (struct sockaddr*)&new_addr, &addr_size)) > 0){
    //     recieve_image(&new_sock);
    //     // pthread_create(&tid,NULL,recieve_image,&new_sock);
    //     // pthread_join(tid,NULL);
    // }

    printf("[ ]Reading Picture Size\n");
    int size;
    read(new_sock, &size, sizeof(int));

    printf("[ ]Reading Picture Byte Array\n");
    char p_array[size];
    read(new_sock, p_array, size);

    printf("[ ]Converting Byte Array to Picture\n");
    FILE *image;
    image = fopen("temp.png", "w");
    fwrite(p_array, 1, sizeof(p_array),image);
    fclose(image);

    printf("[+]Data written in the file successfully.\n");

    get_image("temp.png");

    return 0;
}
