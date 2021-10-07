##===----------------------------------------------------------------------===//
##
##                         The Libhelper Project
##
##  This program is free software: you can redistribute it and/or modify
##  it under the terms of the GNU General Public License as published by
##  the Free Software Foundation, either version 3 of the License, or
##  (at your option) any later version.
##
##  This program is distributed in the hope that it will be useful,
##  but WITHOUT ANY WARRANTY; without even the implied warranty of
##  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
##  GNU General Public License for more details.
##
##  You should have received a copy of the GNU General Public License
##  along with this program.  If not, see <http://www.gnu.org/licenses/>.
##
##  Copyright (C) 2021, Is This On? Holdings Limited
##  
##  Harry Moulton <me@h3adsh0tzz.com>
##
##===----------------------------------------------------------------------===//

cmake_minimum_required(VERSION 3.15)

############################ CONFIGURATION #####################################

# Check this module should be built
#if (NOT LIBHELPER_)

target_include_directories(libhelper
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}../../
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_sources(libhelper
    PUBLIC
        load-command.c
        segment.c
        section.c

        load-commands/source_version_command.c
        load-commands/dylib_command.c

        macho-header.c
        macho64.c
        macho.c
)
