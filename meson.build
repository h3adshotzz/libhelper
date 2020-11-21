project('libhelper', 'c', version: '1.1.0')

subdir('include')
subdir('src')

libhelper_dep = declare_dependency (include_directories: libhelper_inc,
                                              link_with: libhelper_lib)

subdir('tools')
