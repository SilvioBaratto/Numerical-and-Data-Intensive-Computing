task1: task1.o
	gcc -o task1 task1.o -lgomp
	
task1.o: task1.c
	gcc -c task1.c -O2 -fopenmp
		
clean:
	rm -f task1
	rm *.o
