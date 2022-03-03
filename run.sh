rm a.out
echo "
"
echo "Lines of code: " $(wc -l *.cpp Graphics_Lib/*.cpp Graphics_Lib/Shapes/*.cpp Graphics_Lib/*.hpp Graphics_Lib/Shapes/*.hpp) "
"
echo "Compiling: " *.cpp Graphics_Lib/*.cpp Graphics_Lib/Shapes/*.cpp "


"
g++ *.cpp Graphics_Lib/*.cpp Graphics_Lib/Shapes/*.cpp -lSDL2 -mavx2 -pthread -O3
./a.out