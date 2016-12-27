CC = g++
CFLAGS = -Wall -std=c++11 -I../../sf2cute/include/
LDFLAGS = -lsndfile -L../../sf2cute/ -lsf2cute -L/usr/local/lib/libgig/ -lgig
SRCS = $(wildcard *.cpp)
OBJS = $(patsubst %.cpp, %.o, $(SRCS))
DEPS = $(patsubst %.o, %.d, $(OBJS))

all:katy daisy chengu jennifer

KATY_OBJS = katy.o note.o sf2gigbuilder.o
katy:$(KATY_OBJS)
	$(CC) -o katy $(KATY_OBJS) $(LDFLAGS)

DAISY_OBJS = daisy.o note.o sf2gigbuilder.o
daisy:$(DAISY_OBJS)
	$(CC) -o daisy $(DAISY_OBJS) $(LDFLAGS)

CHENGU_OBJS = chengu.o note.o sf2gigbuilder.o
chengu:$(CHENGU_OBJS)
	$(CC) -o chengu $(CHENGU_OBJS) $(LDFLAGS)

JENNIFER_OBJS = jennifer.o note.o sf2gigbuilder.o
jennifer:$(JENNIFER_OBJS)
	$(CC) -o jennifer $(JENNIFER_OBJS) $(LDFLAGS)

-include $(DEPS)

%.o: %.cpp
	$(CC) -c -o $@ $< $(CFLAGS)

clean:
	-rm *.o katy daisy chengu jennifer
