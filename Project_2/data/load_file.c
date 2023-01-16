#include <stdio.h>
#include <string.h>
#include <limits.h>
#define STB_IMAGE_IMPLEMENTATION
#include "./headers/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "./headers/stb_image_write.h"
typedef struct pixel{
    int x;
    int y;
}pixel; 

unsigned char *turn_grey(unsigned char *image, int width, int height, int channel)
{
    size_t image_size= width*height*channel;
    int grey_channel = (channel == 4) ? 2 : 1;
    size_t grey_image_size= width*height*grey_channel;
 
    unsigned char *grey_image= malloc(grey_image_size);

    for(unsigned char *p = image, *pg = grey_image; p != image + image_size; p += channel, pg += grey_channel) {
        *pg = (uint8_t)((*p + *(p + 1) + *(p + 2))/3.0);
        if(channel == 4) {*(pg + 1) = *(p + 3);
        }
    }
    return grey_image;
}
pixel matching_SAD(unsigned char *image_main, unsigned char* image_sub, int width_main, int height_main, int channel_main, int width_sub, int height_sub, int channel_sub){
    long long int minSAD = LONG_LONG_MAX;
    pixel tmp;
    int x_res= 0, y_res= 0;

    // loop through the search image
    for ( size_t x = 0; x <= height_main - height_sub; x++ ) {
        for ( size_t y = 0; y <= width_main - width_sub; y++ ) {
            long int SAD = 0;

            // loop through the template image
            for ( size_t i = 0; i < height_sub; i++ ) {
                for ( size_t j = 0; j < width_sub; j++ ){
                        SAD+= abs( image_main[(x+i)*width_main + (y+j)] - image_sub[i*width_sub + j]);
                }
            }

            // save the best found position 
            if ( minSAD > SAD ) { 
                minSAD = SAD;
                // give me min SAD
                x_res = x;
                y_res = y;
            }
        }
        
    } 

    tmp.x= x_res; 
    tmp.y= y_res;
    return tmp;
}
pixel matching_cosine_similarity(unsigned char *image_main, unsigned char* image_sub, int width_main, int height_main, int channel_main, int width_sub, int height_sub, int channel_sub){
    
    pixel res;
    res.x= 0;res.y= 0;
    double max_val= 0, tmp= 0;
    
    // loop through the search image
    for ( size_t x = 0; x <= height_main - height_sub; x++ ) {
        for ( size_t y = 0; y <= width_main - width_sub; y++ ) {
            double numerator= 0, denominator_main= 0, denominator_sub= 0;

            for ( size_t i = 0; i < height_sub; i++ ) {
                for ( size_t j = 0; j < width_sub; j++ ){
                    int A= image_main[(x+i)*width_main + (y+j)], B= image_sub[i*width_sub + j];

                    numerator+= A*B;
                    denominator_main+= A*A;
                    denominator_sub+= B*B;
                }
            }

            // save the best found position 
            tmp= numerator/(sqrt(denominator_main)*sqrt(denominator_sub));
            if ( max_val < tmp ) { 
                max_val= tmp;
                // give me min SAD
                res.x = x;
                res.y = y;
            }
        }
        
    } 

    return res;
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
int main ()
{
    int test_case= 0; 
    while(test_case <= 62){
        int width_main, height_main, channel_main, channel_tmp;
        int width_sub, height_sub, channel_sub;
        char str[3];

        sprintf(str, "%d", test_case);
        char img_main[20];
        sprintf(img_main, "%s%d%s", "./images/img", test_case, ".jpg");
        char img_sub[]= "temp.jpg" ;
        char save_path[20];
        sprintf(save_path, "%s%d%s", "output", test_case, ".jpg");
        test_case++;

        // read image data
        unsigned char *image_main = stbi_load (img_main, &width_main, &height_main, &channel_main, 0);
        unsigned char *image_sub = stbi_load (img_sub, &width_sub, &height_sub, &channel_sub, 0);

        if (image_main == NULL || image_sub == NULL)
        {
            printf ("\nError in loading the image\n");
            exit (1);
        }

        
        unsigned char *image_tmp= turn_grey(image_main, width_main, height_main, channel_main);
        channel_tmp= (channel_main == 4) ? 2:1;
        
        image_sub= turn_grey(image_sub, width_sub, height_sub, channel_sub);
        channel_sub= (channel_sub == 4) ? 2:1;
        
        pixel result= matching_consine_similarity(image_tmp, image_sub, width_main, height_main, channel_tmp, width_sub, height_sub, channel_sub);
        
        printf("%d %d\n", result.x, result.y);
        //draw_point(image_main, width_main, height_main, channel_main, result.x, result.y);
        draw(image_main, width_main, height_main, channel_main, width_sub, height_sub, result.x, result.y);
        
        stbi_write_png (save_path, width_main, height_main, channel_main, image_main, width_main * channel_main);
        
        free(image_main);
        free(image_sub);
        free(image_tmp);
        printf("Done test_case %dth!\n", test_case);
    }
    return 0;
}
