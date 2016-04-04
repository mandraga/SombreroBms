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
STKINC = "../"

ifeq ($(OPERATINGON),__LINUX)
else
	ifdef RELEASE
		CONSOLEFLAG=
	else
		CONSOLEFLAG=-mconsole
	endif
endif

ifdef RELEASE
FLAGS = -Wall -O2 -c -D$(OPERATINGON) -I$(INCLUDES) -I$(STKINC)
else
FLAGS = -Wall -g -c -D_DEBUG -D$(OPERATINGON) -I$(INCLUDES) -I$(STKINC)
endif

LIBDIRS =
ifeq ($(OPERATINGON),__WINDOWS)
endif

ifeq ($(OPERATINGON),__LINUX)
else
endif
LIBS += -lpthread

OBJDIR=.
SRCDIR=.
appname=test_stkbootloader_cmd
ifeq ($(OPERATINGON),__LINUX)
EXE_NAME=$(appname)
else
EXE_NAME=$(appname).exe
endif
OUTPUT_DIR=./

clean:
	rm -f $(OBJDIR)/*.o
	rm -f *.bin
	rm -f *~
	rm -f $(SERDIR)/*~
	rm -f $(GLDIR)/*~
	rm -f $(OUTPUT_DIR)$(EXE_NAME)

OBJS =  $(OBJDIR)/main.o
OBJS += $(OBJDIR)/boot_test.o

$(OBJS): $(OBJDIR)/%.o : $(SRCDIR)/%.cpp
	@echo "-------- : $<"
	$(CC) $(FLAGS) $< -o $@

all: $(OBJS)
	$(CC) $(OBJS) $(LIBDIRS) $(LIBS) -o $(OUTPUT_DIR)$(EXE_NAME) $(CONSOLEFLAG)

