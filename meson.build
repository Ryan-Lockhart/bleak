project('libbleak', 'cpp', version: '0.0.1', meson_version: '>=1.3.0')

install_dir = '@0@/lib/@1@/@2@'.format(
	meson.project_source_root(),
	target_machine.system(),
	target_machine.cpu_family()
)

build_type = get_option('buildtype')

if build_type == 'debug'
    libbleak_args = ['-DBLEAK_DEBUG']
else
    libbleak_args = ['-DBLEAK_RELEASE', '-DNDEBUG']
endif

libbleak_include = include_directories(['.'])
