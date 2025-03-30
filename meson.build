project(
    'libbleak',
    'cpp',
    version: '0.0.1',
)

cpu_family = target_machine.cpu_family()

if cpu_family == 'x86'
    install_dir = 'C:\\Program Files (x86)\\Necrowarp'
elif cpu_family == 'x86_64'
    install_dir = 'C:\\Program Files\\Necrowarp'
endif

build_type = get_option('buildtype')

if build_type == 'debug'
    subsystem = 'console'
    args = ['-DBLEAK_DEBUG']
else
    subsystem = 'windows'
    args = ['-DBLEAK_RELEASE', '-DNDEBUG']
endif

cxx = meson.get_compiler('cpp')



sdl_dependencies = [
    dependency('sdl2', required: true, static: true),
    dependency('sdl2_image', required: true, static: true),
    dependency('sdl2_mixer', required: true, static: true),
    dependency('sdl2_net', required: true, static: true),
    dependency('sdl2_ttf', required: true, static: true),

]

sources = [
    'src/program.cpp'
]

includes = include_directories(
	[
		'inc',
		'src'
	]
)

assert(sources.length() == 1, 'project is designed for a single source executable')

executable(
    'necrowarp',
    sources,

    dependencies: [
        sdl_dependencies,
        cxx.find_library('gcc', required: true, static: true),
        cxx.find_library('winpthread', required: true, static: true)
    ],

    include_directories: includes,

    override_options: [
        'cpp_std=c++26',
        'cpp_stdlib=libstdc++',
    ],

    c_args: args,
    cpp_args: args,

    install: true,
    install_dir: install_dir,

    win_subsystem: subsystem,
)

install_subdir('log', install_dir: install_dir)
install_subdir('res', install_dir: install_dir)

install_data('log\\message.log', install_dir: install_dir + '\\log')
install_data('log\\error.log', install_dir: install_dir + '\\log')
