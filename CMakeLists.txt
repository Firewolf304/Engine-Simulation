cmake_minimum_required(VERSION 3.28)
project(EngineSimulation)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DDEBUG" )

add_executable(EngineSimulation main.cpp
        parse/config_parser.hpp
        parse/data.h
        engine/engine_simulate.hpp
        engine/tester.hpp)
