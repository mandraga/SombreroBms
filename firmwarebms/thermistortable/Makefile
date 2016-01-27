#
# Makefile for the ply display
#
#

CC = gcc
INCLUDES = "/usr/include/"
FLAGS = -Wall -g -c -I$(INCLUDES)

EXE_NAME=generate


clean:
	rm -f *.o
	rm -f *.bin
	rm -f *~
	rm -f $(EXE_NAME)

all:
	$(CC) $(FLAGS) create_table.c
	$(CC) create_table.o $(LIBDIRS) $(LIBS) -o $(EXE_NAME)
