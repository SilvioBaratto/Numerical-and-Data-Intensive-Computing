
/* =============================================================================== */
/* =																			 = */
/* =																			 = */
/* =		  Miguel Angel Garcia, Domenec Puig, Carles Torras, Ricard Garcia	 = */
/* =																			 = */
/* =============================================================================== */

#define MAXLABELS	100000
#define UNLABELED	MAXLABELS

#ifndef TRUE 
#define TRUE	1
#endif

#ifndef FALSE
#define FALSE	0
#endif

#ifndef BLACK
#define BLACK	0
#endif

#ifndef WHITE
#define WHITE	255
#endif

typedef long tLabel;

typedef struct
{
	tLabel *label;
	int rows, cols;
} tLabelArray;

void Create_Label_Array( int rows, int cols, tLabelArray *array );
int Destroy_Label_Array(  tLabelArray *array );
int Destroy_Label_Array(  tLabelArray *array );
tLabel Read_Label(  tLabelArray *array, int row, int col );
void Write_Label(  tLabelArray *array, int row, int col, tLabel label );
int Interior_Label( tLabelArray *image, int row, int col );
void Minimum_Label_Neighbor( tLabelArray *image, int row, int col, tLabel *minlabel );
tLabel Minimum_Label( tLabelArray *image, int row, int col );
void Update_Lookup_Neighbor( tLabelArray *image, int row, int col, tLabel label, tLabel *lookup );
void Update_Lookup( tLabelArray *image, int row, int col, tLabel label, tLabel *lookup );
void Label_Pixel( tLabelArray *image, int row, int col, tLabel *curlab, tLabel *lookup );
void Readjust_Lookup( tLabel *lookup, tLabel *numlabels );
tLabel Label_Binary_Image( tGrayImage *image, tLabelArray *labels );






