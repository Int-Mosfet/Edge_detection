#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <math.h>
//#include "mypgm.h"

#define H 500  //raw image size: 249H
#define W 500  //raw image size: 500W
#define GRADIENT 1
#define THREETHREE 0
#define FIVEFIVE 0

void sobel_filtering();


double image1[H][W], image2[H][W];
unsigned char output[H][W];
int weightx[3][3] = { { -1, 0, 1 },
{ -2, 0, 2 },
{ -1, 0, 1 } };
int weighty[3][3] = { { -1, -2, -1 },
{ 0, 0, 0 },
{ 1, 2, 1 } };
int weightxx[5][5] = { { 1, 2, 0, -2, -1 },
{ 4, 8, 0, -8, -4 },
{ 6, 12, 0, -12, -6 },
{ 4, 8, 0, -8, -4 },
{ 1, 2, 0, -2, -1 } };
int weightyy[5][5] = { { -1, -4, -6, -4, -1 },
{ -2, -8, -12, -8, -2 },
{ 0, 0, 0, 0, 0 },
{ 2, 8, 12, 8, 2 },
{ 1, 4, 6, 4, 1 } };
double pixel_valuex;
double pixel_valuey;
double pixel_value33;
double pixel_value55;

int threshold = 125; //for 3X3
int threshold1 = 115;


//double min, max;
int x, y, i, j;  //* Loop variable
int y_size1 = 500, x_size1 = 500;

unsigned char buffer[H][W];

void sobel_filtering()
//* Spatial filtering of image data 
//* Sobel filter (horizontal differentiation 
//* Input: image1[y][x] ---- Outout: image2[y][x] 
{
	//* Definition of Sobel filter in horizontal direction 
	//, y_size2 = 500, x_size2 = 500;
	//double image1[H][W], image2[H][W];

	//* Maximum values calculation after filtering
	printf("Now, filtering of input image is performed\n\n");
	//min = DBL_MAX;
	//max = -DBL_MAX;
	for (y = 1; y < y_size1 - 1; y++) {
		for (x = 1; x < x_size1 - 1; x++) {
			pixel_valuex = 0.0;
			pixel_valuey = 0.0;
			pixel_value33 = 0.0;
			pixel_value55 = 0.0;
			for (j = -1; j <= 1; j++) {
				for (i = -1; i <= 1; i++) {
#if GRADIENT
					pixel_valuex += weightx[j + 1][i + 1] * buffer[y + j][x + i];
					pixel_valuey += weighty[j + 1][i + 1] * buffer[y + j][x + i];
#elif THREETHREE
					pixel_value33 += weightx[j + 1][i + 1] * buffer[y + j][x + i];
					pixel_value33 += weighty[j + 1][i + 1] * buffer[y + j][x + i];
#elif FIVEFIVE
					pixel_value55 += weightxx[j + 1][i + 1] * buffer[y + j][x + i];
					pixel_value55 += weightyy[j + 1][i + 1] * buffer[y + j][x + i];
#endif
					


				}
			}

#if GRADIENT
			output[y][x] = (unsigned char)(sqrt((pixel_valuex*pixel_valuex) + (pixel_valuey*pixel_valuey)) / 256); // +128); //divide by 128 for G55, divide by 8 for G33
																//take out adding 128 for gradient	
			
			
#elif THREETHREE
			output[y][x] = (unsigned char)(pixel_value33 / 8 + 128);
			if (output[y][x] < threshold && \
				output[y][x] > output[y - 1][x - 1] && \
				output[y][x] >= output[y + 1][x + 1] )
				output[y][x] = 255;
			else
				output[y][x] = 0;
#elif FIVEFIVE
			output[y][x] = (unsigned char)(pixel_value55 / 8 + 128); //divide by 128 for Dx55 or Dy55, some kind of overflow happening.  E55 is ok dividing by 8
			if (output[y][x] < threshold1 && \
				output[y][x] > output[y - 1][x - 1] && \
				output[y][x] >= output[y + 1][x + 1] )
				output[y][x] = 255;
			else
				output[y][x] = 0;
#endif
			
		}
	}
	
}




int main()
{


	FILE *fp, *fd;  //file
	fp = fopen("C:/Users/user_name/Documents/Visual Studio 2013/Projects/Project1/Project1/bird1.raw", "rb");  //open file
	if (!fp)
	{
		printf("ERROR!\n");
	}

	fread(buffer, H*W, 1, fp);  //pData is the image
	fclose(fp); //close fp point




	sobel_filtering();   //* Sobel filter is applied to image1

	printf("Filtering done");



	fd = fopen("C:/Users/user_name/Documents/Visual Studio 2013/Projects/Project1/Project1/E33.raw", "wb");

	fwrite(output, H*W, 1, fd);
	fclose(fd); //close
	return 0;
}
