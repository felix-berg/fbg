shopt -s globstar
g++-11 -std=c++20 -c ./include/**/*.hpp -lSDL2 -mavx2 -pthread -O3
cd include
g++-11 -std=c++20 -c **/*.cpp -lSDL2 -mavx2 -pthread -O3