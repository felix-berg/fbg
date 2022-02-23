rm a.out
echo "Lines of code: " $(wc -l *.cpp Graphics_Lib/*.cpp Graphics_Lib/Shapes/*.cpp)
echo "Compiling: " *.cpp Graphics_Lib/*.cpp Graphics_Lib/Shapes/*.cpp
g++ *.cpp Graphics_Lib/*.cpp Graphics_Lib/Shapes/*.cpp -lSDL2
./a.out