#
# Makefile, wavefront file loader
#
#

ifdef SystemRoot
OPERATINGON=__WINDOWS
else
OPERATINGON=__LINUX
endif
RELEASE=TRUE

CC = g++
INCLUDES = "/usr/include/"
COMMONINCLUDE = "../common/"

CC = g++
C  = gcc
CFLAGS = -Wall -g -c -I$(INCLUDES) -I$(COMMONINCLUDE)
LIB = -Lsystem/
ifeq ($(OPERATINGON),__LINUX)
EXT=
else
EXT=.exe
endif

TARGET_NAME=meshloader.a

default: all

clean:
	rm -f *.o
	rm -f *.bin
	rm -f *~
	rm -f $(TARGET_NAME)
	rm -f $(EXE_NAME)$(EXT)
	rm -f test_lex_yy$(EXT)

.PHONY: test
test:
	cat cleffa.obj | ./test_lex_yy$(EXT)

flexObjects = lex.yy.o yywrap.o
objects = meshloader.o parser.o

lex.yy.o: objmesh.l parse_codes.h yywrap.c
	flex objmesh.l
	$(C) -c lex.yy.c
	$(C) -c yywrap.c
	$(C) -c test_lex_yy.c
	$(C) $(flexObjects) test_lex_yy.o -o test_lex_yy$(EXT) $(CONSOLEFLAG)
#-lfl

$(objects): %.o: %.cpp
	@echo "-------- Makes: $<"
	$(CC) $(CFLAGS) $<

all: lex.yy.o $(objects)
	@echo "-------- Makes: $(EXE_NAME)$(EXT)"
	ar rs $(TARGET_NAME) $(flexObjects) $(objects)

