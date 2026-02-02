cmake_minimum_required(VERSION 4.1)
project(cnc_gcode_generator)

set(CMAKE_CXX_STANDARD 20)

add_executable(cnc_gcode_generator
        src/main.cpp
        src/settings.cpp
        src/drl_reader.cpp
        src/drilling_manager.cpp
        src/drilling_gcode_creator.cpp
        src/gcode-file-manager.cpp
        external/minIni.c
)
target_include_directories(cnc_gcode_generator PRIVATE
        include
        external
)

