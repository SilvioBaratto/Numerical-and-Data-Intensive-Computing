/* ============================================================	*/
/* =							      = */
/* =	     		    BMP LIBRARY	  		      = */
/* =							      = */
/* =		Miguel Angel Garcia (October 2002)	      = */
/* =		                    (September 2016)	      = */
/* =							      = */
/* ============================================================	*/

/* Basic set of uncompressed BMP functions */

#include <stdio.h>
#include <string.h>


#include <stdlib.h>
#include <malloc.h> 

#include <math.h>
#include "bmp.h"

/* ============================================================	*/
/* =			    Compute_Padding		      = */
/* ============================================================	*/

int Compute_Padding( int cols )
{
	return( cols % 4 );	
}


/* ============================================================	*/
/* =			    Read_int			      = */
/* ============================================================	*/

unsigned int Read_int( FILE *f )
{
/* Assemble a int word from 4 bytes read from the given
   open file. The bytes are sorted in little endian order */
   
   char buf[ 8 ];
   unsigned int *tmp;
   
   fread( buf, 1, 4, f );
   tmp = (unsigned int*)buf;

   return( *tmp );
}

/* ============================================================	*/
/* =			    Read_Short			      = */
/* ============================================================	*/

unsigned short Read_Short( FILE *f )
{
/* Assemble a short word from 2 bytes read from the given
   open file. The bytes are sorted in little endian order */
   
   char buf[ 8 ];
   unsigned short *tmp;
   
   fread( buf, 1, 2, f );
   tmp = (unsigned short*)buf;

   return( *tmp );
}

/* ============================================================	*/ 
/* =			    BMP_Create_Gray_Image	      = */
/* ============================================================	*/

int BMP_Create_Gray_Image( int rows, int cols, tGrayImage *image )
{
/* Create a gray-level image in memory space given the image 
   descriptor, number of rows and columns. The value of all
   pixels at creation is undefined. The function returns 
   TRUE in case of success and FALSE in case of failure. */
   
	int r,c;

	image->rows = rows;
	image->cols = cols;
	image->image = (tGrayPixel*) malloc( image->rows * image->cols * sizeof(tGrayPixel));


	for (r=0; r < image->rows; r++)
	   for (c=0; c < image->cols; c++)		
		 image->image[ r * image->cols + c ] = 0;



	if (image->image == 0) return( FALSE );
	else return( TRUE );
}

/* ============================================================	*/
/* =			    BMP_Create_Color_Image	      = */
/* ============================================================	*/

int BMP_Create_Color_Image( int rows, int cols, tColorImage *image )
{
/* Create a color image in memory space given the image 
   descriptor, number of rows and columns. At creation, the 
   value of all pixels is undefined. The function returns 
   TRUE in case of success and FALSE in case of failure. */
   
	image->rows = rows;
	image->cols = cols;
	image->image = (tColorPixel*) malloc( image->rows * image->cols * sizeof(tColorPixel));

	if (image->image == 0) return( FALSE );
	else return( TRUE );
}
	
/* ============================================================	*/
/* =			    BMP_Read_Gray_Image		      = */
/* ============================================================	*/

