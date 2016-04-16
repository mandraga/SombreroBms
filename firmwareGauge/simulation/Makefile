#
# Makefile for the ply display
#
#

CC = g++
INCLUDES = "/usr/include/"
SDL2_INCLUDES="/usr/local/include/"
TINYBINDINCLUDES="../../tinybind-master/"
COMMONINCLUDES="../../common/"
FLAGS = -Wall -g -c -I$(INCLUDES) -I$(SDL2_INCLUDES) -I$(TINYBINDINCLUDES) -I$(COMMONINCLUDES)
#FLAGS += -I$(TINYBINDINCLUDES)
SDL2_DIR="/usr/local/lib/"
LIBDIRS = -L$(OPENCL_DIR) -L$(SDL2_DIR)
LIBS = -lSDL2 -lSDL2_ttf -lSDL2_image -lGL -lpthread -lm
LIBS += -lSDL2_gfx

OBJDIR=obj
SRCDIR=.
EXE_NAME=vbat.bin

clean:
	rm -f $(OBJDIR)/*.o
	rm -f *.bin
	rm -f *~
	rm -f COMMONSRCDIR/*~

main.o: main.h tcpclient.h threads.h
tcpclient.o: tcpclient.h messages_xml_decoder.h
threads.o: main.h tcpclient.h threads.h
messages_xml_decoder.o: messages_xml_decoder.h

OBJS =  $(OBJDIR)/main.o
OBJS += $(OBJDIR)/sdlcalls.o
OBJS += $(OBJDIR)/keypress.o
OBJS += $(OBJDIR)/vbat.o
OBJS += $(OBJDIR)/app.o
OBJS += $(OBJDIR)/gfxareas.o

$(OBJS): $(OBJDIR)/%.o : $(SRCDIR)/%.cpp
	@echo "-------- : $<"
	$(CC) $(FLAGS) $< -o $@

all: $(OBJS)
	$(CC) $(OBJS) $(COMMONOBJS) $(LIBDIRS) $(LIBS) -o $(EXE_NAME)

