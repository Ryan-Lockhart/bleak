project('bleak', 'cpp', version: '0.1.1', meson_version: '>=1.3.0')

build_type = get_option('buildtype')

if build_type == 'debug'
    bleak_args = ['-DBLEAK_DEBUG']
else
    bleak_args = ['-DBLEAK_RELEASE', '-DNDEBUG']
endif

cxx = meson.get_compiler('cpp')

if target_machine.system() == 'windows'

    bleak_internal_includes = include_directories('C:/dev/msys64/ucrt64/include/SDL2', 'C:/dev/steam/include', 'C:/dev/external/magic_enum/include')

    std_deps = [
        cxx.find_library('gcc_eh', dirs: 'C:/dev/msys64/ucrt64/lib/gcc/x86_64-w64-mingw32/15.1.0', required: true, static: true),
        cxx.find_library('stdc++', dirs: 'C:/dev/msys64/ucrt64/lib', required: true, static: true),
        cxx.find_library('winpthread', dirs: 'C:/dev/msys64/ucrt64/lib', required: true, static: true),
        cxx.find_library('steam_api64', dirs: 'C:/dev/steam/bin/win64', required: true, static: false),
    ]

elif target_machine.system() == 'linux'

    bleak_internal_includes = include_directories('/usr/local/include/SDL2', '/usr/local/include')

    std_deps = [
        cxx.find_library('gcc_eh', dirs: '/usr/lib/gcc/x86_64-pc-linux-gnu/15.1.1', required: true, static: true),
        cxx.find_library('stdc++', dirs: '/usr/lib', required: true, static: true),
        cxx.find_library('pthread', dirs: '/usr/lib', required: true, static: true),
        cxx.find_library('steam_api', dirs: '/usr/local/lib', required: true, static: false),
    ]

endif

sdl_deps = [
	dependency('SDL2', required: true, static: true),
	dependency('SDL2_image', required: true, static: true),
	dependency('SDL2_mixer', required: true, static: true),
	dependency('SDL2_net', required: true, static: true),
]

subdir('inc')

bleak_dep = declare_dependency(
	include_directories: [bleak_public_includes, bleak_internal_includes],
	dependencies: [std_deps, sdl_deps],
)
