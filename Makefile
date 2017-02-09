CC = g++
CFLAGS = -I

OBJS =		typingspeed.o

WORDS =       typingspeedwords

LIBS =      -lncurses

TARGET =	/usr/bin/typingspeed



$(TARGET):	$(OBJS)
	cp $(WORDS) /usr/bin
	$(CC) -std=c++11 main.cpp -o $(TARGET) $(LIBS)

all:	$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET) 
