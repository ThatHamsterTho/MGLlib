SHELL:=/bin/bash
#### PROJECT SETTINGS ####
# The name of the executable to be created
BIN_NAME := main
# The name of the lib headerfile
HeaderFile := MGLlib.hpp
# Compiler used
CXX ?= g++
# Extension of source files used in the project
SRC_EXT = cpp
# Path to the source directory, relative to the makefile
SRC_PATH = src
# Space-separated pkg-config libraries used by this project
LIBS =
# General compiler flags
COMPILE_FLAGS = -std=c++17 -Wall -Wextra
# Additional release-specific flags
RCOMPILE_FLAGS = -D NDEBUG
# Additional debug-specific flags
DCOMPILE_FLAGS = -D DEBUG
# Add additional include paths
INCDIR := inc
# Exclude these folders from LIB (these are inside inc)
EXCLUDEDIR := tests external
# gets all the directories inside the inc folder
RINCDIRS := $(foreach dir, $(INCDIR), $(shell find $(dir) -type d))
INCLUDES := $(foreach dir, $(RINCDIRS), $(patsubst %, -I%, $(dir))) -ILibraries/

# General linker settings
ifeq ($(OS), Windows_NT)
	LINK_FLAGS = -lopengl32 -lglu32 -llibglew32 -llibglfw3
else 
	LINK_FLAGS = -lGLEW -lglfw -lGL
endif
# Additional release-specific linker settings
RLINK_FLAGS = -o2
# Additional debug-specific linker settings
DLINK_FLAGS = -ggdb3
# Destination directory, like a jail or mounted system
DESTDIR = 
# Install path (bin/ is appended automatically)
INSTALL_PREFIX = 

# for lib
BuildDir := build/release
#### END PROJECT SETTINGS ####

# Optionally you may move the section above to a separate config.mk file, and
# uncomment the line below
# include config.mk

# Generally should not need to edit below this line

# Obtains the OS type, either 'Darwin' (OS X) or 'Linux'
UNAME_S:=$(shell uname -s)

# Function used to check variables. Use on the command line:
# make print-VARNAME
# Useful for debugging and adding features
print-%: ; @echo $*=$($*)

# Shell used in this makefile
# bash is used for 'echo -en'
SHELL = /bin/bash
# Clear built-in rules
.SUFFIXES:
# Programs for installation
INSTALL = install
INSTALL_PROGRAM = $(INSTALL)
INSTALL_DATA = $(INSTALL) -m 644

# Append pkg-config specific libraries if need be
ifneq ($(LIBS),)
	COMPILE_FLAGS += $(shell pkg-config --cflags $(LIBS))
	LINK_FLAGS += $(shell pkg-config --libs $(LIBS))
endif

# Verbose option, to output compile and link commands
export V := false
export CMD_PREFIX := @
ifeq ($(V),true)
	CMD_PREFIX :=
endif

# Combine compiler and linker flags
release: export CXXFLAGS := $(CXXFLAGS) $(COMPILE_FLAGS) $(RCOMPILE_FLAGS)
release: export LDFLAGS := $(LDFLAGS) $(LINK_FLAGS) $(RLINK_FLAGS)
debug: export CXXFLAGS := $(CXXFLAGS) $(COMPILE_FLAGS) $(DCOMPILE_FLAGS)
debug: export LDFLAGS := $(LDFLAGS) $(LINK_FLAGS) $(DLINK_FLAGS)

# Build and output paths
release: export BUILD_PATH := build/release
release: export BIN_PATH := bin/release
debug: export BUILD_PATH := build/debug
debug: export BIN_PATH := bin/debug
install: export BIN_PATH := bin/release

# Find all source files in the source directory, sorted by most
# recently modified
ifeq ($(UNAME_S),Darwin)
	SOURCES = $(shell find $(SRC_PATH) -name '*.$(SRC_EXT)' | sort -k 1nr | cut -f2-)
else
	SOURCES = $(shell find $(SRC_PATH) -name '*.$(SRC_EXT)' -printf '%T@\t%p\n' \
						| sort -k 1nr | cut -f2-)
endif

# fallback in case the above fails
rwildcard = $(foreach d, $(wildcard $1*), $(call rwildcard,$d/,$2) \
						$(filter $(subst *,%,$2), $d))
ifeq ($(SOURCES),)
	SOURCES := $(call rwildcard, $(SRC_PATH), *.$(SRC_EXT))
endif

# Set the object file names, with the source directory stripped
# from the path, and the build path prepended in its place
OBJECTS = $(SOURCES:$(SRC_PATH)/%.$(SRC_EXT)=$(BUILD_PATH)/%.o)
# Set the dependency files that will be used to add header dependencies
DEPS = $(OBJECTS:.o=.d)

# Macros for timing compilation
ifeq ($(UNAME_S),Darwin)
	CUR_TIME = awk 'BEGIN{srand(); print srand()}'
	TIME_FILE = $(dir $@).$(notdir $@)_time
	START_TIME = $(CUR_TIME) > $(TIME_FILE)
	END_TIME = read st < $(TIME_FILE) ; \
		$(RM) $(TIME_FILE) ; \
		st=$$((`$(CUR_TIME)` - $$st)) ; \
		echo $$st
