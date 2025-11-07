#----------------------------------------------------------------------------
# Setup the project
cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
project(BetaMon)

find_package(Geant4 REQUIRED ui_all vis_all)
include(${Geant4_USE_FILE})
include_directories(${PROJECT_SOURCE_DIR}/include ${PROJECT_SOURCE_DIR}/include/external)

INCLUDE(FindROOT.cmake)
INCLUDE_DIRECTORIES(SYSTEM ${ROOT_INCLUDE_DIR})

## GNU prof tools
#set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -pg")
#set(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_CXX_FLAGS} -pg")
#set(CMAKE_DCMAKE_SHARED_LINKER_FLAGS  "${CMAKE_CXX_FLAGS} -pg")

## Google perf-tools
#set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -fno-builtin-malloc -fno-builtin-calloc -fno-builtin-realloc -fno-builtin-free")
#set(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_CXX_FLAGS} -l:libtcmalloc.so.4")
#set(CMAKE_DCMAKE_SHARED_LINKER_FLAGS  "${CMAKE_CXX_FLAGS} -ltcmalloc")

#----------------------------------------------------------------------------
# Locate sources and headers for this project
# NB: headers are included so they will show up in IDEs
#
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)

add_definitions(-g -O3 -std=c++17 -W -Wall -Wpointer-arith)
#----------------------------------------------------------------------------
# Add the executable, and link it to the Geant4 libraries
#
add_executable(BetaMon BetaMon.cc ${sources} ${headers})
target_link_libraries(BetaMon ${Geant4_LIBRARIES} ${ROOT_LIBRARIES})
link_directories(/fastdata)
target_link_libraries(BetaMon gsl gslcblas)

#----------------------------------------------------------------------------
# Copy all scripts to the build directory, i.e. the directory in which we
# build B1. This is so that we can run the executable directly because it
# relies on these scripts being in the current working directory.
#
set(EXAMPLEB1_SCRIPTS
  #BetaMon.in
  #BetaMon.out
  init.mac
  init_vis.mac
  run1.mac
  run2.mac
  run3.mac
  vis.mac
)

foreach(_script ${EXAMPLEB1_SCRIPTS})
  configure_file(
    ${PROJECT_SOURCE_DIR}/${_script}
    ${PROJECT_BINARY_DIR}/${_script}
    COPYONLY
    )
endforeach()

#----------------------------------------------------------------------------
# For internal Geant4 use - but has no effect if you build this
# example standalone
#
add_custom_target(B1 DEPENDS BetaMon)

#----------------------------------------------------------------------------
# Install the executable to 'bin' directory under CMAKE_INSTALL_PREFIX
#
install(TARGETS BetaMon DESTINATION bin)
