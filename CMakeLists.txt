cmake_minimum_required(VERSION 3.22)
project(PlanetsSimulation)

set(CMAKE_CXX_STANDARD 20)

add_executable(PlanetsSimulation main.cpp)

find_package(SFML 2.5 COMPONENTS system window graphics network audio REQUIRED)
include_directories(${SFML_INCLUDE_DIRS})
target_link_libraries(PlanetsSimulation sfml-system sfml-window sfml-graphics sfml-audio sfml-network)