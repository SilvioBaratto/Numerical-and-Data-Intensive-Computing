/* =================================================== */
/* =												 = */
/* =		  MICROALGI DETECTION 1.0 				= */
/* =												 = */
/* =		  Miguel Angel Garcia (August 2011)		 = */
/* =												 = */
/* =============================================== */

/* This program detects the poruses of a membrane imaged by a
   scanning microscope */
   
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "bmp.h"
#include "label.h"

#define THRESHOLD 20

void Graylevel_Histogram( tGrayImage *src, int graylevels, int *histo );

int ThresholdYellow;

typedef struct
{
	int area, areagroc;
	float sumgroc, sumgroc2;
	int mingroc, maxgroc;
	int rowmin, rowmax, colmin, colmax;
} tAlga;


/* ==================================================== */
/* =				Add_Color_Images			      = */
/* ==================================================== */

void Add_Color_Images( tColorImage *src1, tColorImage *src2, tColorImage *dst )
{
	int r, c, i;
	tColorPixel pixel1, pixel2, pixel;
	int tmp;

	for (r=0; r<src1->rows; r++)
	for (c=0; c<src1->cols; c++)
	{
		BMP_Read_Color_Pixel( src1, r, c, &pixel1 );
		BMP_Read_Color_Pixel( src2, r, c, &pixel2 );

		for (i=0; i<3; i++)
		{
			tmp = (pixel1[i] + pixel2[i]);
			pixel[i] = tmp;
		}

		BMP_Write_Color_Pixel( dst, r, c, pixel );
	}
}



/* ==================================================== */
/* =				Binarize_Gray_Image			      = */
/* ==================================================== */

void Binarize_Gray_Image( tGrayImage *src, tGrayPixel threshold, tGrayImage *dst )
{
	int r, c;
	tGrayPixel pixel;

	for (r=0; r<src->rows; r++)
	for (c=0; c<src->cols; c++)
	{
		BMP_Read_Gray_Pixel( src, r, c, &pixel );
		if (pixel > threshold) pixel = 255;
		else pixel = 0;
		BMP_Write_Gray_Pixel( dst, r, c, pixel );
	}
}


/* ==================================================== */
/* =				Or_Gray_Images				      = */
/* ==================================================== */

void Or_Gray_Images( tGrayImage *src1, tGrayImage *src2, tGrayImage *dst )
{
	int r, c;
	tGrayPixel pixel1, pixel2;

	for (r=0; r<src1->rows; r++)
	for (c=0; c<src1->cols; c++)
	{
		BMP_Read_Gray_Pixel( src1, r, c, &pixel1 );
		BMP_Read_Gray_Pixel( src2, r, c, &pixel2 );

		pixel1 = pixel1 | pixel2;
		BMP_Write_Gray_Pixel( dst, r, c, pixel1 );
	}
}



/* ==================================================== */
/* =			Otsu_Binarize_Gray_Image		      = */
/* ==================================================== */

int Otsu_Binarize_Gray_Image( tGrayImage *src, tGrayImage *dst )
{
	int histo[256];
	float nhisto[256];
	int i, j, r, c;
	int posmaxdif;
	float maxdif, acum, avg1, avg2, prob1, prob2, dif;
	tGrayPixel pixel;

	Graylevel_Histogram( dst, 256, histo );
	acum = 0;
	for (i=0; i<256; i++) acum += histo[i];
	for (i=0; i<256; i++) nhisto[i] = (float) histo[i] / acum;


	maxdif = 0;
	for (i=1; i<255; i++)
	{
		prob1 = avg1 = 0;
		for (j=0; j<i; j++)
		{
			avg1 += nhisto[j] * (j+1);
			prob1 += nhisto[j];

		}


		prob2 = avg2 = 0;
		for (j=i; j<256; j++)
		{
			avg2 += nhisto[j] * (j+1);
			prob2 += nhisto[j];
		}

		dif = avg2 - avg1;
		dif = prob1 * prob2 * dif * dif;

		if (dif > maxdif)
		{
			maxdif = dif;
			posmaxdif = i;
		}
	}
printf("posmaxdif %d\n", posmaxdif );

	for (r=0; r<src->rows; r++)
	for (c=0; c<src->cols; c++)
	{
		BMP_Read_Gray_Pixel( src, r, c, &pixel );
		if (pixel > posmaxdif) pixel = 255;
		else pixel = 0;
		BMP_Write_Gray_Pixel( dst, r, c, pixel );
	}

	return posmaxdif;
}



