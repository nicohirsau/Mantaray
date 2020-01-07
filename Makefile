CC		:= g++
D_FLAGS := -g -Wall -Wextra
C_FLAGS := -c -std=c++11 -static -static-libgcc -static-libstdc++ 

BIN		:= lib
SRC		:= src
INCLUDE	:= -iquote include -I external/include
LIB		:= -L external/lib -L lib

LIBRARIES	:= -lglfw3 -lgdi32 -lglad

BUILD_NAME := libmantaray.a
BUILD := $(BIN)/libmantaray.a

all: $(BIN)/$(BUILD_NAME)

clean:
	$(RM) $(BUILD)

$(BIN)/$(BUILD_NAME): $(SRC)/*.cpp
	$(CC) $(D_FLAGS) $(C_FLAGS) $(INCLUDE) $(LIB) $^ $(LIBRARIES)
	ar rvs $@ *.o
	rm *.o