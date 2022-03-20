rm a.out

echo "Lines of code: " $(wc -l *.cpp Graphics_Lib/*.cpp Graphics_Lib/Shapes/*.cpp Graphics_Lib/*.hpp Graphics_Lib/Shapes/*.hpp Graphics_Lib/Shapes/drawing_algorithms/*.cpp Graphics_Lib/Shapes/drawing_algorithms/*.hpp) "
"
echo "Compiling: " *.cpp Graphics_Lib/*.cpp Graphics_Lib/Shapes/*.cpp Graphics_Lib/Shapes/drawing_algorithms/*.cpp"

"
g++ -std=c++20 *.cpp Graphics_Lib/*.cpp Graphics_Lib/Shapes/*.cpp Graphics_Lib/Shapes/drawing_algorithms/*.cpp -lSDL2 -mavx2 -pthread -O3