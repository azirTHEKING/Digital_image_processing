#include <stdio.h>
#include <string.h>
#define STB_IMAGE_IMPLEMENTATION
#include "./headers/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "./headers/stb_image_write.h"

void subtraction (unsigned char *image_fg, unsigned char *image_bg, unsigned char *image_ag, int width, int height, int channel)
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int flag= 0;
            for (int k = 0; k < channel; k++)
            {
                flag+= abs( image_fg[i * width * channel + j * channel + k] - image_bg[i * width * channel + j * channel + k]);
            }

            if(flag < 160){
                for(int k= 0; k < 3 ; k++){
                    unsigned char tmp= image_ag[i * width * channel + j * channel + k];
                    image_fg[i * width * channel + j * channel + k] = tmp;
                }
            }
        }
    }

    return;
}

int main ()
{
    int width_fg, height_fg, channel_fg;
    int width_bg, height_bg, channel_bg;
    int width_ag, height_ag, channel_ag;
    char fore_ground[]= "./images/foreground.png" ;
    char back_ground[]= "./images/background.png" ;
    char add_ground[]= "./images/weather_forecast-resized_1.png";
    char save_path[]= "./images/result_image.png";

    // read image data
    unsigned char *image_fg = stbi_load (fore_ground, &width_fg, &height_fg, &channel_fg, 0);
    unsigned char *image_bg = stbi_load (back_ground, &width_bg, &height_bg, &channel_bg, 0);
    unsigned char *image_ag = stbi_load (add_ground, &width_ag, &height_ag, &channel_ag, 0);

    printf("info of foreground: %d %d %d\n", width_fg, height_fg, channel_fg);
    printf("info of background: %d %d %d\n", width_bg, height_bg, channel_bg);
    printf("info of weather_forecast: %d %d %d\n", width_ag, height_ag, channel_ag);
    if (image_fg == NULL || image_bg == NULL || image_ag == NULL)
    {
        printf ("\nError in loading the image\n");
        exit (1);
    }

    // info_fg == info_bg == info_ag
    subtraction(image_fg, image_bg, image_ag, width_fg, height_fg, channel_fg);

    stbi_write_png (save_path, width_fg, height_fg, channel_fg, image_fg, width_fg * channel_fg);
    printf ("New image saved to %s\n", save_path);

    char command[]= "start ";
    strcat(command, save_path);
    system(command);
    return 0;
}
