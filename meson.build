project('libhelper', 'c', version: '1.1.0')

# Build glib without warnings since there isn't much we can do about it anyway
# (asssuming you have new enough meson)
glib_proj = subproject('glib', default_options: 'warning_level=0')
glib_dep = glib_proj.get_variable('libglib_dep')

subdir('include')
subdir('src')

libhelper_dep = declare_dependency (include_directories: libhelper_inc,
                                              link_with: libhelper_lib,
                                           dependencies: [glib_dep])

subdir('tools')
