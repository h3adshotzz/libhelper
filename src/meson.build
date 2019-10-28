#
#   Build file for libhelper.
#
#   Copyright (C) 2019, @h3adsh0tzz
#   See LICENSE
#

#
#   Sources
#
sources = ['libhelper.c', 'file.c', 'macho/macho.c']
incdir = include_directories('../include')

#
#   Dependencies
#
deps = [glib_dep]

#
#   dont quite know what im doing here. ignore for now.
#
libhelper = shared_library('libhelper', sources, version : '1.0', dependencies: deps, include_directories : incdir)