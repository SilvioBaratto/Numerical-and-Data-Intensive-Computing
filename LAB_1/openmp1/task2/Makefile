task2: task2.o
	gcc -o task2 task2.o -lgomp
	
task2.o: task2.c
	gcc -c task2.c -O2 -fopenmp
		
clean:
	rm -f task2
	rm *.o
