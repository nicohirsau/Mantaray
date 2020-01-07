CC		:= g++
D_FLAGS := -g -Wall -Wextra
C_FLAGS := -c -std=c++11 -static -static-libgcc -static-libstdc++ 

BIN		:= lib
SRC		:= src
INCLUDE	:= -iquote include -I external/include
LIB		:= -L external/lib -L lib

LIBRARIES	:= -lglfw3 -lgdi32 -lglad

BUILD_NAME := libmantaray.a
BUILD := $(BIN)/$(BUILD_NAME)

DEBUG_BUILD_NAME := libmantaray-d.a
DEBUG_BUILD := $(BIN)/$(DEBUG_BUILD_NAME)

all: $(BUILD)

debug: $(DEBUG_BUILD)

clean:
	$(RM) $(BUILD)
	$(RM) $(DEBUG_BUILD)

$(BUILD): $(SRC)/*.cpp
	$(CC) $(C_FLAGS) $(INCLUDE) $(LIB) $^ $(LIBRARIES)
	ar rvs $@ *.o
	rm *.o

$(DEBUG_BUILD): $(SRC)/*.cpp
	$(CC) $(D_FLAGS) $(C_FLAGS) $(INCLUDE) $(LIB) $^ $(LIBRARIES)
	ar rvs $@ *.o
	rm *.o
