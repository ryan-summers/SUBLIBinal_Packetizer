# This makefile compiles the library into a library object file

VPATH=src/

# Variables
LCC= ar
LCFLAGS= rvs
CC=gcc
CFLAGS= -g -I$(INC_PATH)

# Sources and Objects list
SOURCES=$(foreach sdir, $(SRC_DIR), $(wildcard $(sdir)*.c))
OBJECTS=$(patsubst $(SRC_DIR)%.c, $(BUILD_PATH)%.o, $(SOURCES))
EXE=libSUBLIBinal_packetizer.a

# Paths
INC_PATH=include/
SRC_DIR=src/
BUILD_PATH=build/

all: checkdirs $(EXE)

$(EXE) : $(OBJECTS)
	$(LCC) $(LCFLAGS) $(BUILD_PATH)$(EXE) $(OBJECTS)

$(BUILD_PATH)%.o: $(SRC_DIR)%.c
	$(CC) -c $(CFLAGS) $< -o $@

clean: 
	@rm -rf $(BUILD_PATH)*.o $(BUILD_PATH)*.a

checkdirs :$(BUILD_PATH)

$(BUILD_PATH):
	@mkdir -p $@