int BMP_Read_Gray_Image( char *filename, tGrayImage *image )
{
/* Read a BMP image contained in a file with the given filename. 
   The image is interpreted in gray level. The image is returned 
   in the second parameter. The function returns TRUE in case of 
   success and FALSE in case of failure. */
   
	FILE *f;
	int padding;
	unsigned char buf[ 100 ];
	int row, col;
	int r, g, b, tmp, tmp1, tmp2;
	
f = fopen(filename, "rb");
	if (f == NULL) return( FALSE ); /* File does not exist */
	
	/* Check signature */
	
	buf[0] = fgetc( f );
	buf[1] = fgetc( f );
	
	if (buf[0] != 'B' || buf[1] != 'M')
		return( FALSE ); /* File is not BMP */

	/* Read image width & height */
	
	fread( buf, 1, 16, f );		/* skip header bytes */
	image->cols = Read_int( f );	/* read image width */
	image->rows = Read_int( f );	/* read image height */
	
	/* Ensure there is no compression */
	
	tmp1 = Read_Short( f );		/* Skip bitplanes */
	tmp1 = Read_Short( f );		/* read Color depth */



//printf("tmp1=%d\n",tmp1); getch();


//	if (tmp1 != 24) return( FALSE ); /* Image is color compressed */


	if ((tmp1 != 24) && (tmp1 != 8)) return( FALSE );

	tmp2 = Read_int( f );		/* read compression type */

//printf("tmp2=%d\n",tmp2); getch();

	if (tmp2 != 0) return( FALSE ); /* Image is compressed */

	image->image = (tGrayPixel*) malloc( image->rows * image->cols * sizeof(tGrayPixel));




if (tmp1==24)
{
	fread( buf, 1, 20, f );	/* skip rest of header */
	
	/* Read image pixels */

	for (row=0; row < image->rows; row++)
	{
	   for (col=0; col < image->cols; col++)
	   {
		buf[ 2 ] = fgetc( f );	/* blue */
		buf[ 1 ] = fgetc( f );	/* green */
		buf[ 0 ] = fgetc( f );  /* red */

		r = buf[ 0 ];
		g = buf[ 1 ];
		b = buf[ 2 ];
	
		tmp = (r + g + b) / 3;
		
		image->image[ row * image->cols + col ] = tmp;
		//printf("(%d,%d)=%d,%d,%d,%d\n",row,col,buf[ 2 ],buf[ 1 ],buf[ 0 ],tmp); getch();
	   }
	   
	   /* skip padding */
	   
	   padding = Compute_Padding( image->cols );
	   if (padding > 0) fread( buf, 1, padding, f ); 
	}

}
else /* tmp1==8 */
{
	//printf("temp1==8\n");

	//fread( buf, 1, 1042, f );	/* skip rest of header */
	for (row=0; row < 1044; row++) fgetc( f );

	
	/* Read image pixels */

	for (row=0; row < image->rows; row++)
	{
	   for (col=0; col < image->cols; col++)
	   {
		tmp = fgetc( f );	/* gray */
		
		image->image[ row * image->cols + col ] = tmp;

		//printf("(%d,%d)=%d\n",row,col,tmp); getch();
	   }
	   
	   /* skip padding */
	   
	   padding = Compute_Padding( image->cols * 3 );
	   if (padding > 0) fread( buf, 1, padding, f ); 
	}

}
		
	fclose( f );
	return( TRUE );
}




/* ============================================================	*/
/* =			  BMP_Write_Gray_Image		      = */
/* ============================================================	*/

int BMP_Write_Gray_Image( char *filename, tGrayImage *image )
{
/* Write the given gray image into a file with the given filename. */
   
	FILE *f;
	unsigned char buf[ 100 ];
	int row, col;
	int inttmp;
	short shortmp;
	int padding;
	
f = fopen(filename, "wb");
	if (f == NULL) return( FALSE ); /* File cannot be opened */
	
	/* Write header */
	buf[0] = 'B';
	buf[1] = 'M';
	fwrite( buf, 1, 2, f );
	
	padding = Compute_Padding( image->cols );
	inttmp = image->cols*image->rows*3 + padding*image->rows + 54;
//printf("%d %d %d \n", padding, inttmp, sizeof(short));
//exit(-1);
	fwrite( &inttmp, sizeof(inttmp), 1, f );
	
	inttmp = 0;
	fwrite( &inttmp, sizeof(inttmp), 1, f );
	inttmp = 54;
	fwrite( &inttmp, sizeof(inttmp), 1, f );
	inttmp = 40;
	fwrite( &inttmp, sizeof(inttmp), 1, f );
	inttmp = image->cols;
	fwrite( &inttmp, sizeof(inttmp), 1, f );
	inttmp = image->rows;
	fwrite( &inttmp, sizeof(inttmp), 1, f );
	shortmp = 1;
	fwrite( &shortmp, sizeof(shortmp), 1, f );
	shortmp = 24;
	fwrite( &shortmp, sizeof(shortmp), 1, f );
	inttmp = 0;
	fwrite( &inttmp, sizeof(inttmp), 1, f );
	inttmp = image->rows*image->cols;
	fwrite( &inttmp, sizeof(inttmp), 1, f );
	inttmp = 2834;					/* Hor. resolution */
	fwrite( &inttmp, sizeof(inttmp), 1, f );
	inttmp = 2834;					/* ver. resolution */
	fwrite( &inttmp, sizeof(inttmp), 1, f );
	inttmp = 0;
	fwrite( &inttmp, sizeof(inttmp), 1, f );
	inttmp = 0;
	fwrite( &inttmp, sizeof(inttmp), 1, f );
		
	/* Write image pixels */
	
	for (row=0; row < image->rows; row++)
	{
	   for (col=0; col < image->cols; col++)
	   {
		buf[0] = buf[1] = buf[2] = image->image[ row * image->cols + col];
		fwrite( buf, 1, 3, f );
	   }
	   
	   /* padding */
	   padding = Compute_Padding( image->cols );
	   if (padding > 0) fwrite( buf, 1, padding, f ); 
	}
		
	fclose( f );
        return( TRUE );
}

