#include <stdio.h>
#define STB_IMAGE_IMPLEMENTATION
#include "./headers/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "./headers/stb_image_write.h"

unsigned char mask_image (unsigned char *image, int width, int height, int channel){
	for (int i = 0; i < height / 1; i++){
      for (int j = 0; j < width / 1; j++){
      	if(image[i * width * channel + j * channel + 0])
         for (int k = 0; k < channel; k++){
             image[i * width * channel + j * channel + k] = 0;
         }
      }
    }
}
int main (){
	int width, height, channel; 
	char path_img[]= "./images/98239648_p0.png" ;
	char save_path[]= "./images/98239648_p0-New.png";

	// read image data
	unsigned char *image = stbi_load (path_img, &width, &height, &channel, 0);
	if (image == NULL){
	     printf ("\nError in loading the image\n");
	     exit (1);
	}
	printf ("Width= %d\nHeight= %d\nChannel= %d\n", width , height , channel ) ;
	// fill image with black pixels
	mask_image (image, width, height, channel);
	//
	   
	stbi_write_png (save_path, width, height, channel, image, width * channel);
	printf ("New image saved to %s\n", save_path);

}