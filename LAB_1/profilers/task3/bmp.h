/* ============================================================	*/
/* =							      = */
/* =	     		    BMP LIBRARY	  		      = */
/* =							      = */
/* =		Miguel Angel Garcia (October 2002)	      = */
/* =		                    (December 2005)	      = */
/* =							      = */
/* ============================================================	*/

#define PRIVATE static
 
#define TRUE	1
#define FALSE	0

typedef unsigned char tColorPixel[3];
typedef unsigned char tGrayPixel;

typedef struct
{
	int rows, cols;
	tGrayPixel *image;
} tGrayImage;

typedef struct
{
	int rows, cols;
	tColorPixel *image;
} tColorImage;

int BMP_Create_Gray_Image( int, int, tGrayImage* );
int BMP_Read_Gray_Image( char*, tGrayImage* );
int BMP_Write_Gray_Image( char*, tGrayImage* );
int BMP_Read_Gray_Pixel( tGrayImage*, int, int, tGrayPixel* );
int BMP_Write_Gray_Pixel( tGrayImage*, int, int, tGrayPixel );
void BMP_Free_Gray_Image( tGrayImage* );

int BMP_Create_Color_Image( int, int, tColorImage* );
int BMP_Read_Color_Image( char*, tColorImage* );
int BMP_Write_Color_Image( char*, tColorImage* );
int BMP_Read_Color_Pixel( tColorImage*, int, int, tColorPixel* );
int BMP_Write_Color_Pixel( tColorImage*, int, int, tColorPixel );
void BMP_Free_Color_Image( tColorImage* );





