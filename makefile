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

INCDIR 	:= inc External/inc
SRCDIR	:= src External/src
BLDDIR  := build
OUT		:= main

# Compiler flags
CFLAGS		:= -Wall -Wextra
CPPFLAGS 	:=
LFLAGS		:= -lGLEW -lglfw -lGL

# recursive include and source directories
RINCDIRS	:= $(foreach dir, $(INCDIR), $(shell find $(dir) -type d))
RSRCDIRS	:= $(foreach dir, $(SRCDIR), $(shell find $(dir) -type d))

# set the include dir
IFLAGS		:= $(foreach dir, $(RINCDIRS), $(patsubst %,-I%/, $(dir)))


INC 	:= $(foreach dir, $(RINCDIRS), $(wildcard $(dir)/*.hpp))
INCH	:= $(foreach dir, $(RINCDIRS), $(wildcard $(dir)/*.h))
SRC		:= $(foreach dir, $(RSRCDIRS), $(wildcard $(dir)/*.c) $(wildcard $(dir)/*.cpp))
OBJ		:= $(foreach dir, $(RSRCDIRS), $(patsubst %.cpp, $(BLDDIR)/%.o, $(wildcard $(dir)/*.cpp)))
OBJ		+= $(foreach dir, $(RSRCDIRS), $(patsubst %.c  , $(BLDDIR)/%.o, $(wildcard $(dir)/*.c)))


OBJFOLDER := /

$(OUT): $(OBJ)
	@echo ""
	$(CPP) -o $@ $^ $(IFLAGS) $(LFLAGS)

$(BLDDIR)/%.o: %.c $(INC)
	$(eval OBJFOLDER=$(shell echo '$@' | sed 's|\(.*\)/.*|\1|' | xargs mkdir -p))
	$(CC) $(CFLAGS) -c $< -o $@ $(IFLAGS)

$(BLDDIR)/%.o: %.cpp $(INC)
	$(eval OBJFOLDER=$(shell echo '$@' | sed 's|\(.*\)/.*|\1|' | xargs mkdir -p))
	$(CPP) $(CFLAGS) -c $< -o $@ $(IFLAGS)


clean:
	rm -f $(OBJ)