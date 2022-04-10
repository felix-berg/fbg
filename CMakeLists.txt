cmake_minimum_required(VERSION 3.10)

project(fbg)
add_library(fbtt INTERFACE)

# add_subdirectory(include)
# add_subdirectory(src)


target_include_directories(${PROJECT_NAME} PRIVATE include)
target_link_directories(${PROJECT_NAME} PUBLIC src)


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

if ()