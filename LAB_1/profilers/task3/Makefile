algi: algi.o label.o bmp.o
	gcc -o algi algi.o label.o bmp.o -lm -pg
	
algi.o: algi.c
	gcc -c algi.c -O2 -pg
	
label.o: label.c label.h
	gcc -c label.c -O2 -pg
	
bmp.o: bmp.c bmp.h
	gcc -c bmp.c -O2 -pg

clean:
	rm *.o
	rm algi
