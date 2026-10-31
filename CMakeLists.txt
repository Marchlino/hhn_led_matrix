cmake_minimum_required(VERSION 3.23)

project(LED_Matrix CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/build)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE Debug)

find_package(SDL2 REQUIRED)
include_directories(LED_Matrix ${SDL2_INCLUDE_DIRS})
find_package(Freetype REQUIRED)
include_directories(LED_Matrix ${FREETYPE_INCLUDE_DIR_freetype2})
include_directories(LED_Matrix ${FREETYPE_INCLUDE_DIR_ft2build})


add_definitions(-DLOG_LEVEL=0)
add_definitions(-DWITH_GZFILEOP)

add_executable( LED_Matrix )

target_sources( LED_Matrix
    PRIVATE
        src/main.cpp
        src/games/Player.cpp
        src/games/MainMenu.cpp
        src/display/Image.cpp
        src/display/drivers/Stdout.cpp
        src/display/drivers/Sdl.cpp
        src/utils/geometry.cpp
        src/utils/colors.cpp
        include/logger/logger.cpp
)

target_include_directories( LED_Matrix
    PRIVATE
        include/logger

)

target_link_libraries( LED_Matrix ${SDL2_LIBRARIES} )
target_link_libraries( LED_Matrix ${FREETYPE_LIBRARIES} )
