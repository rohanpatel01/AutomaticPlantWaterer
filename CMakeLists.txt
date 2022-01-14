cmake_minimum_required(VERSION 3.21)
project(AutoPlant)

set(CMAKE_CXX_STANDARD 14)

set(${mainProgram}_BOARD uno)
set(${mainProgram}_PORT COM3)


include_directories(.)
include_directories(mainProgram)

add_executable(AutoPlant
        mainProgram/mainProgram.ino)
