#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <libgen.h>
#include <string.h>
#define SIZE 1024

int main(int argc, char *argv[]){
    char *ip = argv[1];
    int port = 8080;
    int e;

    int sockfd;
    struct sockaddr_in server_addr;
    char fp[100];

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0) {
        perror("[-]Error in socket");
        exit(1);
    }
    printf("[+]Server socket created successfully.\n");

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = port;
    server_addr.sin_addr.s_addr = inet_addr(ip);

    
    while(1){
        e = connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr));
        if(e == -1) {
            perror("[-]Error in socket");
            exit(1);
        }
        printf("[+]Connected to Server.\n");

    
        printf("Image Directory: ");
        scanf("%s",fp);
        printf("%s\n",fp);

        if (fp == NULL) {
            perror("[-]Error in reading file.");
            exit(1);
        }

        if (strcmp(fp,"fin")==0){
            printf("[+]Closing the connection.\n");
            close(sockfd);
            return 0;
        }

        printf("[ ]Getting Picture Size\n");
        FILE *picture;
        picture = fopen(fp,"r");
        int size;
        fseek(picture, 0, SEEK_END);
        size = ftell(picture);
        fseek(picture,0, SEEK_SET);

        printf("[ ]Sending Picture name\n");
        write(sockfd, basename(fp), sizeof(fp));

        printf("[ ]Sending Picture size\n");
        write(sockfd, &size, sizeof(size));

        printf("[ ]Sending Picture as Byte Array\n");
        char send_buffer[size];
        while(!feof(picture)){
            fread(send_buffer,1,sizeof(send_buffer),picture);
            write(sockfd, send_buffer, sizeof(send_buffer));
            bzero(send_buffer, sizeof(send_buffer));
        }
        
        printf("[+]File data sent successfully.\n");

        close(e);
    }

    return 0;
}