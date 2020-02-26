CC		:= g++
R_FLAGS := -O2
D_FLAGS := -g -O0 -Wall -Wextra
C_FLAGS := -c -std=c++11 -static -static-libgcc -static-libstdc++ 

BIN		:= lib
SRC		:= src
INCLUDE	:= -iquote include -I external/include
LIB		:= #-L external/lib

LIBRARIES	:= #-lglfw3 -lgdi32 -lglad

RELEASE_BUILD_NAME := libmantaray.a
RELEASE_BUILD := $(BIN)/$(RELEASE_BUILD_NAME)

DEBUG_BUILD_NAME := libmantaray-d.a
DEBUG_BUILD := $(BIN)/$(DEBUG_BUILD_NAME)

ifeq ($(OS),Windows_NT)
DEFINES	:= -D PLATFORM_WINDOWS
else
DEFINES	:= -D PLATFORM_LINUX
endif

all: $(RELEASE_BUILD) $(DEBUG_BUILD)

release: $(RELEASE_BUILD)

debug: $(DEBUG_BUILD)

clean:
	$(RM) $(RELEASE_BUILD)
	$(RM) $(DEBUG_BUILD)

$(RELEASE_BUILD): $(SRC)/*.cpp $(SRC)/*.c
	$(CC) $(R_FLAGS) $(C_FLAGS) $(DEFINES) $(INCLUDE) $^ $(LIB) $(LIBRARIES)
	ar rvs $@ *.o
	rm *.o

$(DEBUG_BUILD): $(SRC)/*.cpp $(SRC)/*.c
	$(CC) $(D_FLAGS) $(C_FLAGS) $(DEFINES) $(INCLUDE) $^ $(LIB) $(LIBRARIES)
	ar rvs $@ *.o
	rm *.o
