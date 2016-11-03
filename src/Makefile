all:makegig

makegig:makegig.o note.o
	g++ -o makegig makegig.o note.o -L/usr/local/lib/libgig/ -lgig -lsndfile

makegig.o:makegig.cpp note.h
	g++ -c -Wall -std=c++11 makegig.cpp

note.o:note.cpp note.h
	g++ -c -Wall -std=c++11 note.cpp

clean:
	-rm *.o makegig
