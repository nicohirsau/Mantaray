CC		:= g++
D_FLAGS := -g -Wall -Wextra
C_FLAGS := -std=c++11 -static -static-libgcc -static-libstdc++ #-mwindows

BIN		:= bin
SRC		:= src
INCLUDE	:= -I ../../include -I ../../external/include
LIB		:= -L external/lib -L ../../lib


EXECUTABLE_NAME := snake
ifeq ($(OS),Windows_NT)
EXECUTABLE	:= $(EXECUTABLE_NAME).exe
LIBRARIES	:= -lMantaray -lglfw3 -lgdi32
else
EXECUTABLE	:= $(EXECUTABLE_NAME)
LIBRARIES	:= -lmantaray -lglfw3 -lX11 -lXxf86vm -lXrandr -lpthread -lXi -ldl -lXinerama -lXcursor 
endif

all: $(BIN)/$(EXECUTABLE)

clean:
	$(RM) $(BIN)/$(EXECUTABLE)

run: all
	./$(BIN)/$(EXECUTABLE)

$(BIN)/$(EXECUTABLE): $(SRC)/*.cpp
	$(CC) $(D_FLAGS) $(C_FLAGS) $(INCLUDE) $^ $(LIB) $(LIBRARIES) -o $@
