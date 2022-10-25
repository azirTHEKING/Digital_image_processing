
#include <math.h>
#include <stdio.h>

#define STB_IMAGE_IMPLEMENTATION
#define M_PI 3.14159265358979323846
#include "./headers/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "./headers/stb_image_write.h"

unsigned char * uc_arrayNew_1d ( int _size )
{
    return ( unsigned char *) calloc (_size, sizeof ( unsigned char ));
}

unsigned char * image_rotation ( unsigned char *image, int width, int height, int channel, int degrees )
{
    unsigned char * tary = uc_arrayNew_1d ( width * height * channel );
    float radians = degrees * M_PI / 180.0;
    float xcenter = ( float )( width ) / 2.0;
    float ycenter = ( float )( height ) / 2.0;
    for (int i = 0; i < height ; ++i)
    {
        for (int j = 0; j < width ; ++j)
        {
            for ( int k = 0; k < channel ; k++)
            {
                int rorig = ycenter + (( float )(i)-ycenter ) * cos (- radians ) - ((
                                float )(j)-xcenter ) * sin(- radians );
                int corig = xcenter + (( float )(i)-ycenter ) * sin (- radians ) + ((
                                float )(j)-xcenter ) * cos(- radians );
                if ( rorig >= 0 && rorig < height && corig >= 0 && corig < width )
                {
                    tary [i * width * channel + j * channel + k] = image [ rorig *
                            width * channel + corig * channel + k];
                }
            }
        }
    }
    return tary ;
}
int main ()
{

    int width, height, channel ;
    char path_img [] = "./images/98239648_p0.png";
    char save_path_rotate [] = "./images/98239648_p0-Rotated.png";

// read image data
    unsigned char * image = stbi_load ( path_img, &width, &height, & channel, 0);
    if ( image == NULL )
    {
        printf ("\nError in loading the image \n");
        exit (1) ;
    }
    printf (" Width = %d\nHeight = %d\nChannel = %d\n", width, height, channel );

// roate the image
    unsigned char * rimage = image_rotation (image, width, height, channel, 180) ;
// save image
    stbi_write_png ( save_path_rotate, width, height, channel, rimage, width*channel );
    printf (" New image saved to %s\n", save_path_rotate );
}
