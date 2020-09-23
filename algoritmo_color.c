#include <stdio.h>
#include <stdlib.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image/stb_image.h"

//Valores Globales
long red, green, blue = 0;

//Funciones
void get_image(char *image);
int greater(int r, int g, int b);
void selector(long r, long g, long b);

//Main
int main(void)
{
    get_image("images/sample.jpeg");
}

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
