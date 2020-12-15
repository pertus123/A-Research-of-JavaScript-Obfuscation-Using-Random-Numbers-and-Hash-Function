exetesthtml2: test.o sha256.o
	g++ -o exetesthtml2 test.o sha256.o

test.o: test.cpp
	g++ -c test.cpp

sha256.o: sha256.cpp
	g++ -c sha256.cpp

clean:
	rm *.o
