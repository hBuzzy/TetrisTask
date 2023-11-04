cmake_minimum_required(VERSION 3.26)
project(tetris)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_PREFIX_PATH "D:/Code/C++/qt6/qt6-static/lib/cmake")

find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)

include_directories(src)

add_executable(tetris main.cpp
        src/tetris/tetris.cpp
        src/tetris/tetris.ui
        src/gamefield/table.cpp
        src/gamefield/gamefield.cpp
        src/figure/figure_generator.hpp
        src/figure/figure.hpp
        src/figure/figure.cpp)

target_link_libraries(tetris
        Qt::Core
        Qt::Gui
        Qt::Widgets
)

file(COPY
        src/gamefield/gamefield.h
        src/gamefield/table.h
        DESTINATION ${PROJECT_NAME}_autogen/include)
