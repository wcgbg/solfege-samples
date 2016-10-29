all:makegig gigwritedemo

makegig:makegig.o
	g++ -o makegig makegig.o -L/usr/local/lib/libgig/ -lgig 

makegig.o:makegig.cpp
	g++ -c -Wall -std=c++11 makegig.cpp

gigwritedemo:gigwritedemo.o
	g++ -o gigwritedemo gigwritedemo.o -L/usr/local/lib/libgig/ -lgig 

gigwritedemo.o:gigwritedemo.cpp
	g++ -c -Wall gigwritedemo.cpp

clean:
	-rm *.o makegig gigwritedemo