/* ==================================================== */
/* =				Color_Image_to_Gray			      = */
/* ==================================================== */

void Color_Image_to_Gray( tColorImage *src, tGrayImage *dst )
{
/* Convert the src color image to the dst gray image */
   
	int i;
	int r, c;
	unsigned char max;
	tColorPixel pixel;
	tGrayPixel gpixel;
	
	for (r=0; r<src->rows; r++)
	for (c=0; c<src->cols; c++)
	{
		BMP_Read_Color_Pixel( src, r, c, &pixel );

		max = 0;

		for (i=0; i<3; i++)
			if (pixel[i] > max) max = pixel[i];

		gpixel = max;
		BMP_Write_Gray_Pixel( dst, r, c, gpixel );
	}
}


/* ==================================================================== */
/* =				Graylevel_Histogram		      = */
/* ==================================================================== */

void Graylevel_Histogram( tGrayImage *src, int graylevels, int *histo )
{
/* Compute the histogram of a gray image of the given number of
   gray levels */
   
	int i;
	int r, c;
	tGrayPixel pixel;
	
	for (i=0; i<graylevels; i++) histo[i] = 0;
	for (r=0; r<src->rows; r++)
	for (c=0; c<src->cols; c++)
	{
		BMP_Read_Gray_Pixel( src, r, c, &pixel );
		histo[ pixel ]++;
	}
}

/* ==================================================================== */
/* =			Conditional_Graylevel_Histogram		      = */
/* ==================================================================== */

void Conditional_Graylevel_Histogram( tGrayImage *src, tGrayImage *cnd, int graylevels, int *histo )
{
/* Compute the histogram of a gray image of the given number of
   gray levels only considering the pixels that are equal to 255
   in the cns image */
   
	int i;
	int r, c;
	tGrayPixel pixel, flag;
	
	for (i=0; i<graylevels; i++) histo[i] = 0;
	for (r=0; r<src->rows; r++)
	for (c=0; c<src->cols; c++)
	{
		BMP_Read_Gray_Pixel( src, r, c, &pixel );
		BMP_Read_Gray_Pixel( cnd, r, c, &flag );
		if (flag == 255) histo[ pixel ]++;
	}
}


/* ==================================================================== */
/* =				Sobel_Filter			      = */
/* ==================================================================== */

void Sobel_Filter( tGrayImage *src, tGrayImage *dst )
{
	int r, c;
	double tmp1, tmp2;
	tGrayPixel vpix[9], pixel;
	double **sobel;
	double min, max, tmp;
	
	sobel = (double**) malloc( src->rows * sizeof(double*) );
	for (r=0; r<src->rows; r++) 
		sobel[r] = (double*) malloc( src->cols * sizeof(double));
	
	min = HUGE_VAL;
	max = 0;
	
	for (r=1; r<src->rows-1; r++)
	for (c=1; c<src->cols-1; c++)
	{
		BMP_Read_Gray_Pixel( src, r-1, c-1, &vpix[0] );
		BMP_Read_Gray_Pixel( src, r-1, c, &vpix[1] );
		BMP_Read_Gray_Pixel( src, r-1, c+1, &vpix[2] );
		BMP_Read_Gray_Pixel( src, r, c-1, &vpix[3] );
		BMP_Read_Gray_Pixel( src, r, c, &vpix[4] );
		BMP_Read_Gray_Pixel( src, r, c+1, &vpix[5] );
		BMP_Read_Gray_Pixel( src, r+1, c-1, &vpix[6] );
		BMP_Read_Gray_Pixel( src, r+1, c, &vpix[7] );
		BMP_Read_Gray_Pixel( src, r+1, c+1, &vpix[8] );
		
		tmp1 = vpix[0] * -1 + 
			     vpix[1] * -2 +
			     vpix[2] * -1 +
			     vpix[6] +
			     vpix[7] * 2 +
			     vpix[8];
			     
		tmp2 = vpix[0] * -1 + 
			     vpix[2] +
			     vpix[3] * -2 +
			     vpix[5] * 2 +
			     vpix[6] * -1 +
			     vpix[8];
			     
		sobel[r][c] = sqrt( tmp1*tmp1 + tmp2*tmp2 ); 	
		
		if (sobel[r][c] > max) max = sobel[r][c];
		if (sobel[r][c] < min) min = sobel[r][c];
	}
	
	for (r=1; r<src->rows-1; r++)
	for (c=1; c<src->cols-1; c++)
	{
		tmp = (sobel[r][c] - min) / (max-min) * 255;
		pixel = (tGrayPixel) tmp;
		
		BMP_Write_Gray_Pixel( dst, r, c, pixel );
	}
	
	for (r=0; r<src->rows; r++) free( sobel[r] );
	free( sobel );
}

