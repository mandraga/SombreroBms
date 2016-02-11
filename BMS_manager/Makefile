#
# Makefile
#
#

ifeq ($(OS), Windows_NT)
OPERATINGON=__WINDOWS
else
OPERATINGON=__LINUX
endif
RELEASE=TRUE

CC = g++
INCLUDES = "/usr/include/"

ifeq ($(OPERATINGON),__LINUX)
else
	ifdef RELEASE
		CONSOLEFLAG=
	else
		CONSOLEFLAG=-mconsole
	endif
endif

ifdef RELEASE
FLAGS = -Wall -O2 -c -D$(OPERATINGON) `fltk-config --cxxflags` -I$(INCLUDES)
else
FLAGS = -Wall -g -c -D_DEBUG -D$(OPERATINGON) `fltk-config --cxxflags` -I$(INCLUDES)
endif

LIBDIRS =
ifeq ($(OPERATINGON),__WINDOWS)
endif

ifeq ($(OPERATINGON),__LINUX)
LIBS += -lGL
LIBS += -lSDL2 -lSDL2_ttf -lSDL2_image
else
LIBS += -lOpenGL32 
LIBS += `sdl2-config --libs`
LIBS += -lSDL2_ttf -lSDL2_image 
LIBS += -lWs2_32
endif
#LIBS += -lpthread
LIBS +=  -ltinyxml
LIBS += `fltk-config --use-images --ldflags`

OBJDIR=obj
SRCDIR=.
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
	rm -f COMMONSRCDIR/*~
	rm -f $(OUTPUT_DIR)$(EXE_NAME)

OBJS =  $(OBJDIR)/main.o
OBJS += $(OBJDIR)/info_tab.o
OBJS += $(OBJDIR)/setup_tab.o
OBJS += $(OBJDIR)/input_tab.o
OBJS += $(OBJDIR)/serialport.o

$(OBJS): $(OBJDIR)/%.o : $(SRCDIR)/%.cpp
	@echo "-------- : $<"
	$(CC) $(FLAGS) $< -o $@

all: $(OBJS)
	$(CC) $(OBJS) $(LIBDIRS) $(LIBS) -o $(OUTPUT_DIR)$(EXE_NAME) $(CONSOLEFLAG)

