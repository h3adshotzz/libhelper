sources = ['main.c']
deps = [glib_dep]

libhelper_tests = executable('libhelper-tests', sources, dependencies: deps, link_with: libhelper, include_directories : incdir)
#header_test = executable ('machotest-header', sources: ['machotest-header.c'], dependencies: deps, link_with: libhelper, include_directories : incdir)

macho_test = executable ('machotest', sources: ['macho.c'])
