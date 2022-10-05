/* =============================================================================== */
/* =								                                             = */ 
/* =																			 = */
/* =		  Miguel Angel Garcia, Domenec Puig, Carles Torras, Ricard Garcia	 = */
/* =																			 = */
/* =============================================================================== */

#include <string.h>  
#include <stdlib.h>
#include <malloc.h> 

#include <stdio.h>
#include <math.h>


#include "bmp.h"
#include "label.h"

#define FOUR_CONNECTIVITY	1
#define EIGHT_CONNECTIVITY	2
#define CONNECTIVITY	FOUR_CONNECTIVITY

/* ============================================================	*/
/* =			  Create_Label_Array		      = */
/* ============================================================	*/

void Create_Label_Array( int rows, int cols, tLabelArray *array )
{
/* Create an array of rows x cols labels */

	array->label = (tLabel*) malloc( rows * cols * sizeof(tLabel) );
	array->rows = rows;
	array->cols = cols;
}

/* ============================================================	*/
/* =			  Destroy_Label_Array		      = */
/* ============================================================	*/

int Destroy_Label_Array(  tLabelArray *array )
{
/* Destroy an array of labels. The array has to been previously
   created. */

	free( array->label );

	return( TRUE );
}

/* ============================================================	*/
/* =			  Read_Label			      = */
/* ============================================================	*/

tLabel Read_Label(  tLabelArray *array, int row, int col )
{
/* Return the label at the given row and column of the input array */

	return( array->label[ row * array->cols + col ] );
}

/* ============================================================	*/
/* =			  Write_Label			      = */
/* ============================================================	*/

void Write_Label(  tLabelArray *array, int row, int col, tLabel label )
{
/* Write the label at the given row and column of the input array */

	array->label[ row * array->cols + col ] = label;
}

/* ============================================================	*/
/* =			    Interior_Label		      = */
/* ============================================================	*/

int Interior_Label( tLabelArray *image, int row, int col )
{
/* Return TRUE whenever the given pixel coordinates (row,col)
   are inside the area of the given gray image. Return FALSE 
   otherwise. */
   	
	if (row<0 || row>=image->rows) return( FALSE );
	if (col<0 || col>=image->cols) return( FALSE );
	return( TRUE );

}

/* ============================================================	*/
/* =			    Minimum_Label_Neighbor	      = */
/* ===========================================================	*/

void Minimum_Label_Neighbor( tLabelArray *image, int row, int col, 
				tLabel *minlabel )
{
/* If pixel (row,col) in the given image is a valid pixel with a
   value smaller than *minlabel, *minlabel is set to that value,
   otherwise *minvalue is unaffected */
   
   	tLabel curlabel;
	
   	if (Interior_Label( image, row, col ))
	{
		curlabel = Read_Label( image, row, col );
		if (curlabel != 0 && 
		    curlabel < *minlabel) *minlabel = curlabel;
	}
}

/* ============================================================	*/
/* =			    Minimum_Label		      = */
/* ============================================================	*/

tLabel Minimum_Label( tLabelArray *image, int row, int col )
{
/* Return the minimum label around the given (row,col) in the
   given image. If the minimum label is equal to BLACK (0), it
   means that all the surrounding pixels are either unlabeled or 
   already labeled to the background */
   
   	tLabel minlabel = MAXLABELS;
	
	Minimum_Label_Neighbor( image, row-1, col, &minlabel );
	Minimum_Label_Neighbor( image, row, col-1, &minlabel );
	Minimum_Label_Neighbor( image, row, col+1, &minlabel );
	Minimum_Label_Neighbor( image, row+1, col, &minlabel );

   if (CONNECTIVITY == EIGHT_CONNECTIVITY)
   {
	Minimum_Label_Neighbor( image, row-1, col-1, &minlabel );
	Minimum_Label_Neighbor( image, row-1, col+1, &minlabel );
	Minimum_Label_Neighbor( image, row+1, col-1, &minlabel );
	Minimum_Label_Neighbor( image, row+1, col+1, &minlabel );
   }
	
	return( minlabel );
}

/* ============================================================	*/
/* =			    Update_Lookup_Neighbor	      = */
/* ============================================================	*/

void Update_Lookup_Neighbor( tLabelArray *image, int row, int col, tLabel label, tLabel *lookup )
{
/* Update the lookup table by changing the entry of the label
   at (row,col) to the given label if that entry keeps a valid 
   label above 0 */

	tLabel pixel;

   if (Interior_Label( image, row, col ))
   {
	pixel = Read_Label( image, row, col );
	if (pixel != 0 && pixel != UNLABELED && pixel != label)
	   if (pixel > label) lookup[pixel] = label;
   }
}

/* ============================================================	*/
/* =			    Update_Lookup		      = */
/* ============================================================	*/