/* ==================================================================== */
/* =				Binary_Dilation			      = */
/* ==================================================================== */

void Binary_Dilation( tGrayImage *src, tGrayImage *dst )
{
	int r, c, rr, cc;
	int r1, r2, c1, c2;
	char flag;
	tGrayPixel pixel;
	
	for (r=0; r<src->rows; r++)
	for (c=0; c<src->cols; c++)
	{
	   BMP_Read_Gray_Pixel( src, r, c, &pixel );
	   BMP_Write_Gray_Pixel( dst, r, c, pixel );
	}
	
	for (r=0; r<src->rows; r++)
	for (c=0; c<src->cols; c++)
	{
	   BMP_Read_Gray_Pixel( src, r, c, &pixel );
	   if (pixel == 0)
	   {
		
		flag = 0;
		r1 = r-1; if (r1<0) r1=0;
		r2 = r+1; if (r2>=src->rows) r2=src->rows-1;
		c1 = c-1; if (c1<0) c1=0;
		c2 = c+1; if (c2>=src->cols) c2=src->cols-1;
		for (rr=r1; rr<=r2; rr++)
		for (cc=c1; cc<=c2; cc++)
		if (rr != r || cc != c)
		{
			BMP_Read_Gray_Pixel( src, rr, cc, &pixel );
			if (pixel == 255)
			{
				flag = 1;
				break;
			}
		}
		
		if (flag) BMP_Write_Gray_Pixel( dst, r, c, 255 );
			
	   }
	}
}


/* ==================================================================== */
/* =				Binary_Erosion			      = */
/* ==================================================================== */

void Binary_Erosion( tGrayImage *src, tGrayImage *dst )
{
	int r, c, rr, cc;
	int r1, r2, c1, c2;
	char flag;
	tGrayPixel pixel;
	
	for (r=0; r<src->rows; r++)
	for (c=0; c<src->cols; c++)
	{
	   BMP_Read_Gray_Pixel( src, r, c, &pixel );
	   BMP_Write_Gray_Pixel( dst, r, c, pixel );
	}
	
	for (r=0; r<src->rows; r++)
	for (c=0; c<src->cols; c++)
	{
	   BMP_Read_Gray_Pixel( src, r, c, &pixel );
	   if (pixel == 255)
	   {
		
		flag = 0;
		r1 = r-1; if (r1<0) r1=0;
		r2 = r+1; if (r2>=src->rows) r2=src->rows-1;
		c1 = c-1; if (c1<0) c1=0;
		c2 = c+1; if (c2>=src->cols) c2=src->cols-1;
		for (rr=r1; rr<=r2; rr++)
		for (cc=c1; cc<=c2; cc++)
		if (rr != r || cc != c)
		{
			BMP_Read_Gray_Pixel( src, rr, cc, &pixel );
			if (pixel == 0)
			{
				flag = 1;
				break;
			}
		}
		
		if (flag) BMP_Write_Gray_Pixel( dst, r, c, 0 );
			
	   }
	}
}

