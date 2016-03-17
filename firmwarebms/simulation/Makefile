#
# Makefile for the ply display
#
#

CC = g++
INCLUDES = "/usr/include/"
SDL2_INCLUDES="/usr/local/include/"
COMMONSRCINCLUDES=../../BMS_manager/serial/
FLAGS = -Wall -g -c -D_SIMULATION -I$(INCLUDES) -I$(SDL2_INCLUDES) -I$(COMMONSRCINCLUDES)
SDL2_DIR="/usr/local/lib/"
LIBDIRS = -L$(OPENCL_DIR) -L$(SDL2_DIR)
LIBS = -lSDL2 -lSDL2_ttf -lSDL2_image -lGL -lpthread -lm
#LIBS += -lSDL2_gfx

OBJDIR=obj
SRCDIR=.
AVRSRCDIR=../
COMMONSRCDIR=../../BMS_manager/serial/
EXE_NAME=simulation.bin

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
OBJS += $(OBJDIR)/app.o
OBJS += $(OBJDIR)/gfxareas.o
OBJS += $(OBJDIR)/AD7280A.o
OBJS += $(OBJDIR)/eeprom.o
OBJS += $(OBJDIR)/fake_AD7280A.o
OBJS += $(OBJDIR)/fake_battery.o
OBJS += $(OBJDIR)/fake_charger.o
OBJS += $(OBJDIR)/fake_uart.o
OBJS += $(OBJDIR)/thrSerial.o
OBJS += $(OBJDIR)/inout.o

$(OBJS): $(OBJDIR)/%.o : $(SRCDIR)/%.cpp
	@echo "-------- : $<"
	$(CC) $(FLAGS) $< -o $@

AVROBJS  = $(OBJDIR)/balancing.o
AVROBJS += $(OBJDIR)/state_machine.o
AVROBJS += $(OBJDIR)/serial.o
AVROBJS += $(OBJDIR)/eeprom_functions.o

$(AVROBJS): $(OBJDIR)/%.o : $(AVRSRCDIR)/%.c
	@echo "-------- : $<"
	$(CC) $(FLAGS) $< -o $@

COMMONOBJS  = $(OBJDIR)/serialport.o

$(COMMONOBJS): $(OBJDIR)/%.o : $(COMMONSRCDIR)/%.cpp
	@echo "-------- : $<"
	$(CC) $(FLAGS) $< -o $@

all: $(OBJS) $(AVROBJS) $(COMMONOBJS)
	$(CC) $(OBJS) $(AVROBJS) $(COMMONOBJS) $(LIBDIRS) $(LIBS) -o $(EXE_NAME)