void Update_Lookup( tLabelArray *image, int row, int col, tLabel label, tLabel *lookup )
{
/* Update the lookup table by changing the entries of the pixels
   around (row,col) to the given label whenever those entries
   keep valid labels above 0 */

	Update_Lookup_Neighbor( image, row-1, col, label, lookup );
	Update_Lookup_Neighbor( image, row, col-1, label, lookup );
	Update_Lookup_Neighbor( image, row, col+1, label, lookup );
	Update_Lookup_Neighbor( image, row+1, col, label, lookup );

   if (CONNECTIVITY == EIGHT_CONNECTIVITY)
   {
	Update_Lookup_Neighbor( image, row-1, col-1, label, lookup );
	Update_Lookup_Neighbor( image, row-1, col+1, label, lookup );
	Update_Lookup_Neighbor( image, row+1, col-1, label, lookup );
	Update_Lookup_Neighbor( image, row+1, col+1, label, lookup );
   }
}


/* ============================================================	*/
/* =			    Label_Pixel			      = */
/* ============================================================	*/

void Label_Pixel( tLabelArray *image, int row, int col, tLabel *curlab, tLabel *lookup )
{
/* Label the pixel at position (row,col) from the given label image.
   *curlab keeps the value of the next unassigned label. lookup[x] 
   keeps the label equivalent to label x. */

	tLabel minlabel, newlabel;

	minlabel = Minimum_Label( image, row, col );
	if (minlabel == MAXLABELS)
	{
		newlabel = *curlab;
		(*curlab)++;
	}
	else if (minlabel > 0) newlabel = minlabel;
	Write_Label( image, row, col, newlabel );
	Update_Lookup( image, row, col, newlabel, lookup );
}

/* ============================================================	*/
/* =			    Readjust_Lookup		      = */
/* ============================================================	*/

void Readjust_Lookup( tLabel *lookup, tLabel *numlabels )
{
/* Each entry of the lookup table is followed up and
   substituted for the last value in the chain. The
   procedure receives the maximun number of labels found 
   during the first labeling phase and returns through 
   the same parameter the number of labels after readjustment */
   
	long i,j,k;
	tLabel numlab;
	
	numlab = *numlabels;
	
	k=0;
	for (i=0; i<*numlabels; i++)
	if (lookup[i] > 0)
	{
		/* Find end of chain */
		j = i;
		while (lookup[j] > 0) j = lookup[j];
		   if (lookup[j] < 0) lookup[i] = lookup[j];
		   else lookup[i] = j;
		numlab--;
	}
	else
	{
		lookup[i] = -k;
		k++;
	}
	
	for (i=0; i<*numlabels; i++) lookup[i] = -lookup[i];
	
	*numlabels = numlab; /* number of different labels */
	
}


/* ============================================================	*/
/* =			    Label_Binary_Image		      = */
/* ============================================================	*/

tLabel Label_Binary_Image( tGrayImage *image, tLabelArray *labels )
{
/* Label the separate white regions of a given binary image. Return
   the number of labels found */

	long i;
	int row, col;
	tLabel lookup[MAXLABELS];
	tLabel curlabel = 1;
	unsigned char pixel;
	tLabel label;
	
	/* Initialize label image */
	for (row=0; row<image->rows; row++)
	for (col=0; col<image->cols; col++)
	{
		BMP_Read_Gray_Pixel( image, row, col, &pixel );
		if (pixel == BLACK) Write_Label( labels, row, col, 0 );
		else if (pixel == WHITE) Write_Label( labels, row, col, UNLABELED );

/*printf("BinaryImage(%d,%d)=%d\tBLACK %d\tWHITE %d\n",row,col,pixel,BLACK,WHITE);
if (pixel == WHITE) { 
printf("WHITE(%d,%d)=%d\tBLACK %d\tWHITE %d\n",row,col,pixel,BLACK,WHITE);
printf("labels(%d,%d)=%ld\tUNLABELED %ld\n",row,col,labels->label[row*labels->cols+col],UNLABELED);
getch();}*/


	}
	
	/* Initialize lookup table for keeping equivalence
	   between different labels */
	for (i=0; i<MAXLABELS; i++) lookup[i] = 0;
	
	/* Start labeling phase 1 */

	//printf("Ini OK\n");
			
	for (row=0; row<image->rows; row++)
	for (col=0; col<image->cols; col++)
	if (Read_Label( labels, row, col ) == UNLABELED) /* unlabeled pixel */
	{
		Label_Pixel( labels, row, col, &curlabel, lookup ); 
	}

	Readjust_Lookup( lookup, &curlabel );
	//printf("Fase1 OK\n");
	/* Start labeling phase 2 */
	
	for (row=0; row<image->rows; row++)
	for (col=0; col<image->cols; col++)
	{
		label = Read_Label( labels, row, col );
		if (label > 0) 
		if (lookup[label] != -1)
			Write_Label( labels, row, col, lookup[label] );
	}
	//printf("Fase2 OK\n");

	return( curlabel );  /* Number of labels */
}


