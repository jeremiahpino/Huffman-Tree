all: htable

htable: lab3.o
	gcc -o htable -ansi -Wall -pedantic lab3.o
lab3.o: lab3.c
	gcc -ansi -Wall -pedantic -g -c lab3.c
clean:
	rm *.o htable
test:
	./htable INPUT
