g++-11 -std=c++20 -c ./Graphics_Lib/*.hpp -lSDL2 -mavx2 -pthread -O3
g++-11 -std=c++20 -c ./Graphics_Lib/Shapes/*.hpp -lSDL2 -mavx2 -pthread -O3
g++-11 -std=c++20 -c ./Graphics_Lib/Shapes/drawing_algorithms/*.hpp -lSDL2 -mavx2 -pthread -O3
g++-11 -std=c++20 -c ./Graphics_Lib/Shapes/BMP/*.hpp -lSDL2 -mavx2 -pthread -O3
g++-11 -std=c++20 -c 2D_Graphics_Lib.hpp -lSDL2 -mavx2 -pthread -O3