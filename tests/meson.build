sources = ['main.c']
deps = [glib_dep]

libhelper_tests = executable('libhelper-tests', sources, dependencies: deps, link_with: libhelper, include_directories : incdir)
checkra1n_example = executable('checkra1n-tests', sources: ['checkra1n_extract_demo.c'], dependencies: deps, link_with: libhelper, include_directories : incdir)

hslist_tests = executable ('hlibc_hslist_test', sources: ['hslist_tests.c'], dependencies: deps, link_with: libhelper, include_directories : incdir)

#header_test = executable ('machotest-header', sources: ['machotest-header.c'], dependencies: deps, link_with: libhelper, include_directories : incdir)

macho_test = executable ('machotest', sources: ['macho.c'])
