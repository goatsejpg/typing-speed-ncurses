CC = g++
CFLAGS = -I

FILES = main.cpp type.cpp ncursesfuncs.cpp files.cpp

OBJS = typing.o

WORDS = words.txt

LIBS = -lncurses

TARGET = type

$(TARGET):
	$(CC) -std=c++11 $(FILES) -o $(TARGET) $(LIBS)
	

all:	$(SHARE)

clean:
	rm type;
