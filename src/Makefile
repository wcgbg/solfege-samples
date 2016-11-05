all:makegig makesf2

makegig:makegig.o note.o
	g++ -o makegig makegig.o note.o -L/usr/local/lib/libgig/ -lgig -lsndfile

makegig.o:makegig.cpp note.h
	g++ -c -Wall -std=c++11 makegig.cpp

makesf2:makesf2.o note.o
	g++ -o makesf2 makesf2.o note.o -lsndfile -L../../sf2cute/ -l sf2cute

makesf2.o:makesf2.cpp note.h
	g++ -c -Wall -std=c++11 makesf2.cpp -I../../sf2cute/include/

note.o:note.cpp note.h
	g++ -c -Wall -std=c++11 note.cpp

clean:
	-rm *.o makegig makesf2
