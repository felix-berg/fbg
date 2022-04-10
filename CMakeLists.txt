cmake_minimum_required(VERSION 3.10)

project(fbg)
add_library(fbg INTERFACE)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")

add_subdirectory(external/SDL)
add_subdirectory(external/fbtt)

target_include_directories(${PROJECT_NAME} INTERFACE include)

target_include_directories(${PROJECT_NAME} 
   INTERFACE include
   INTERFACE external/fbtt/include
   INTERFACE external/SDL/include
)

target_link_directories(${PROJECT_NAME}
   INTERFACE src
   INTERFACE external/SDL/src
)

target_link_libraries(${PROJECT_NAME}
   INTERFACE fbtt
   INTERFACE SDL2
)

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

if (NOT EXISTS "${PROJECT_SOURCE_DIR}/external/fbtt/CMakeLists.txt" OR 
    NOT EXISTS "${PROJECT_SOURCE_DIR}/external/SDL/CMakeLists.txt")

    message(FATAL_ERROR "Couldn't load SDL or fbtt!")
endif()