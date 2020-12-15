test: test.o sha256.o
	g++ -o test test.o sha256.o

test.o: test.c
	g++ -c test.c

sha256.o:sha256.c
	g++ -c sha256.c
clean:
	rm *.o test sha256
