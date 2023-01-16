#include <stdio.h>
#include <math.h>
#include <limits.h>
#include <stdlib.h>

typedef struct pixel{
    int x;
    int y;
    double val;
}pixel;
pixel matching_cosine_similarity(unsigned char *image_main, unsigned char* image_sub, int width_main, int height_main, int channel_main, int width_sub, int height_sub, int channel_sub){
    pixel res;
    res.x= 0; res.y= 0; res.val= 0;  

    // loop through the search image
    for ( size_t x = 0; x <= height_main - height_sub; x++ ) {
        for ( size_t y = 0; y <= width_main - width_sub; y++ ) {
            double numerator= 0, denominator_main= 0, denominator_sub= 0;
            double tmp= 0;

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
            if ( res.val < tmp ) {
                res.val= tmp;
                // give me min SAD
                res.x = x;
                res.y = y;
            }
        }

    }

    return res;
}
pixel matching_SAD(unsigned char *image_main, unsigned char* image_sub, int width_main, int height_main, int channel_main, int width_sub, int height_sub, int channel_sub){
    pixel res;  
    res.x = 0; res.y= 0, res.val= INT_MAX;

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
            if ( res.val > SAD ) {
                res.val = SAD;
                // give me min SAD
                res.x = x;
                res.y = y;
            }
        }

    }

    return res;
}

pixel matching_upgrade(unsigned char *image_main, unsigned char* image_sub, int width_main, int height_main, int channel_main, int width_sub, int height_sub, int channel_sub){

    long long int minSAD = 0;
    pixel tmp;
    int x_res= 0, y_res= 0;

    int **matrix_main_col= (int**)malloc(sizeof(int*)*height_main);
    for(int i= 0; i < height_main; i++){
        matrix_main_col[i]= (int*)calloc(width_main, sizeof(int));
    }
    int **matrix_main_row= (int**)malloc(sizeof(int*)*height_main);
    for(int i= 0; i < height_main; i++){
        matrix_main_row[i]= (int*)calloc(width_main, sizeof(int));
    }

    int **matrix_sub_col= (int**)malloc(sizeof(int*)*height_sub);
    for(int i= 0; i < height_sub; i++){
        matrix_sub_col[i]= (int*)calloc(width_sub, sizeof(int));
    }
    int **matrix_sub_row= (int**)malloc(sizeof(int*)*height_sub);
    for(int i= 0; i < height_sub; i++){
        matrix_sub_row[i]= (int*)calloc(width_sub, sizeof(int));
    }

    for ( size_t i = 0; i < height_main; i++ ) {
        for ( size_t j = 0; j < width_main; j++ ){
            matrix_main_row[i][j]= matrix_main_col[i][j]= (int)image_main[i*width_main + j];
            if(j > 0){
                matrix_main_row[i][j]+= matrix_main_row[i][j-1];
            }
            if(i > 0){
                matrix_main_col[i][j]+= matrix_main_col[i-1][j];
            }
        }
    }

    for ( size_t i = 0; i < height_sub; i++ ) {
        for ( size_t j = 0; j < width_sub; j++ ){
            matrix_sub_row[i][j]= (int)image_sub[i*width_sub + j];
            matrix_sub_row[i][j]= (int)image_sub[i*width_sub + j];
            if(j > 0){
                matrix_sub_row[i][j]+= matrix_sub_row[i][j-1];
            }
            if(i > 0){
                matrix_sub_col[i][j]+= matrix_sub_col[i-1][j];
            }
        }
    }

    // loop through the search image
    for ( size_t x = 0; x <= height_main - height_sub; x++ ) {
        for ( size_t y = 0; y <= width_main - width_sub; y++ ) {
            double numerator= 0, denominator_main= 0, denominator_sub= 0;

            for ( size_t i = 0; i < height_sub; i++ ) {
                int A= matrix_main_col[x+i][y+width_sub-1]-matrix_main_col[x+i][y];
                int B= matrix_sub_col[i][width_sub-1];
                
                numerator+= A*B;
                denominator_main+= A*A;
                denominator_sub+= B*B;
            }
            for ( size_t j = 0; j < width_sub; j++ ) {
                int A= matrix_main_row[x+height_sub-1][y+j]-matrix_main_row[x][y+j];
                int B= matrix_sub_row[height_sub-1][j];
                
                numerator+= A*B;
                denominator_main+= A*A;
                denominator_sub+= B*B;   
            }

            double tmp= numerator/(sqrt(denominator_main)*sqrt(denominator_sub));

            if ( minSAD < tmp ) {
                minSAD= tmp.val = tmp;
                
                x_res = x;
                y_res = y;
            }
        }

    }
    //exit(0);
    tmp.x= x_res;
    tmp.y= y_res;
    return tmp;
}
