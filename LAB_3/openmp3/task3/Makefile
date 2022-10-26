task3: task3.o
	gcc -o task3 task3.o -lgomp -lm
	
task3.o: task3.c
	gcc -c task3.c -fopenmp
		
clean:
	rm -f task3
	rm *.o
