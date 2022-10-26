algi: algi.o label.o bmp.o
	gcc -o algi algi.o label.o bmp.o -lm -lgomp -lrt
	
algi.o: algi.c
	gcc -c algi.c -O2 -fopenmp
	
label.o: label.c label.h
	gcc -c label.c -O2 -fopenmp
	
bmp.o: bmp.c bmp.h
	gcc -c bmp.c -O2 -fopenmp

clean:
	rm -f *.o
	rm -f algi.bmp fat.bmp joint.bmp
	rm -f algi
