project('libbleak', 'cpp', version: '0.0.1', meson_version: '>=1.3.0')

install_dir = '@0@/lib/@1@/@2@'.format(
	meson.project_source_root(),
	target_machine.system(),
	target_machine.cpu_family()
)

build_type = get_option('buildtype')

if build_type == 'debug'
    args = ['-DBLEAK_DEBUG']
else
    args = ['-DBLEAK_RELEASE', '-DNDEBUG']
endif

sources = [
    'bleak.hpp',

    'bleak/applicator.hpp',
    'bleak/arc.hpp',
    'bleak/area.hpp',
    'bleak/array.hpp',
    'bleak/camera.hpp',
    'bleak/cardinal.hpp',
    'bleak/cell.hpp',
    'bleak/circle.hpp',
    'bleak/clip_pool.hpp',
    'bleak/clock.hpp',
    'bleak/color.hpp',
    'bleak/concepts.hpp',
    'bleak/constants.hpp',
    'bleak/creeper.hpp',
    'bleak/cursor.hpp',
    'bleak/extent.hpp',
    'bleak/field.hpp',
    'bleak/gamepad.hpp',
    'bleak/glyph.hpp',
    'bleak/hash.hpp',
    'bleak/input.hpp',
    'bleak/iter.hpp',
    'bleak/keyboard.hpp',
    'bleak/line.hpp',
    'bleak/log.hpp',
    'bleak/lut.hpp',
    'bleak/memory.hpp',
    'bleak/mixer.hpp',
    'bleak/mouse.hpp',
    'bleak/octant.hpp',
    'bleak/offset.hpp',
    'bleak/path.hpp',
    'bleak/primitive.hpp',
    'bleak/primitive_types.hpp',
    'bleak/quadrant.hpp',
    'bleak/random.hpp',
    'bleak/rect.hpp',
    'bleak/region.hpp',
    'bleak/renderer.hpp',
    'bleak/sound.hpp',
    'bleak/sprite.hpp',
    'bleak/steam.hpp',
    'bleak/subsystem.hpp',
    'bleak/texture.hpp',
    'bleak/timer.hpp',
    'bleak/tree.hpp',
    'bleak/triangle.hpp',
    'bleak/typedef.hpp',
    'bleak/utility.hpp',
    'bleak/wave.hpp',
    'bleak/window.hpp',
    'bleak/zone.hpp',

    'bleak/constants/bindings.hpp',
    'bleak/constants/characters.hpp',
    'bleak/constants/colors.hpp',
    'bleak/constants/glyphs.hpp',
    'bleak/constants/keys.hpp',
    'bleak/constants/numeric.hpp',
    'bleak/constants/octants.hpp',
]

cxx = meson.get_compiler('cpp')

if target_machine.system() == 'windows'

    std_deps = [
        cxx.find_library('gcc_s_seh-1', dirs: 'c:/dev/msys64/ucrt64/bin', required: true, static: false),
        cxx.find_library('stdc++', required: true, static: false),
        cxx.find_library('winpthread', required: true, static: false),
        cxx.find_library('steam_api', dirs: 'c:/dev/steam/sdk/redistributable_bin/win64', required: true, static: false),
    ]

    sdl_deps = [
        dependency('SDL2', required: true, static: false),
        dependency('SDL2_image', required: true, static: false),
        dependency('SDL2_mixer', required: true, static: false),
        dependency('SDL2_net', required: true, static: false),
        dependency('SDL2_ttf', required: true, static: false),
    ]

elif target_machine.system() == 'linux'

    std_deps = [
        cxx.find_library('gcc_s', required: true, static: false),
        cxx.find_library('stdc++', required: true, static: false),
        cxx.find_library('pthread', dirs: '/usr/lib64', required: true, static: false),
        cxx.find_library('steam_api', required: true, static: false),
    ]

    sdl_deps = [
        cxx.find_library('SDL2', dirs: '/usr/local/lib', required: true, static: false),
        cxx.find_library('SDL2_image', dirs: '/usr/local/lib', required: true, static: false),
        cxx.find_library('SDL2_mixer', dirs: '/usr/local/lib', required: true, static: false),
        cxx.find_library('SDL2_net', dirs: '/usr/local/lib', required: true, static: false),
        cxx.find_library('SDL2_ttf', dirs: '/usr/local/lib', required: true, static: false),
    ]

endif

libbleak = both_libraries(
    'bleak',

    sources,

    dependencies: [
        std_deps,
        sdl_deps
    ],

    c_args: args,
    cpp_args: args,

    install: true,
    install_dir: install_dir,
)

libbleak_dep = declare_dependency(
    include_directories: include_directories(['.']),
    link_with: [libbleak.get_shared_lib()],
    link_whole: [libbleak.get_static_lib()],
)
