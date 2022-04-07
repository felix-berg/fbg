shopt -s globstar
cd include
g++-11 -std=c++20 **/*.cpp ../*.cpp -o ../a.out -lSDL2 -mavx2 -pthread -O3