project('mawu', 'c', default_options: ['warning_level=2'])
include_dir = include_directories('include')
src = ['src/main.c', 'src/lexer/tokens.c', 'src/lexer/scanner.c']
cc = meson.get_compiler('c')
m_dep = cc.find_library('m', required : true)
executable('mawu', src, include_directories: include_dir, dependencies: m_dep)
