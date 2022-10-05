task4: task4.o
	gcc -o task4 task4.o -lgomp
	
task4.o: task4.c
	gcc -c task4.c -O2 -fopenmp
		
clean:
	rm -f task4
	rm *.o
