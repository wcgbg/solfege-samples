all:makegig

makegig:makegig.o
	g++ -o makegig makegig.o -L/usr/local/lib/libgig/ -lgig -lsndfile

makegig.o:makegig.cpp
	g++ -c -Wall -std=c++11 makegig.cpp

clean:
	-rm *.o makegig
