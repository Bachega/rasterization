all:
	g++ -Isrc/include -Lsrc/lib -o drawer main.cpp raster.hpp raster.cpp -lmingw32 -lSDL2main -lSDL2