/* ==================================================================== */
/* =			Structuring_Element_Match		      = */
/* ==================================================================== */

int Structuring_Element_Match( tGrayPixel window[3][3], int se[][3] )
{
/* Determine whether the given window of gray image pixels
   matches the given structuring element */
   
	int r, c;
		
	for (r=0; r<3; r++)
	for (c=0; c<3; c++)
	if (se[r][c] != -1)
	   if (se[r][c] && !window[r][c] ||
	       !se[r][c] && window[r][c]) return( 0 );
	return( 1 );
}




/* ==================================================================== */
/* =				Binary_Closing			      = */
/* ==================================================================== */

void Binary_Closing( tGrayImage *src, tGrayImage *dst )
{
	tGrayImage tmp;
	
	BMP_Create_Gray_Image( src->rows, src->cols, &tmp );
	
	Binary_Dilation( src, &tmp );
	Binary_Erosion( &tmp, dst );
	
	BMP_Free_Gray_Image( &tmp );
}

/* ==================================================================== */
/* =				Binary_Opening			      = */
/* ==================================================================== */

void Binary_Opening( tGrayImage *src, tGrayImage *dst )
{
	tGrayImage tmp;
	
	BMP_Create_Gray_Image( src->rows, src->cols, &tmp );
	
	Binary_Erosion( src, &tmp );
	Binary_Dilation( &tmp, dst );
	
	BMP_Free_Gray_Image( &tmp );
}


/* ============================================================	*/
/* =			     main			      = */
/* ============================================================	*/

