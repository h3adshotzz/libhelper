#
#   Build file for libhelper.
#
#   Copyright (C) 2019, @h3adsh0tzz
#   See LICENSE
#

#
#   Sources
#
sources = ['libhelper.c']

#
#   Dependencies
#
deps = [glib_dep]

#
#   dont quite know what im doing here. ignore for now.
#
libhelper_lib = static_library ('runtime', sources, install: false)
libhelper_shared = shared_library ('runtime', sources, version: '1.0', install: false)