CC 		= gcc
CPP		= g++
#	$(BLDDIR)
#		| - *.o
#		| - *.o
#		| - ...
#	$(INCDIR)
#		| - *.h
#		| - ...
#	$(SRCDIR)
#		| - *.c
#		| - *.cpp
#		| - ...
#	$(OUT)
#	makefile

INCDIR 	:= inc
SRCDIR	:= src
BLDDIR  := build
OUT		:= main

# Compiler flags
CFLAGS		:= -Wall -Wextra
CPPFLAGS 	:=
LFLAGS		:= -lGLEW -lglut -lGL

# recursive include and source directories
RINCDIRS	:= $(shell find $(INCDIR) -type d)
RSRCDIRS	:= $(shell find $(SRCDIR) -type d)

# set the include dir
IFLAGS		:= $(foreach dir, $(RINCDIRS), $(patsubst %,-I%/, $(dir)))


INC 	:= $(foreach dir, $(RINCDIRS), $(wildcard $(dir)/*.h))
INC 	+= $(foreach dir, $(RINCDIRS), $(wildcard $(dir)/*.hpp))
SRC		:= $(foreach dir, $(RSRCDIRS), $(wildcard $(dir)/*.c) $(wildcard $(dir)/*.cpp))
OBJ		:= $(foreach dir, $(RSRCDIRS), $(patsubst %.cpp, $(BLDDIR)/%.o, $(foreach file, $(wildcard $(dir)/*.cpp), $(shell echo $(file) | sed 's:src/::'))))
OBJ		+= $(foreach dir, $(RSRCDIRS), $(patsubst %.c, $(BLDDIR)/%.o, $(foreach file, $(wildcard $(dir)/*.c), $(shell echo $(file) | sed 's:src/::'))))


OBJFOLDER := /

$(OUT): $(OBJ)
	@echo ""
	$(CPP) -o $@ $^ $(LFLAGS)

$(BLDDIR)/%.o: $(SRCDIR)/%.c $(INC)
	$(eval OBJFOLDER=$(shell echo '$@' | sed 's|\(.*\)/.*|\1|' | xargs mkdir -p))
	$(CC) $(CFLAGS) -c $< $(IFLAGS) -o $@

$(BLDDIR)/%.o: $(SRCDIR)/%.cpp $(INC)
	$(eval OBJFOLDER=$(shell echo '$@' | sed 's|\(.*\)/.*|\1|' | xargs mkdir -p))
	$(CPP) $(CFLAGS) -c $< $(IFLAGS) -o $@


clean:
	rm -f $(OBJ)