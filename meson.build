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

sources = [
    'src/program.cpp'
]

executable(
    'bleakdepth',
    sources,

    dependencies: [sdl_dependencies],

    override_options: [
        'cpp_std=c++26',
        'cpp_stdlib=libc++',
    ],

    install: true,
    install_dir: install_dir,
)

install_subdir('log', install_dir: install_dir)
install_subdir('res', install_dir: install_dir)
