rm a.out
shopt -s globstar
cd include
if [[ $(wc -l **/*.o) ]] 
then
   echo "Found .o files - compiling from existing precompiled files"
   echo "Compiling..."
   g++-11 -std=c++20 **/*.o ../*.cpp -o ../a.out -lSDL2 -mavx2 -pthread -O3
else
   echo "Didn't find .o files"
   echo "Precompile!"
fi


echo "Finished."