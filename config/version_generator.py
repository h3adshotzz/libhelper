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

from dataclasses import dataclass
from jinja2 import Environment, FileSystemLoader
import jinja2 as j2
import argparse

VERBOSE = False

@dataclass
class Version:
    master_version_data: list
    build_version_major: int
    build_version_minor: int
    build_version_revision: int
    source_version_major: int
    source_version_minor: int
    build_type: str

def log(msg):
    if VERBOSE:
        print("LOG: {}".format(msg))

def parse_master_version_file(file, build_type):
    log("Parsing file: {}".format(file))
    with open(file, "r") as f:
        data = f.readlines()
    build_version = data[0].strip().split(".")
    build_version_major, build_version_minor, build_version_revision = map(int, build_version)
    source_version_major, source_version_minor = map(int, data[1:3])

    version = Version(
        master_version_data=data,
        build_version_major=build_version_major,
        build_version_minor=build_version_minor,
        build_version_revision=build_version_revision,
        source_version_major=source_version_major,
        source_version_minor=source_version_minor,
        build_type=build_type
    )

    BUILD_VERSION = "{}.{}.{}".format(version.build_version_major, version.build_version_minor, version.build_version_revision)
    SOURCE_VERSION = f"libhelper-{build_version_major}{build_version_minor}{build_version_revision}.{source_version_major}.{source_version_minor}"
    log("BUILD_VERSION: {}".format(BUILD_VERSION))
    log("SOURCE_VERSION: {}".format(SOURCE_VERSION))

    return version

def write_master_version_file(fn, version):
    version.master_version_data[1] = f"{version.source_version_major}\n"
    version.master_version_data[2] = f"{version.source_version_minor}\n"
    with open(fn, "w") as f:
        f.writelines(version.master_version_data)

def update_source_version_number(version):
    if version.source_version_minor >= 99:
        version.source_version_major += 1
        version.source_version_minor = 0
    else:
        version.source_version_minor += 1
    SOURCE_VERSION = f"libhelper-{version.build_version_major}{version.build_version_minor}{version.build_version_revision}.{version.source_version_major}.{version.source_version_minor}"
    print("New source version: {}".format(SOURCE_VERSION))
    return version

def write_version_header(header_name, mv_name, template, version):
    env = j2.Environment(loader=FileSystemLoader(template))
    template = env.get_template("version.h.j2")

    build_version = f"{version.build_version_major}.{version.build_version_minor}.{version.build_version_revision}"
    build_version_condensed = "".join(build_version.split("."))
    source_version = f"libhelper-{build_version_condensed}.{version.source_version_major}.{version.source_version_minor}"

    header = template.render(
        LIBHELPER_BUILD_VERSION_MAJOR_VAL=version.build_version_major,
        LIBHELPER_BUILD_VERSION_MINOR_VAL=version.build_version_minor,
        LIBHELPER_BUILD_VERSION_REVISION_VAL=version.build_version_revision,
        LIBHELPER_SOURCE_VERSION_MAJOR_VAL=version.source_version_major,
        LIBHELPER_SOURCE_VERSION_MINOR_VAL=version.source_version_minor,
        LIBHELPER_BUILD_TYPE_VAL=version.build_type,
        LIBHELPER_BUILD_VERSION=build_version,
        LIBHELPER_SOURCE_VERSION=source_version,
    )
    with open(header_name, 'w') as f:
        f.write(header)
    print(f"path: {header_name}:\n{header}")

if __name__ == "__main__":
    parsed_version = None
    parser = argparse.ArgumentParser()
    parser.add_argument("-v", "--verbose", action="store_true", help="Print verbose messages")
    parser.add_argument("-m", "--master", action="store", help="MASTER file to read build info from")
    parser.add_argument("-t", "--template", action="store", help="directory containing templates")
    parser.add_argument("-o", "--outfile", action="store", help="Path to new header file")
    parser.add_argument("-b", "--build-type", action="store", help="Build type: DEBUG, BETA, RELEASE")
    args = parser.parse_args()

    if args.verbose:
        VERBOSE = True

    if args.master:
        parsed_version = parse_master_version_file(args.master, args.build_type)
        new_version = update_source_version_number(parsed_version)
        write_master_version_file(args.master, new_version)
        write_version_header(args.outfile, args.master, args.template, new_version)
