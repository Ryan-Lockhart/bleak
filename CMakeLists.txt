cmake_minimum_required(VERSION 3.20)

project(bleakdepth)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_compile_options(-Wall -Wextra -Wpedantic -g -o0)

find_package(fmt CONFIG REQUIRED)

find_package(SDL2 CONFIG REQUIRED)

find_package(SDL2_image CONFIG REQUIRED)
find_package(SDL2_mixer CONFIG REQUIRED)

find_package(SDL2_net CONFIG REQUIRED)
find_package(SDL2_ttf CONFIG REQUIRED)

add_library(bleak SHARED
    lib/array.hpp
    lib/array/iter.tpp
    lib/array/row.tpp
    lib/array/layer.tpp
    lib/array/volume.tpp
    lib/atlas.hpp
    lib/cardinal.hpp
    lib/clock.hpp
    lib/color.hpp
    lib/coord.hpp
    lib/cursor.hpp
    lib/glyph.hpp
    lib/input.hpp
    lib/keyboard.hpp
    lib/line.hpp
    lib/log.hpp
    lib/lut.hpp
    lib/mouse.hpp
    lib/octant.hpp
    lib/path.hpp
    lib/point.hpp
    lib/rect.hpp
    lib/renderer.hpp
    lib/sprite.hpp
    lib/subsystem.hpp
    lib/texture.hpp
    lib/timer.hpp
    lib/tree.hpp
    lib/typedef.hpp
    lib/wave.hpp
    lib/window.hpp
    lib/constants/bindings.hpp
    lib/constants/characters.hpp
    lib/constants/colors.hpp
    lib/constants/glyphs.hpp
    lib/constants/keys.hpp
    lib/constants/octants.hpp
)

set_target_properties(bleak PROPERTIES LINKER_LANGUAGE CXX)

target_link_libraries(bleak PUBLIC fmt::fmt)

target_include_directories(bleak PUBLIC lib)

target_link_libraries(bleak PUBLIC $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main> $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>)

target_link_libraries(bleak PUBLIC $<IF:$<TARGET_EXISTS:SDL2_image::SDL2_image>,SDL2_image::SDL2_image,SDL2_image::SDL2_image-static>)
target_link_libraries(bleak PUBLIC $<IF:$<TARGET_EXISTS:SDL2_mixer::SDL2_mixer>,SDL2_mixer::SDL2_mixer,SDL2_mixer::SDL2_mixer-static>)

target_link_libraries(bleak PUBLIC $<IF:$<TARGET_EXISTS:SDL2_net::SDL2_net>,SDL2_net::SDL2_net,SDL2_net::SDL2_net-static>)
target_link_libraries(bleak PUBLIC $<IF:$<TARGET_EXISTS:SDL2_ttf::SDL2_ttf>,SDL2_ttf::SDL2_ttf,SDL2_ttf::SDL2_ttf-static>)

add_executable(bleakdepth
    src/program.cpp
    src/test.cpp
)

set_target_properties(bleakdepth PROPERTIES LINKER_LANGUAGE CXX)

target_link_libraries(bleakdepth PRIVATE bleak)

configure_file(resources/cursor.png ${CMAKE_CURRENT_BINARY_DIR}/resources/cursor.png COPYONLY)
configure_file(resources/grid_cursor.png ${CMAKE_CURRENT_BINARY_DIR}/resources/grid_cursor.png COPYONLY)

configure_file(resources/glyphs_8x8.png ${CMAKE_CURRENT_BINARY_DIR}/resources/glyphs_8x8.png COPYONLY)
configure_file(resources/glyphs_12x12.png ${CMAKE_CURRENT_BINARY_DIR}/resources/glyphs_12x12.png COPYONLY)
configure_file(resources/glyphs_16x16.png ${CMAKE_CURRENT_BINARY_DIR}/resources/glyphs_16x16.png COPYONLY)
configure_file(resources/glyphs_24x24.png ${CMAKE_CURRENT_BINARY_DIR}/resources/glyphs_24x24.png COPYONLY)