/* ============================================================	*/
/* =			BMP_Read_Gray_Pixel		      = */
/* ============================================================	*/

int BMP_Read_Gray_Pixel( tGrayImage *image, int row, int col, tGrayPixel *value )
{
/* Return the value associated with the pixel located at the
   given row and column of the given gray image */
   
	if (row<0 || row >= image->rows || col<0 || col >= image->cols) 
		return( FALSE );
	*value = image->image[ row * image->cols + col ];
	return( TRUE );
}

/* ============================================================	*/
/* =			   BMP_Write_Gray_Pixel		      = */
/* ============================================================	*/

int BMP_Write_Gray_Pixel( tGrayImage *image, int row, int col, tGrayPixel value )
{
/* Set the value associated with the pixel located at the given
   row and column of the given image */
   
	if (row<0 || row >= image->rows || col<0 || col >= image->cols) 
		return( FALSE );
	image->image[ row * image->cols + col ] = value;


//printf("(%d,%d)=%d \n", row,col,value );



	return( TRUE );
}

/* ============================================================	*/
/* =			   BMP_Free_Gray_Image		      = */
/* ============================================================	*/

void BMP_Free_Gray_Image( tGrayImage *image )
{
/* Free dynamic memory utilized by the image */
   
	free( image->image );
}


/* ============================================================	*/
/* =			    BMP_Read_Color_Image	      = */
/* ============================================================	*/

int BMP_Read_Color_Image( char *filename, tColorImage *image )
{
/* Read a BMP image contained in a file with the given filename. 
   The image is interpreted in true color. The image is returned 
   in the second parameter. The function returns TRUE in case of 
   success and FALSE otherwise. */
   
	FILE *f;
	int padding;
	unsigned char buf[ 100 ];
	int row, col;
	int r, g, b, tmp;
	
	f = fopen(filename, "rb");
	if (f == NULL) return( FALSE ); /* File does not exist */
	
	/* Check signature */
	
	fread( buf, 1, 18, f );	/* skip header bytes */	
	
	
	if (buf[0] != 'B' || buf[1] != 'M')
		return( FALSE ); /* File is not BMP */

	/* Read image width & height */
	
	image->cols = Read_int( f );	/* read image width */
	image->rows = Read_int( f );	/* read image height */
printf("%d %d \n", image->cols, image->rows );
	
	/* Ensure there is no compression */
	
	tmp = Read_Short( f );		/* Skip bitplanes */
	tmp = Read_Short( f );		/* read Color depth */
	if (tmp != 24) return( FALSE ); /* Image is color compressed */
	tmp = Read_int( f );		/* read compression type */
	if (tmp != 0) return( FALSE ); /* Image is compressed */

	image->image = (tColorPixel*) malloc( image->rows * image->cols * sizeof(tColorPixel));

	fread( buf, 1, 20, f );	/* skip rest of header */
	
	/* Read image pixels */

	for (row=0; row < image->rows; row++)
	{
	   for (col=0; col < image->cols; col++)
	   {
		b = fgetc( f );
		g = fgetc( f );
		r = fgetc( f );

		image->image[ row * image->cols + col ][0] = r;	
		image->image[ row * image->cols + col ][1] = g;	
		image->image[ row * image->cols + col ][2] = b;	
	   }
	   /* skip padding */
	   
	   padding = Compute_Padding( image->cols );
	   if (padding > 0) fread( buf, 1, padding, f );    
	}
		
	fclose( f );
	return( TRUE );
}

