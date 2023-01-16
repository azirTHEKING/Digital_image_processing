#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

unsigned char *turn_grey(unsigned char *image, int width, int height, int channel)
{
    size_t image_size= width*height*channel;
    int grey_channel = (channel == 4) ? 2 : 1;
    size_t grey_image_size= width*height*grey_channel;
 
    unsigned char *grey_image= malloc(grey_image_size);

    for(unsigned char *p = image, *pg = grey_image; p != image + image_size; p += channel, pg += grey_channel) {
        *pg = (uint8_t)((*p + *(p + 1) + *(p + 2))/3.0);
        if(channel == 4) {
            *(pg + 1) = *(p + 3);
        }
    }
    return grey_image;
}
void draw(unsigned char *image_main, int width_main, int height_main, int channel_main, int width_sub, int height_sub, int x, int y){
    for(int i= x; i < x+height_sub; i++){
        for(int j= 0; j < channel_main; j++){
            image_main[i*width_main*channel_main + y*channel_main + j]= (j != 1)?0:255;
        }
    }
    for(int i= x; i < x+height_sub; i++){
        for(int j= 0; j < channel_main; j++){
            image_main[i*width_main*channel_main + (y+width_sub)*channel_main + j]= (j != 1)?0:255;
        }
    }

    for(int j= y; j < y+width_sub; j++){
        for(int k= 0; k < channel_main; k++){
            image_main[x*width_main*channel_main + j*channel_main + k]= (k != 1)?0:255;   
        }
    }
    for(int j= y; j < y+width_sub; j++){
        for(int k= 0; k < channel_main; k++){
            image_main[(x+height_sub)*width_main*channel_main + j*channel_main + k]= (k != 1)?0:255;
        }
    }

        
}
void update(unsigned char *image_main, unsigned char* image_sub, int width_main, int height_main, int channel_main, int width_sub, int height_sub, int channel_sub, int x, int y){
    for(int i= 0; i < height_sub; i++){
        for(int j= 0; j < width_sub; j++){
            image_sub[i*width_sub + j]= image_main[(i+x)*width_main + (j+y)];
        }
    }

    return;
}