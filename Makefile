CC = g++
CFLAGS = -Wall
PROG = carrom

SRCS = carrom.cpp Coin.h __Header__.h Board.h
LIBS = -std=c++11 -lglut -lGL -lGLU -lm

all: $(PROG)

$(PROG):	$(SRCS)
	$(CC) $(CFLAGS) -o $(PROG) $(SRCS) $(LIBS)

clean:
	rm -f $(PROG)
