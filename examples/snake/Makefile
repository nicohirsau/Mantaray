CC		:= g++
D_FLAGS := -g -Wall -Wextra
C_FLAGS := -std=c++11

BIN		:= bin
SRC		:= src
INCLUDE	:= -I ../../include -I ../../external/include
LIB		:= -L external/lib -L ../../lib


EXECUTABLE_NAME := snake
ifeq ($(OS),Windows_NT)
EXECUTABLE	:= $(EXECUTABLE_NAME).exe
C_FLAGS		+= -static -static-libgcc -static-libstdc++
LIBRARIES	:= -lmantaray -lglfw3 -lgdi32
else
EXECUTABLE	:= $(EXECUTABLE_NAME)
C_FLAGS		+= -no-pie
LIBRARIES	:= -lmantaray -lglfw3 -lrt -lm -ldl -lX11 -lpthread -lxcb -lXau -lXdmcp
endif

all: $(BIN)/$(EXECUTABLE)

clean:
	$(RM) $(BIN)/$(EXECUTABLE)

run: all
	./$(BIN)/$(EXECUTABLE)

$(BIN)/$(EXECUTABLE): $(SRC)/*.cpp
	$(CC) $(D_FLAGS) $(C_FLAGS) $(INCLUDE) $^ $(LIB) $(LIBRARIES) -o $@
