CXX = g++
SRC = src/main.cpp
OUT = main.exe

SDL_INCLUDE = C:/SDL2/SDL3-devel-3.4.2-mingw (1)/SDL3-3.4.2/x86_64-w64-mingw32/include
SDL_LIB = C:/SDL2/SDL3-devel-3.4.2-mingw (1)/SDL3-3.4.2/x86_64-w64-mingw32/lib

CXXFLAGS = -std=c++17 -I"$(SDL_INCLUDE)"
LDFLAGS = -L"$(SDL_LIB)" -lSDL3 -lSDL3_image -lSDL3_ttf

all:
	$(CXX) $(SRC) -o $(OUT) $(CXXFLAGS) $(LDFLAGS)

run: all
	.\$(OUT)

clean:
	del /Q $(OUT)