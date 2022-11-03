#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <omp.h>
#include <math.h>
#include <stdbool.h>

#define NUM_THREADS	16

#define FILENAME "mandel.ppm"

struct timespec Diff_timespec(struct timespec start, struct timespec end)
{
	struct timespec temp;
	if ((end.tv_nsec-start.tv_nsec)<0) {
		temp.tv_sec = end.tv_sec-start.tv_sec-1;
		temp.tv_nsec = 1000000000+end.tv_nsec-start.tv_nsec;
	} else {
		temp.tv_sec = end.tv_sec-start.tv_sec;
		temp.tv_nsec = end.tv_nsec-start.tv_nsec;
	}
	return temp;
}


void color(char *img, int w, int x, int y, int red, int green, int blue)
{
    
    int p;
    
    p = y*w*3 + x*3;
    img[p]=red;
    img[p+1]=green;
    img[p+2]=blue;
}

void dump_img( char *img, int w, int h, FILE *f )
{
	int x, y;
	int p;
	int red, green, blue;

	for(y = 0; y < h; y++)
    	for(x = 0; x < w; x++)
	{
   		p = y*w*3 + x*3;
		red = img[p];
    		green = img[p+1];
    		blue = img[p+2];
	
    		fputc((char)red, f);
    		fputc((char)green, f);
    		fputc((char)blue, f);
	}
}

int main(int argc, char *argv[])
{
    int w = 640*4, h = 480*4, x, y; 
    //each iteration, it calculates: newz = oldz*oldz + p, where p is the current pixel, and oldz stars at the origin
    double pr, pi;                   //real and imaginary part of the pixel p
    double newRe, newIm, oldRe, oldIm;   //real and imaginary parts of new and old z
    double zoom = 1, moveX = -0.5, moveY = 0; //you can change these to zoom and change position
    int maxIterations = 2000;//after how much iterations the function should stop

    double z;
    int brightness;
    int i;
    FILE *f;
    struct timespec ini, end, diff;
    char *img;
    
    img = malloc(w*h*3);

    omp_set_num_threads( NUM_THREADS );  // Set maximum number of OpenMP threads

    
    f = fopen(FILENAME,"wb");

    fprintf(f, "P6\n# CREATOR: E.T / mandel program\n");
    fprintf(f, "%d %d\n255\n",w,h);
    
    

    clock_gettime( CLOCK_REALTIME, &ini );

    //loop through every pixel
    #pragma omp parallel for schedule(static) collapse(2)
    for(y = 0; y < h; y++){
        for(x = 0; x < w; x++){       
                //calculate the initial real and imaginary part of z, based on the pixel location and zoom and position values
                pr = 1.5 * (x - w / 2) / (0.5 * zoom * w) + moveX;
                pi = (y - h / 2) / (0.5 * zoom * h) + moveY;
                newRe = newIm = oldRe = oldIm = 0; //these should start at 0,0
                //"i" will represent the number of iterations
                //start the iteration process
                for(i = 0; i < maxIterations; i++)
                {
                    //remember value of previous iteration
                    oldRe = newRe;
                    oldIm = newIm;
                    //the actual iteration, the real and imaginary part are calculated
                    newRe = oldRe * oldRe - oldIm * oldIm + pr;
                    newIm = 2 * oldRe * oldIm + pi;
                    //if the point is outside the circle with radius 2: stop
                    if((newRe * newRe + newIm * newIm) > 4){
                        break;
                    } 
                }

            if(i == maxIterations)
                    color(img, w, x, y, 0, 0, 0); // black
                else
                {
                    z = sqrt(newRe * newRe + newIm * newIm);
                    brightness = 256. * log2(1.75 + i - log2(log2(z))) / log2((double)maxIterations);
                    color(img, w, x, y, brightness, brightness, 255);
                }
            }
        }

    clock_gettime( CLOCK_REALTIME, &end );
    diff = Diff_timespec( ini, end );  

    dump_img( img, w, h, f );   
    fclose(f);
    free(img);
    
printf("Wall time = %ld ms\n",  diff.tv_sec * 1000 + diff.tv_nsec/1000000 );	

    return 0;
}
