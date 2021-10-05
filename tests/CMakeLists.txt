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

####################### GENERIC LIBHELPER TESTS ############################

##  Libhelper Core Tests
##
add_executable(libhelper-core-test)
target_sources(libhelper-core-test PUBLIC libhelper-core-test.c)
target_include_directories(libhelper-core-test PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(libhelper-core-test libhelper)

##  Libhelper Mach-O Tests
##
add_executable(libhelper-macho-test)
target_sources(libhelper-macho-test PUBLIC libhelper-macho-test.c)
target_include_directories(libhelper-macho-test PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(libhelper-macho-test libhelper)

##  Libhelper FAT Tests
##
add_executable(libhelper-fat-test)
target_sources(libhelper-fat-test PUBLIC libhelper-fat-test.c)
target_include_directories(libhelper-fat-test PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(libhelper-fat-test libhelper)

