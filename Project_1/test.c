#include <stdio.h>
#define STB_IMAGE_IMPLEMENTATION
#include "./headers/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "./headers/stb_image_write.h"

void mask_image (unsigned char *image, int width, int height, int channel){
	for (int i = 0; i < height; i++){
      for (int j = 0; j < width; j++){
      	if(image[i * width * channel + j * channel + 0]){
      		for (int k = 0; k < channel; k++){
	         		printf("%d ",  image[i * width * channel + j * channel + k]);
         	}	
         	printf("\n");

      	}
         
      }
    }

}
int main (){
	int width, height, channel; 
	char path_img[]= "./images/foreground-Neu.png" ;
	unsigned char * image = stbi_load ( path_img, &width, &height, & channel, 0);

	mask_image (image, width, height, channel);
	//
	   
	/*stbi_write_png (save_path, width, height, channel, image, width * channel);
	printf ("New image saved to %s\n", save_path);
	*/
}