cmake_minimum_required(VERSION 3.10)

# Project name: fbg
project(fbg VERSION 1.0.1 DESCRIPTION "Felix Berg 2D Graphics Library")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (UNIX OR MINGW)
   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -mavx2 -std=c++20") # avx, O3 and c++20
   message("Unix or Mingw detected")
elseif (WIN32)
   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /O2 /arch:AVX2 /std:c++20")
endif()


# update modules (git submodule update --init --recursive)
find_package(Git QUIET)
IF (GIT_FOUND AND EXISTS "${PROJECT_SOURCE_DIR}/.git")
   option(GIT_SUBMODULE "Check submodules during build" ON)
   if (GIT_SUBMODULE)
      execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
                      WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                      RESULT_VARIABLE GIT_SUBMOD_RESULT)
      if (NOT GIT_SUBMOD_RESULT EQUAL "0")
         message(FATAL_ERROR "git submodule update failed")
      endif()
   endif()
endif()

# check that fbtt and sdl have cmakelists
if (NOT EXISTS "${PROJECT_SOURCE_DIR}/external/fbtt/CMakeLists.txt" OR 
    NOT EXISTS "${PROJECT_SOURCE_DIR}/external/SDL/CMakeLists.txt")

    message(FATAL_ERROR "Couldn't load SDL or fbtt!")
endif()

# include files needed for setting target
include_directories(${PROJECT_NAME} 
   PRIVATE include
)

# set target by making a library
# with source files
add_library(${PROJECT_NAME}
   src/Shapes/circle.cpp    src/Shapes/image.cpp  src/Shapes/line.cpp  
   src/Shapes/polyline.cpp  src/Shapes/rect.cpp   src/Shapes/shape.cpp  
   src/Shapes/triangle.cpp

   src/alphacomposite.cpp  src/frame.cpp  src/rgba.cpp  
   src/SDL_Handler.cpp     src/Window.cpp

   src/Shapes/BMP/bmp.cpp

   src/Shapes/drawing_algorithms/bresenham.cpp     src/Shapes/drawing_algorithms/drawcircle.cpp  
   src/Shapes/drawing_algorithms/drawline.cpp      src/Shapes/drawing_algorithms/drawpixel.cpp  
   src/Shapes/drawing_algorithms/drawpolyline.cpp  src/Shapes/drawing_algorithms/drawrect.cpp
)

# allow for make install
configure_file(${PROJECT_NAME}.pc.in ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.pc @ONLY)

# add the to cmake directories "external/fbtt" and "external/SDL"
add_subdirectory(external/fbtt)
add_subdirectory(external/SDL)

# headers for all directories
target_include_directories(${PROJECT_NAME}
   PRIVATE external/fbtt/include
   PRIVATE external/SDL/include
   PUBLIC include
)

target_link_directories(${PROJECT_NAME}
   PRIVATE external/SDL/src
)

target_link_libraries(${PROJECT_NAME}
   PRIVATE SDL2main
   PRIVATE SDL2
   PUBLIC fbtt
)

# allow for make install
install (
   TARGETS ${PROJECT_NAME} DESTINATION lib
)
install (
   DIRECTORY include/ DESTINATION include
)