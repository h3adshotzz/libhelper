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

# This script generates a version number for libhelper by reading the current value in
# src/libhelper-core/version.h

#
#   Libhelper version spec:
#
#       - Major:        3
#       - Minor:        0
#       - Revision:     4
#       - Build Count:  23
#       - OS Type:      10      (macOS: 0xa, Linux: 0xb, iOS: 0xc)
#       - Arch:         1       (x86_64: 0x1, AArch64: 0x2)
#       - Build Type:   3       (Release: 0x1, Beta: 0x2, Debug: 0x3)
#
#   Full version format:
#       libhelper-MajorMinorRevision0.BuildCount.OS.Arch~Type
#
#   Example:
#       libhelper-3040.23.10.1~3
#

from dataclasses import dataclass
import platform

@dataclass
class VersionString:
    major: int
    minor: int
    revision: int
    build_count: int
    os_type: int
    arch: int
    build_type: int

def create_version_string_with_values(mj, mn, rev, bc, ostype, arch, btype):
    return "{:01}{:01}{:02}.{:02}.{:02}.{:01}~{:01}".format(mj, mn, rev, bc, ostype, arch, btype)

def parse_version_string(str):
    str = str.split(".")

    # check if pos 0 contains "libhelper-"
    if "libhelper-" in str[0]:
        str[0] = str[0].split("libhelper-")[1]

    print ("parse string: {0}".format(str))

    # pos 0, 1, 2 are fine.
    mmr = str[0]
    bc = str[1]
    ostype = str[2]

    # pos 3 should contain "Arch~BuildType", so split them.
    str = str[3].split("~")
    arch = str[0]
    btype = str[1]

    return VersionString(int(mmr[0]), int(mmr[1]), int("{0}{1}".format(mmr[2], mmr[3])), int(bc), int(ostype), int(arch), int(btype))



def parse_header_file (fn):

    data = None

    header_version = None
    header_version_type = None

    with open(fn, 'r') as f:
        data = f.read()

    data = data.split("\n")
    for l in data:
        if "#" in l:
            if "#define LIBHELPER_VERSION " in l:
                header_version = l.split('"')[1]
            elif "#define LIBHELPER_VERSION_TYPE " in l:
                header_version_type = l.split('"')[1]

    return [header_version, header_version_type]


if __name__ == "__main__":
    print("** Libhelper Version Generator **")

    # Get the version and version type from the header
    version_list = parse_header_file("../src/libhelper-core/version.h")

    # Now parse that version into a VersionString
    vers = parse_version_string (version_list[0])
    print("Current Libhelper Version: {0}.{1}.{2}".format(vers.major, vers.minor, vers.revision))

    # First of all we want to increment the build count.
    vers.build_count += 1

    # Set the build type
    if "RELEASE" in version_list[1]:
        vers.build_type = 1
    elif "BETA" in version_list[1]:
        vers.build_type = 2
    elif "DEBUG" in version_list[1]:
        vers.build_type = 3
    else:
        vers.build_type = 0

    # Set the OS type
    os = platform.system()
    if "Darwin" in os:
        vers.os_type = 10
    elif "Linux" in os:
        vers.os_type = 11
    else:
        vers.os_type = 13

    # Set the arch
    arch = platform.machine()
    if "arm64" in arch:
        vers.arch = 2
    elif "x86" in arch:
        vers.arch = 1
    else:
        vers.arch = 3


    new_version = "libhelper-{0}".format(create_version_string_with_values (vers.major, vers.minor, vers.revision, vers.build_count, vers.os_type, vers.arch, vers.build_type))

    # Now write the new version back.
    print("Writing version: " + str(new_version))











