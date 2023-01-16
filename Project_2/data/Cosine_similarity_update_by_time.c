#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <math.h>
#include "matching_method.h"
#include "basic_image_tools.h"
#define STB_IMAGE_IMPLEMENTATION
#include "./headers/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "./headers/stb_image_write.h"

// Drive: https://drive.google.com/file/d/1OLI9cs1chj8TBd_rtU7NKcxUOmNZZnN3/view?usp=sharing
int main ()
{
    FILE *fp= freopen("log_cosine_similarity.txt", "w+", stdout);
    int test_case= 0;
    //preprocessor part - initialize flexible temp.jpg
    char template[]= "template.jpg";
    char save_img[]= "temp.jpg";
    int width_temp, height_temp, channel_temp;

    unsigned char *image_temp = stbi_load (template, &width_temp, &height_temp, &channel_temp, 0);
    image_temp= turn_grey(image_temp, width_temp, height_temp, channel_temp);
    channel_temp= (channel_temp == 4) ? 2:1;
    stbi_write_png (save_img, width_temp, height_temp, channel_temp, image_temp, width_temp * channel_temp);

    while(test_case <= 62){
        printf("Begin with test_case number %d:\n",test_case);
        int width_main, height_main, channel_main, channel_tmp;
        int width_sub, height_sub, channel_sub;

        char img_main[20];
        sprintf(img_main, "%s%d%s", "./images/img", test_case, ".jpg");
        char img_sub[]= "temp.jpg" ;
        char save_path[60];
        sprintf(save_path, "%s%d%s", "./cosin_similarity_result/tracking/output", test_case, ".jpg");

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

        pixel tmp_1= matching_cosine_similarity(image_tmp, image_sub, width_main, height_main, channel_tmp, width_sub, height_sub, channel_sub);
        pixel tmp_2= matching_SAD(image_tmp, image_temp, width_main, height_main, channel_tmp, width_temp, height_temp, channel_temp);

        printf("sub: %d %d %lf\n", tmp_1.x, tmp_1.y, tmp_1.val);
        printf("temp: %d %d %lf\n", tmp_2.x, tmp_2.y, tmp_2.val);
        if(tmp_2.val >= 1376181){
            pixel result= tmp_1;
            draw(image_main, width_main, height_main, channel_main, width_sub, height_sub, result.x, result.y);
            update(image_tmp, image_sub, width_main, height_main, channel_tmp, width_sub, height_sub, channel_sub, result.x, result.y);
            printf("sub!\n");
        }
        else{   
            pixel result= tmp_2;
            draw(image_main, width_main, height_main, channel_main, width_sub, height_sub, result.x, result.y);
            update(image_tmp, image_sub, width_main, height_main, channel_tmp, width_sub, height_sub, channel_sub, result.x, result.y);
            printf("temp!\n");
        }
        
        

        stbi_write_png (save_path, width_main, height_main, channel_main, image_main, width_main * channel_main);
        stbi_write_png (img_sub, width_sub, height_sub, channel_sub, image_sub, width_sub * channel_sub);

        free(image_main);
        free(image_sub);
        free(image_tmp);
        printf("Done for test_case number %d!\n\n", test_case);
        test_case++;
    }
    fclose(fp);
    return 0;
}
