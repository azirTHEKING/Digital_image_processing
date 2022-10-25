#include <stdio.h>
#define STB_IMAGE_IMPLEMENTATION
#include "./headers/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "./headers/stb_image_write.h"

void eliminate_green (unsigned char *image_fg, unsigned char *image_bg, int width, int height, int channel)
{
    for (int i = 0; i < height / 1; i++)
    {
        for (int j = 0; j < width / 1; j++)
        {
            int flag= 0;
            for (int k = 0; k < channel; k++)
            {
                flag+= abs( image_fg[i * width * channel + j * channel + k] - image_bg[i * width * channel + j * channel + k]);
            }
            
            for(int k= 0; k < channel && flag < 160; k++){
                image_fg[i * width * channel + j * channel + k] = (flag<160) ? (0):(image_fg[i * width * channel + j * channel + k]);
            }
        }
    }

    return ;
}

int main ()
{
    int width_fg, height_fg, channel_fg;
    int width_bg, height_bg, channel_bg;
    char fore_ground[]= "./images/foreground.png" ;
    char back_ground[]= "./images/background.png" ;
    char save_path[]= "./images/foreground-Neu.png";

    // read image data
    unsigned char *image_fg = stbi_load (fore_ground, &width_fg, &height_fg, &channel_fg, 0);
    unsigned char *image_bg = stbi_load (back_ground, &width_bg, &height_bg, &channel_bg, 0);

    if (image_fg == NULL || image_bg == NULL)
    {
        printf ("\nError in loading the image\n");
        exit (1);
    }

    eliminate_green(image_fg, image_bg, width_fg, height_fg, channel_fg);
    //

    stbi_write_png (save_path, width_fg, height_fg, channel_fg, image_fg, width_fg * channel_fg);
    printf ("New image saved to %s\n", save_path);

    return 0;
}