else
	TIME_FILE = $(dir $@).$(notdir $@)_time
	START_TIME = date '+%s' > $(TIME_FILE)
	END_TIME = read st < $(TIME_FILE) ; \
		$(RM) $(TIME_FILE) ; \
		st=$$((`date '+%s'` - $$st - 86400)) ; \
		echo `date -u -d @$$st '+%H:%M:%S'`
endif

# Version macros
# Comment/remove this section to remove versioning
USE_VERSION := false
# If this isn't a git repo or the repo has no tags, git describe will return non-zero
ifeq ($(shell git describe > /dev/null 2>&1 ; echo $$?), 0)
	USE_VERSION := true
	VERSION := $(shell git describe --tags --long --dirty --always | \
		sed 's/v\([0-9]*\)\.\([0-9]*\)\.\([0-9]*\)-\?.*-\([0-9]*\)-\(.*\)/\1 \2 \3 \4 \5/g')
	VERSION_MAJOR := $(word 1, $(VERSION))
	VERSION_MINOR := $(word 2, $(VERSION))
	VERSION_PATCH := $(word 3, $(VERSION))
	VERSION_REVISION := $(word 4, $(VERSION))
	VERSION_HASH := $(word 5, $(VERSION))
	VERSION_STRING := \
		"$(VERSION_MAJOR).$(VERSION_MINOR).$(VERSION_PATCH).$(VERSION_REVISION)-$(VERSION_HASH)"
	override CXXFLAGS := $(CXXFLAGS) \
		-D VERSION_MAJOR=$(VERSION_MAJOR) \
		-D VERSION_MINOR=$(VERSION_MINOR) \
		-D VERSION_PATCH=$(VERSION_PATCH) \
		-D VERSION_REVISION=$(VERSION_REVISION) \
		-D VERSION_HASH=\"$(VERSION_HASH)\"
endif

# Standard, non-optimized release build
.PHONY: release
release: dirs
ifeq ($(USE_VERSION), true)
	@echo "Beginning release build v$(VERSION_STRING)"
else
	@echo "Beginning release build"
endif
	@$(START_TIME)
	@$(MAKE) all --no-print-directory
	@echo -n "Total build time: "
	@$(END_TIME)

# Debug build for gdb debugging
.PHONY: debug
debug: dirs
ifeq ($(USE_VERSION), true)
	@echo "Beginning debug build v$(VERSION_STRING)"
else
	@echo "Beginning debug build"
endif
	@$(START_TIME)
	@$(MAKE) all --no-print-directory
	@echo -n "Total build time: "
	@$(END_TIME)

# Create the directories used in the build
.PHONY: dirs
dirs:
	@echo "Creating directories"
	@mkdir -p $(dir $(OBJECTS))
	@mkdir -p $(BIN_PATH)

# Installs to the set path
.PHONY: install
install:
	@echo "Installing to $(DESTDIR)$(INSTALL_PREFIX)/bin"
	@$(INSTALL_PROGRAM) $(BIN_PATH)/$(BIN_NAME) $(DESTDIR)$(INSTALL_PREFIX)/bin

# Uninstalls the program
.PHONY: uninstall
uninstall:
	@echo "Removing $(DESTDIR)$(INSTALL_PREFIX)/bin/$(BIN_NAME)"
	@$(RM) $(DESTDIR)$(INSTALL_PREFIX)/bin/$(BIN_NAME)

# Removes all build files
.PHONY: clean
clean:
	@echo "Deleting $(BIN_NAME) symlink"
	@$(RM) $(BIN_NAME)
	@echo "Deleting directories"
	@$(RM) -r build
	@$(RM) -r bin

# Main rule, checks the executable and symlinks to the output
all: $(BIN_PATH)/$(BIN_NAME)
	@echo "Making symlink: $(BIN_NAME) -> $<"
	@$(RM) $(BIN_NAME)
	@ln -s $(BIN_PATH)/$(BIN_NAME) $(BIN_NAME)

# Link the executable
$(BIN_PATH)/$(BIN_NAME): $(OBJECTS)
	@echo "Linking: $@"
	@$(START_TIME)
	$(CMD_PREFIX)$(CXX) $(OBJECTS) $(LDFLAGS) -o $@
	@echo -en "\t Link time: "
	@$(END_TIME)

# Add dependency files, if they exist
-include $(DEPS)

# Source file rules
# After the first compilation they will be joined with the rules from the
# dependency files to provide header dependencies
$(BUILD_PATH)/%.o: $(SRC_PATH)/%.$(SRC_EXT)
	@echo "Compiling: $< -> $@"
	@$(START_TIME)
	$(CMD_PREFIX)$(CXX) $(CXXFLAGS) $(INCLUDES) -MP -MMD -c $< -o $@
	@echo -en "\t Compile time: "
	@$(END_TIME)

































OBJEXCLUDEDIRS  := tests
OBJEXCLUDEFILES := main.o

