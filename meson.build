project(
    'libbleak',
    'cpp',
    version: '0.0.1',
)

install_dir = '@0@\\install\\@1@\\@2@'.format(
	meson.project_source_root(),
	target_machine.system(),
	target_machine.cpu_family()
)

build_type = get_option('buildtype')

if build_type == 'debug'
    subsystem = 'console'
    args = ['-DBLEAK_DEBUG']

	install_data('steam_appid.txt', install_dir: install_dir)
else
    subsystem = 'windows'
    args = ['-DBLEAK_RELEASE', '-DNDEBUG']
endif

cxx = meson.get_compiler('cpp')

std_dependencies = [
	cxx.find_library('gcc', required: false, static: false),
	cxx.find_library('gcc_s', required: false, static: false),
	cxx.find_library('stdc++', required: false, static: false),
	cxx.find_library('clang', required: false, static: false),
	cxx.find_library('c++', required: false, static: false),
	cxx.find_library('c++abi', required: false, static: false),
	cxx.find_library('winpthread', required: false, static: false),
	cxx.find_library('steam_api64', required: true, static: false)
]

sdl_dependencies = [
    dependency('sdl2', required: true, static: false),
    dependency('sdl2_image', required: true, static: false),
    dependency('sdl2_mixer', required: true, static: false),
    dependency('sdl2_net', required: true, static: false),
    dependency('sdl2_ttf', required: true, static: false),
]

sources = ['src/program.cpp']

assert(sources.length() == 1, 'project is designed for a single source executable')

includes = include_directories(['inc', 'src'])

dyn_libs = ['zlib1', 'steam_api64', 'SDL2_ttf', 'SDL2_net', 'SDL2_mixer', 'SDL2_image', 'SDL2', 'libzstd', 'libyuv', 'libwinpthread-1', 'libwebpdemux-2', 'libwebp-7', 'libunwind', 'libtiff-6', 'libSvtAv1Enc-3', 'libsharpyuv-0', 'librav1e', 'libpng16-16', 'libpcre2-8-0', 'libopusfile-0', 'libopus-0', 'libogg-0', 'libmpg123-0', 'libmodplug-1', 'liblzma-5', 'libLerc', 'liblcms2-2', 'libjxl_cms', 'libjxl', 'libjpeg-8', 'libjbig-0', 'libintl-8', 'libiconv-2', 'libhwy', 'libharfbuzz-0', 'libgraphite2', 'libglib-2.0-0', 'libfreetype-6', 'libdeflate', 'libdav1d-7', 'libc++', 'libbz2-1', 'libbrotlienc', 'libbrotlidec', 'libbrotlicommon', 'libavif-16', 'libaom']

dyn_lib_path = cxx.get_define('libpath')

dyn_lib_ext = cxx.get_define('libext')

dyn_files = []

foreach dyn_lib : dyn_libs
	dyn_files += dyn_lib_path + dyn_lib + dyn_lib_ext
endforeach

executable(
    'necrowarp',
    sources,

    dependencies: [std_dependencies, sdl_dependencies],

    include_directories: includes,

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

install_data(dyn_files, install_dir: install_dir)
