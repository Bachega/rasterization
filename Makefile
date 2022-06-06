all:
	g++ -Isrc/include -Lsrc/lib -o drawer main.cpp raster.hpp raster.cpp polygon.hpp polygon.cpp -lmingw32 -lSDL2main -lSDL2