ROBJDIRS := $(foreach dir, $(BuildDir), $(shell find $(dir) -type d))
RODIRS	 := $(shell echo $(ROBJDIRS) | sed --regexp-extended 's/([A-Z]|[a-z]|\/)*($(OBJEXCLUDEDIRS))([^ ])*//g';)
ROBJS	 := $(foreach dir, $(RODIRS), $(wildcard $(dir)/*.o))
ROBJSFIN := $(shell echo $(ROBJS) | sed --regexp-extended 's/([A-Z]|[a-z]|\/)*($(OBJEXCLUDEFILES))()*//g';)

RINCDIRS := $(foreach dir, $(INCDIR), $(shell find $(dir) -type d))
EXPR 	 := $(shell echo $(EXCLUDEDIR) | sed 's/ /|/g';)
RIDIRS	 := $(shell echo $(RINCDIRS) | sed --regexp-extended 's/([A-Z]|[a-z]|\/)*($(EXPR))([^ ])*//g';)
RINCS 	 := $(foreach dir, $(RIDIRS), $(wildcard $(dir)/*.hpp) $(wildcard $(dir)/*.h))

#external
#Hfiles := $(shell echo $(RINCS) | grep -E "(inc\/*filepath*)([^ ])*((\.hpp)|(\.h))")
#UtilFunc
HfilesPRE := $(shell echo $(RINCS) | grep -o -E "(inc\/Core\/UtilFunc)([^ ])*((\.hpp)|(\.h))")
# Debug
HfilesPRE += $(shell echo $(RINCS) | grep -o -E "(inc\/Core\/Debug)([^ ])*((\.hpp)|(\.h))")
# Primitives
HfilesPRE += $(shell echo $(RINCS) | grep -o -E "(inc\/Core\/Primitives\/VertexBufferObject)([^ ])*((\.hpp)|(\.h))")
HfilesPRE += $(shell echo $(RINCS) | grep -o -E "(inc\/Core\/Primitives\/VertexBufferLayout)([^ ])*((\.hpp)|(\.h))")
HfilesPRE += $(shell echo $(RINCS) | grep -o -E "(inc\/Core\/Primitives\/IndexBufferObject)([^ ])*((\.hpp)|(\.h))")
HfilesPRE += $(shell echo $(RINCS) | grep -o -E "(inc\/Core\/Primitives)([^ ])*((\.hpp)|(\.h))")
# Shapes Core
HfilesPRE += $(shell echo $(RINCS) | grep -o -E "(inc\/Core\/Shapes\/GenericAbstractShape)([^ ])*((\.hpp)|(\.h))")
HfilesPRE += $(shell echo $(RINCS) | grep -o -E "(inc\/Core\/Shapes)([^ ])*((\.hpp)|(\.h))")
# 3Drender
HfilesPRE += $(shell echo $(RINCS) | grep -o -E "(inc\/Core\/3Drender)([^ ])*((\.hpp)|(\.h))")
# Shapes
HfilesPRE += $(shell echo $(RINCS) | grep -o -E "(inc\/Shapes)([^ ])*((\.hpp)|(\.h))")
# other
HfilesPRE += $(shell echo $(RINCS) | grep -o -E "(inc\/ShapeHandler)([^ ])*((\.hpp)|(\.h))")
HfilesPRE += $(shell echo $(RINCS) | grep -o -E "(inc\/Window)([^ ])*((\.hpp)|(\.h))")
HfilesPRE += $(shell echo $(RINCS) | grep -o -E "(inc\/)([^ \/])*((\.hpp)|(\.h))")
# Controllers

Hfiles := $(shell echo $(HfilesPRE) | awk -v RS="[ \n]+" '!n[$$0]++') 

lib: $(ROBJS)
	rm -f MGLlib.lib MGLlib.hpp
	touch MGLlib.hpp
	@echo "" > "$(HeaderFile)"

	@$(foreach header, $(Hfiles), echo "//$(header) : " >> $(HeaderFile); echo "" >> $(HeaderFile); cat $(header) >> $(HeaderFile); echo "" >> $(HeaderFile);)
	@cat "$(HeaderFile)" | sed 's/#include "/\/\/#include "/g' > temp
	@mv temp "$(HeaderFile)"
	@cat "$(HeaderFile)" | sed 's/#include <GL\/glew.h>//g' > temp
	@mv temp "$(HeaderFile)"
	@cat "$(HeaderFile)" | sed 's/#include <GLFW\/glfw3.h>//g' > temp
	@mv temp "$(HeaderFile)"
	@cat "$(HeaderFile)" | sed 's/#include <glm\/glm.h>//g' > temp
	@mv temp "$(HeaderFile)"

	@echo "#include <GL/glew.h>		// sudo apt-get install libglew-dev" > temp
	@echo "#include <GLFW/glfw3.h>	// sudo apt-get install libglfw3-dev" >> temp
	@echo "#include <glm/glm.hpp>	// sudo apt-get install libglm-dev" >> temp
	@cat "$(HeaderFile)" >> temp
	@mv temp "$(HeaderFile)"

	@$(foreach obj, $(ROBJSFIN), echo $(obj);)

	@ar rcs MGLlib.lib $(ROBJSFIN)
