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

void update(unsigned char *image_main, unsigned char* image_sub, int width_main, int height_main, int channel_main, int width_sub, int height_sub, int channel_sub, int x, int y){
    for(int i= 0; i < height_sub; i++){
        for(int j= 0; j < width_sub; j++){
            image_sub[i*width_sub + j]= image_main[(i+x)*width_main + (j+y)];
        }
    }

    return;
}
int main ()
{
    //FILE *fp= freopen("log_cosine_similarity.txt", "w+", stdout);
    int test_case= 0;
    //preprocessor part - initialize flexible temp.jpg
    char template[]= "template.jpg";
    char save_img[]= "temp.jpg";
    int width_temp, height_temp, channel_temp;

    unsigned char *image_temp = stbi_load (template, &width_temp, &height_temp, &channel_temp, 0);
    image_temp= turn_grey(image_temp, width_temp, height_temp, channel_temp);
    channel_temp= (channel_temp == 4) ? 2:1;
    stbi_write_png (save_img, width_temp, height_temp, channel_temp, image_temp, width_temp * channel_temp);

    while(test_case <= 0){
        printf("Begin with test_case number %d:\n",test_case);
        int width_main, height_main, channel_main, channel_tmp;
        int width_sub, height_sub, channel_sub;

        char img_main[20];
        sprintf(img_main, "%s%d%s", "./images/img", test_case, ".jpg");
        char img_sub[]= "temp.jpg" ;
        char save_path[20];
        sprintf(save_path, "%s%d%s", "output", test_case, ".jpg");

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

        pixel result= matching_upgrade(image_tmp, image_sub, width_main, height_main, channel_tmp, width_sub, height_sub, channel_sub);
        printf("position: %d %d \n", result.x, result.y);
        printf("value: %lf\n", result.val);
        /*pixel tmp_2= matching_cosine_similarity(image_tmp, image_temp, width_main, height_main, channel_tmp, width_temp, height_temp, channel_temp);

        printf("sub: %d %d %lf\n", tmp_1.x, tmp_1.y, tmp_1.val);
        printf("temp: %d %d %lf\n", tmp_2.x, tmp_2.y, tmp_2.val);
        if(tmp_1.val > tmp_2.val){
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
        }*/
        draw(image_main, width_main, height_main, channel_main, width_sub, height_sub, result.x, result.y);
       	update(image_tmp, image_sub, width_main, height_main, channel_tmp, width_sub, height_sub, channel_sub, result.x, result.y);
        

        stbi_write_png (save_path, width_main, height_main, channel_main, image_main, width_main * channel_main);
        stbi_write_png (img_sub, width_sub, height_sub, channel_sub, image_sub, width_sub * channel_sub);

        free(image_main);
        free(image_sub);
        free(image_tmp);
        printf("Done for test_case number %d!\n\n", test_case);
        test_case++;
    }
    //fclose(fp);
    return 0;
}
