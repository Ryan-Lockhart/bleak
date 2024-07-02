project(
    'bleakdepth',
    'cpp',
    version: '0.0.1',
)

cpu_family = target_machine.cpu_family()
if cpu_family == 'x86'
    install_dir = 'C:\\Program Files (x86)\\Bleakdepth'
elif cpu_family == 'x86_64'
    install_dir = 'C:\\Program Files\\Bleakdepth'
endif

install_res_dir = install_dir + '\\res'

sdl_dependencies = [
    dependency('sdl2'),
    dependency('sdl2_image'),
    dependency('sdl2_mixer'),
    dependency('sdl2_net'),
    dependency('sdl2_ttf'),
]

lib_sources = [
    'lib/array.hpp',
    'lib/atlas.hpp',
    'lib/cardinal.hpp',
    'lib/clock.hpp',
    'lib/color.hpp',
    'lib/coord.hpp',
    'lib/cursor.hpp',
    'lib/glyph.hpp',
    'lib/input.hpp',
    'lib/keyboard.hpp',
    'lib/line.hpp',
    'lib/log.hpp',
    'lib/lut.hpp',
    'lib/mouse.hpp',
    'lib/octant.hpp',
    'lib/path.hpp',
    'lib/point.hpp',
    'lib/rect.hpp',
    'lib/renderer.hpp',
    'lib/sprite.hpp',
    'lib/subsystem.hpp',
    'lib/texture.hpp',
    'lib/timer.hpp',
    'lib/tree.hpp',
    'lib/typedef.hpp',
    'lib/wave.hpp',
    'lib/window.hpp',

    'lib/constants/bindings.hpp',
    'lib/constants/characters.hpp',
    'lib/constants/colors.hpp',
    'lib/constants/glyphs.hpp',
    'lib/constants/keys.hpp',
    'lib/constants/octants.hpp',
]

libbleak = static_library(
    'bleak',
    lib_sources,
    dependencies: sdl_dependencies,

    override_options: [
        'cpp_std=c++26',
        'cpp_stdlib=libc++'
    ],
)

bleak_dependency = declare_dependency(
    link_with: libbleak,
    include_directories: include_directories('lib'),
)

exe_source = [
    'src/program.cpp',
    'src/test.cpp',
]

executable(
    'bleakdepth',
    exe_source,

    dependencies: [
        bleak_dependency,
        sdl_dependencies,
    ],

    override_options: [
        'cpp_std=c++26',
        'cpp_stdlib=libc++',
    ],

    install: true,
    install_dir: install_dir,
)

install_subdir('log', install_dir: install_dir)
install_subdir('res', install_dir: install_dir)
