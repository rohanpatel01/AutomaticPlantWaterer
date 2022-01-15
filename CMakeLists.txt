cmake_minimum_required(VERSION 3.21)
project(AutoPlant)

set(CMAKE_CXX_STANDARD 14)

set(${CMAKE_mainProgram}_BOARD uno)
set(${CMAKE_mainProgram}_PORT COM3 (Arduino Uno))


include_directories(.)
include_directories(mainProgram)

add_executable(AutoPlant
        mainProgram/mainProgram.ino)


