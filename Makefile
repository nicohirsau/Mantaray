CC		:= g++
R_FLAGS := -O3
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

all: $(RELEASE_BUILD) $(DEBUG_BUILD)

release: $(RELEASE_BUILD)

debug: $(DEBUG_BUILD)

clean:
	$(RM) $(RELEASE_BUILD)
	$(RM) $(DEBUG_BUILD)

$(RELEASE_BUILD): $(SRC)/*.cpp
	$(CC) $(R_FLAGS) $(C_FLAGS) $(INCLUDE) $(LIB) $^ $(LIBRARIES)
	ar rvs $@ *.o
	rm *.o

$(DEBUG_BUILD): $(SRC)/*.cpp
	$(CC) $(D_FLAGS) $(C_FLAGS) $(INCLUDE) $(LIB) $^ $(LIBRARIES)
	ar rvs $@ *.o
	rm *.o
