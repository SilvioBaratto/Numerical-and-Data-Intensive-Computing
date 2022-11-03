prova: prova.o
	gcc -o prova prova.o -lgomp
	
prova.o: prova.c
	gcc -c prova.c -O2 -fopenmp
		
clean:
	rm -f task1
	rm *.o
