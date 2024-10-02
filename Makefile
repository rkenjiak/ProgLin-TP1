TRACE=NDEBUG
CFLAGS=-D$(TRACE) -g -std=c11

simples: bin/main.o bin/problem.o bin/utils.o
	gcc $(CFLAGS) -o simples bin/main.o bin/problem.o bin/utils.o -Wall

bin/main.o: src/main.c
	gcc $(CFLAGS) -c -o bin/main.o src/main.c

bin/problem.o: src/problem.c src/mysimplex.h
	gcc $(CFLAGS) -c -o bin/problem.o src/problem.c

bin/utils.o: src/utils.c src/mysimplex.h
	gcc $(CFLAGS) -c -o bin/utils.o src/utils.c

.PHONY: clean

clean:
	rm -f bin/*.o simples