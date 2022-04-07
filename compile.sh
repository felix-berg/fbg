rm a.out
shopt -s globstar
cd include
echo "Lines of code: " $(wc -l **/*.cpp **/*.hpp | tr -d ' ' | awk 'END {print $NF}') "
"


echo "Compiling..."
g++-11 -std=c++20 **/*.cpp ../*.cpp -lSDL2 -mavx2 -pthread -O3

echo "Finished."