main( int argc, char **argv )
{	
	FILE *f;
	tLabelArray Labels;
	tLabel label;
	int i, r, c, fin;
	int n;
	tColorImage red, yellow, redyellow;
	tGrayImage red1, red2, yellow1, yellow2, redyellow1, algues;
	tAlga *alga;
	tGrayPixel pixel;
	tColorPixel colpixel;
	float ratio, mitja, sigma;

	if (argc < 3) 
	{
		printf("Usage: algi channel1.bmp channel2.bmp [threshold]\n");
		exit(-1);
	}
		
	if (!BMP_Read_Color_Image( argv[1], &yellow ))
	{
		printf("ERROR: File cannot be opened %s\n", argv[1]);
		exit( -1 );
	};
	
	if (!BMP_Read_Color_Image( argv[2], &red ))
	{
		printf("ERROR: File cannot be opened %s\n", argv[2]);
		exit( -1 );
	};

	if (argc == 4) ThresholdYellow = atoi(argv[3]);
	else ThresholdYellow = THRESHOLD;

	BMP_Create_Color_Image( yellow.rows, yellow.cols, &redyellow );
	Add_Color_Images( &red, &yellow, &redyellow );

	BMP_Create_Gray_Image( yellow.rows, yellow.cols, &yellow1 );
	BMP_Create_Gray_Image( yellow.rows, yellow.cols, &yellow2 );
	BMP_Create_Gray_Image( yellow.rows, yellow.cols, &red1 );
	BMP_Create_Gray_Image( yellow.rows, yellow.cols, &red2 );
	BMP_Create_Gray_Image( yellow.rows, yellow.cols, &redyellow1 );
	BMP_Create_Gray_Image( yellow.rows, yellow.cols, &algues );

	Create_Label_Array( yellow.rows, yellow.cols, &Labels );

	Color_Image_to_Gray( &yellow, &yellow1 );
	Color_Image_to_Gray( &red, &red1 );
	Binarize_Gray_Image( &yellow1, THRESHOLD, &yellow2 );
	Binarize_Gray_Image( &red1, THRESHOLD, &red2 );
	Or_Gray_Images( &yellow2, &red2, &redyellow1 );

	Binarize_Gray_Image( &yellow1, ThresholdYellow, &yellow2 );

	Binary_Closing( &redyellow1, &algues );

	BMP_Write_Gray_Image("fat.bmp", &yellow2 );
	BMP_Write_Gray_Image("algi.bmp", &algues );

	n = Label_Binary_Image( &algues, &Labels );
	n--;
	printf("%d microalgi\n", n);

	alga = malloc( n * sizeof(tAlga) );

	for (i=0; i<n; i++)
	{	
		alga[i].area = 0;
		alga[i].areagroc = 0;
		alga[i].mingroc = 100000;
		alga[i].maxgroc = 0;
		alga[i].colmax = 0;
		alga[i].colmin = 100000;
		alga[i].rowmax = 0;
		alga[i].rowmin = 100000;
		alga[i].sumgroc = 0;
		alga[i].sumgroc2 = 0;
	}

	for (r=0; r<Labels.rows; r++)
	for (c=0; c<Labels.cols; c++)
	{
		label = Read_Label( &Labels, r, c );
		if (label > 0)
		{
			label--;
			alga[label].area++;
			if (r < alga[label].rowmin) alga[label].rowmin = r;
			if (r > alga[label].rowmax) alga[label].rowmax = r;
			if (c < alga[label].colmin) alga[label].colmin = c;
			if (c > alga[label].colmax) alga[label].colmax = c;
			BMP_Read_Gray_Pixel( &yellow2, r, c, &pixel );
			if (pixel == 255)
			{
				alga[label].areagroc++;
				BMP_Read_Gray_Pixel( &yellow1, r, c, &pixel);
				if (pixel > alga[label].maxgroc) alga[label].maxgroc = pixel;
				if (pixel < alga[label].mingroc) alga[label].mingroc = pixel;
				alga[label].sumgroc += (float)pixel;
				alga[label].sumgroc2 += (float)pixel*(float)pixel;
			}
		}
	}

	for (i=0; i<n; i++)
	{
		if (alga[i].areagroc > 1)
		{
			ratio = (float) alga[i].areagroc / (float) alga[i].area * 100;
			mitja = alga[i].sumgroc / (float) alga[i].areagroc;
			sigma = sqrt( (alga[i].sumgroc2 - alga[i].sumgroc*alga[i].sumgroc/(float)alga[i].areagroc)/((float)alga[i].areagroc-1) );
			printf("#%d (%d %d, %d %d): area %d fat %d (%d%%) mean %.2f sigma %.2f \n", i+1, alga[i].rowmin, alga[i].colmin, alga[i].rowmax, alga[i].colmax, alga[i].area, alga[i].areagroc, (int) ratio, mitja, sigma );
			colpixel[0] = colpixel[1] = 255;
			colpixel[2] = 0;
		}
		else
		{
			colpixel[0] = colpixel[1] = colpixel[2] = 128;
			printf("#%d (%d %d, %d %d): area %d \n", i+1, alga[i].rowmin, alga[i].colmin, alga[i].rowmax, alga[i].colmax, alga[i].area );
		}
			for (r=alga[i].rowmin; r<=alga[i].rowmax; r++)
			{
				BMP_Write_Color_Pixel( &redyellow, r, alga[i].colmin, colpixel );
				BMP_Write_Color_Pixel( &redyellow, r, alga[i].colmax, colpixel );
			}
			for (c=alga[i].colmin; c<=alga[i].colmax; c++)
			{
				BMP_Write_Color_Pixel( &redyellow, alga[i].rowmin, c, colpixel );
				BMP_Write_Color_Pixel( &redyellow, alga[i].rowmax, c, colpixel );
			}
	}

BMP_Write_Color_Image("joint.bmp", &redyellow );

	BMP_Free_Color_Image( &redyellow );
	BMP_Free_Color_Image( &red );
	BMP_Free_Color_Image( &yellow );
	BMP_Free_Gray_Image( &red1 );
	BMP_Free_Gray_Image( &red2 );
	BMP_Free_Gray_Image( &yellow1 );
	BMP_Free_Gray_Image( &yellow2 );
	BMP_Free_Gray_Image( &redyellow1 );
	BMP_Free_Gray_Image( &algues );
	Destroy_Label_Array(  &Labels );
	free( alga );

}