/* ============================================================	*/
/* =			  BMP_Write_Color_Image		      = */
/* ============================================================	*/

int BMP_Write_Color_Image( char *filename, tColorImage *image )
{
/* Write the given gray image into a file with the given filename. */
   
	FILE *f;
	unsigned char buf[ 100 ];
	int row, col;
	int inttmp;
	short shortmp;
	int padding;
f = fopen(filename, "wb");
	
	if (f == NULL) return( FALSE ); /* File cannot be opened */
	
	/* Write header */
	buf[0] = 'B';
	buf[1] = 'M';
	fwrite( buf, 1, 2, f );
	
	   padding = Compute_Padding( image->cols );
	inttmp = image->cols*image->rows*3 + padding*image->rows + 54;
	fwrite( &inttmp, sizeof(inttmp), 1, f );
	
	inttmp = 0;
	fwrite( &inttmp, sizeof(inttmp), 1, f );
	inttmp = 54;
	fwrite( &inttmp, sizeof(inttmp), 1, f );
	inttmp = 40;
	fwrite( &inttmp, sizeof(inttmp), 1, f );
	inttmp = image->cols;
	fwrite( &inttmp, sizeof(inttmp), 1, f );
	inttmp = image->rows;
	fwrite( &inttmp, sizeof(inttmp), 1, f );
	shortmp = 1;
	fwrite( &shortmp, sizeof(shortmp), 1, f );
	shortmp = 24;
	fwrite( &shortmp, sizeof(shortmp), 1, f );
	inttmp = 0;
	fwrite( &inttmp, sizeof(inttmp), 1, f );
	inttmp = image->rows*image->cols;
	fwrite( &inttmp, sizeof(inttmp), 1, f );
	inttmp = 2834;					/* Hor. resolution */
	fwrite( &inttmp, sizeof(inttmp), 1, f );
	inttmp = 2834;					/* ver. resolution */
	fwrite( &inttmp, sizeof(inttmp), 1, f );
	inttmp = 0;
	fwrite( &inttmp, sizeof(inttmp), 1, f );
	inttmp = 0;
	fwrite( &inttmp, sizeof(inttmp), 1, f );
		
	/* Write image pixels */
	
	for (row=0; row < image->rows; row++)
	{
	   for (col=0; col < image->cols; col++)
	   { 
		buf[2] = image->image[ row * image->cols + col][0];
		buf[1] = image->image[ row * image->cols + col][1];
		buf[0] = image->image[ row * image->cols + col][2];
		fwrite( buf, 1, 3, f );
	   }
	   
	   /* padding */
	   padding = Compute_Padding( image->cols );
	   if (padding > 0) fwrite( buf, 1, padding, f ); 
	}
		
	fclose( f );
        return( TRUE );
}

/* ============================================================	*/
/* =			BMP_Read_Color_Pixel		      = */
/* ============================================================	*/

int BMP_Read_Color_Pixel( tColorImage *image, int row, int col, tColorPixel *value )
{
/* Return the value associated with the pixel located at the
   given row and column of the given color image */
   	
	if (row<0 || row >= image->rows || col<0 || col >= image->cols) 
		return( FALSE );
	(*value)[0] = image->image[ row * image->cols + col ][0];
	(*value)[1] = image->image[ row * image->cols + col ][1];
	(*value)[2] = image->image[ row * image->cols + col ][2];
	return( TRUE );
}

/* ============================================================	*/
/* =			   BMP_Write_Color_Pixel	      = */
/* ============================================================	*/

int BMP_Write_Color_Pixel( tColorImage *image, int row, int col, tColorPixel value )
{
/* Set the value associated with the pixel located at the given
   row and column of the given image */
   
	if (row<0 || row >= image->rows || col<0 || col >= image->cols) 
		return( FALSE );
	image->image[ row * image->cols + col ][0] = value[0];
	image->image[ row * image->cols + col ][1] = value[1];
	image->image[ row * image->cols + col ][2] = value[2];
	return( TRUE );
}

/* ============================================================	*/
/* =			   BMP_Free_Color_Image		      = */
/* ============================================================	*/

void BMP_Free_Color_Image( tColorImage *image )
{
/* Free dynamic memory utilized by the image */
   
	free( image->image );
}


