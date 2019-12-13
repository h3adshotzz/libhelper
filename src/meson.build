#
#   Build file for libhelper.
#
#   Copyright (C) 2019, @h3adsh0tzz
#   See LICENSE
#

#
#   Sources
#
mach_parser_sources = ['macho/macho.c', 'macho/macho-header.c', 'macho/macho-command.c', 'macho/macho-segment.c', 'macho/macho-symbol.c']
img4_sources = ['img4/sep.c']
sources = ['libhelper.c', 'file.c', 'strutils.c', 'hslist.c', mach_parser_sources, img4_sources]
incdir = include_directories('../include')

#
#   Dependencies
#
deps = [glib_dep]

#
#   dont quite know what im doing here. ignore for now.
#
libhelper_static    =  static_library('helper', sources, version : '1.0', dependencies: deps, include_directories : incdir)
libhelper           =  shared_library('helper', sources, version : '1.0', dependencies: deps, include_directories : incdir)