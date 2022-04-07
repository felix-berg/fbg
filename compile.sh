#!/bin/bash
rm a.out

echo "Lines of code: " $(wc -l **/*.cpp **/*.hpp | tr -d ' ' | awk 'END {print $NF}') "
"


files=
echo $files

echo "Compiling..."
g++-11 -std=c++20 -c ./include/**/*.cpp -lSDL2 -mavx2 -pthread -O3
# g++-11 -std=c++20 *.o ../*.cpp -lSDL2 -mavx2 -pthread -O3

echo "Finished."