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
##  Copyright (C) 2023, Is This On? Holdings Limited
##  
##  Harry Moulton <me@h3adsh0tzz.com>
##
##===----------------------------------------------------------------------===//

cmake_minimum_required(VERSION 3.15)

set(VERSION_CMD "${CMAKE_CURRENT_SOURCE_DIR}/config/version_generator.py")
set(MASTER_VERSION "${CMAKE_CURRENT_SOURCE_DIR}/src/MasterVersion")
set(TEMPLATE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/config/")
set(OUTFILE "${CMAKE_CURRENT_SOURCE_DIR}/src/libhelper-core/version.h")
set(BUILD_TYPE "${CMAKE_BUILD_TYPE}")


add_custom_target(generate_version
                DEPENDS ${VERSION_CMD}
                        ${CMAKE_CURRENT_SOURCE_DIR}/src/*)

add_custom_command(TARGET generate_version
                PRE_BUILD
                COMMAND python3 ${VERSION_CMD} -m ${MASTER_VERSION} -o ${OUTFILE} -b ${BUILD_TYPE} -t ${TEMPLATE_DIR} -v
)

#add_custom_target(run_python_script
#    COMMAND python3 ${VERSION_CMD} -m ${MASTER_VERSION} -o ${OUTFILE} -b ${BUILD_TYPE} -t ${TEMPLATE_DIR} -v
#)