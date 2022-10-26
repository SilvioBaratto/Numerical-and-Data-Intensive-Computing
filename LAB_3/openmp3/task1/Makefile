task1: task1.o
	gcc -o task1 task1.o -lgomp -lm
	
task1.o: task1.c
	gcc -c task1.c -fopenmp
		
clean:
	rm -f task1
	rm *.o
