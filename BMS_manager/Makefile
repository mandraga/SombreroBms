#
# Makefile
#
#

ifeq ($(OS), Windows_NT)
OPERATINGON=__WINDOWS
else
OPERATINGON=__LINUX
endif
#RELEASE=TRUE

CC = g++
INCLUDES = "/usr/include/"
GLDIRINC = "./common/"
MESHINC = "./meshloader/"

ifeq ($(OPERATINGON),__LINUX)
else
	ifdef RELEASE
		CONSOLEFLAG=
	else
		CONSOLEFLAG=-mconsole
	endif
endif

ifdef RELEASE
FLAGS = -Wall -O2 -c -D$(OPERATINGON) `fltk-config --cxxflags` -I$(INCLUDES) -I$(GLDIRINC) -I$(MESHINC)
else
FLAGS = -Wall -g -c -D_DEBUG -D$(OPERATINGON) `fltk-config --cxxflags` -I$(INCLUDES) -I$(GLDIRINC) -I$(MESHINC)
endif

LIBDIRS =
ifeq ($(OPERATINGON),__WINDOWS)
endif

ifeq ($(OPERATINGON),__LINUX)
LIBS += -lGL -lGLEW
LIBS += -lSDL2 -lSDL2_ttf -lSDL2_image
else
LIBS += -lOpenGL32 
LIBS += `sdl2-config --libs`
LIBS += -lSDL2_ttf -lSDL2_image 
LIBS += -lWs2_32
endif
LIBS += -lpthread
LIBS += -ltinyxml
LIBS += ./meshloader/meshloader.a
LIBS += `fltk-config --use-images --use-gl --ldflags`

OBJDIR=obj
SRCDIR=.
SERDIR=./serial
GLDIR=./common
appname=BMSmanager
ifeq ($(OPERATINGON),__LINUX)
EXE_NAME=$(appname)
else
EXE_NAME=$(appname).exe
endif
ifeq ($(OPERATINGON),__LINUX)
OUTPUT_DIR=./lbin/
else
OUTPUT_DIR=./wbin/
endif

clean:
	rm -f $(OBJDIR)/*.o
	rm -f *.bin
	rm -f *~
	rm -f $(SERDIR)/*~
	rm -f $(GLDIR)/*~
	rm -f $(OUTPUT_DIR)$(EXE_NAME)

OBJS =  $(OBJDIR)/main.o
OBJS += $(OBJDIR)/cfgfile.o
OBJS += $(OBJDIR)/SombreroBMS.o
OBJS += $(OBJDIR)/port_tab.o
OBJS += $(OBJDIR)/about_tab.o
OBJS += $(OBJDIR)/info_tab.o
OBJS += $(OBJDIR)/setup_tab.o
OBJS += $(OBJDIR)/battery_tab.o
OBJS += $(OBJDIR)/charge_tab.o
OBJS += $(OBJDIR)/thrSerial.o
OBJS += $(OBJDIR)/timer.o

$(OBJS): $(OBJDIR)/%.o : $(SRCDIR)/%.cpp
	@echo "-------- : $<"
	$(CC) $(FLAGS) $< -o $@

SEROBJS = $(OBJDIR)/serialport.o

$(SEROBJS): $(OBJDIR)/%.o : $(SERDIR)/%.cpp
	@echo "-------- : $<"
	$(CC) $(FLAGS) $< -o $@

GLOBJS =  $(OBJDIR)/gl2Dprimitives.o
GLOBJS += $(OBJDIR)/glShaders.o
GLOBJS += $(OBJDIR)/gfxareas.o
GLOBJS += $(OBJDIR)/mesh.o

$(GLOBJS): $(OBJDIR)/%.o : $(GLDIR)/%.cpp
	@echo "-------- : $<"
	$(CC) $(FLAGS) $< -o $@

all: $(OBJS) $(SEROBJS) $(GLOBJS)
	$(CC) $(OBJS) $(SEROBJS) $(GLOBJS) $(LIBDIRS) $(LIBS) -o $(OUTPUT_DIR)$(EXE_NAME) $(CONSOLEFLAG)

