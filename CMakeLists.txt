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

# Include a file from config/defaults.cmake which has default values.

# The default build type should be DEBUG. The build type can be set with
# -DCMAKE_BUILD_TYPE.
if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "DEBUG" CACHE STRING "Build type: RELEASE, DEBUG" FORCE)
endif()

############################## PROJECT #########################################

# Declare the project
project("The Libhelper Project" VERSION ${LIBHELPER_VERSION} LANGUAGES C)
add_library(libhelper SHARED)

set_property(TARGET libhelper PROPERTY C_STANDARD 11)

# OpenSSL configuration
if (LINUX)
    find_package(OpenSSL REQUIRED)
    target_link_libraries(libhelper OpenSSL::Crypto)
endif()

# Libhelper public headers.
target_include_directories(libhelper
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Setup versioning script
if (USE_VERSION_GENERATOR)
    include (config/version.cmake)
    add_dependencies(libhelper generate_version)
endif()

# Libhelper-core is actually the direct source files for libhelper, it's not
# built as a static library and then linked like the others.
add_subdirectory(src/libhelper-core)
add_subdirectory(src/libhelper-macho)
add_subdirectory(src/libhelper-compression)
add_subdirectory(src/libhelper-image4)
add_subdirectory(src/macho-toolset)

# Add other subdirectories
add_subdirectory(tools)
add_subdirectory(tests)

#include(config/install.cmake)
