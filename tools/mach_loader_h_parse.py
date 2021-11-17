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

# This script just compares which structs are present in the XNU loader.h compared
# to the libhelper one so i can get a quick list of whats implemented.

class bcolors:
    HEADER = '\033[95m'
    OKBLUE = '\033[94m'
    OKCYAN = '\033[96m'
    OKGREEN = '\033[92m'
    WARNING = '\033[93m'
    FAIL = '\033[91m'
    ENDC = '\033[0m'
    BOLD = '\033[1m'
    UNDERLINE = '\033[4m'

def parse_header_file(fn):
    struct_list = []
    data = None

    with open(fn, 'r') as f:
        data = f.read()

    data = data.split("\n")
    for l in data:
        if "struct" in l and "{" in l:
            name = l.split(" ")[1]
            struct_list.append(name)

    return struct_list

def get_implemented_structs():
    fn = "../include/macho/loader.h"
    return parse_header_file(fn)

def get_loader_h_structs():
    fn = "loader-test.h"
    return parse_header_file(fn)


if __name__ == "__main__":
    print("** Unimplemented structs in libhelper **")

    # Gather list of implemented structs
    implemented = get_implemented_structs ()

    # Gather list of all structs
    all_structs = get_loader_h_structs ()

    yes = []
    no = []

    for struct in all_structs:
        if struct not in implemented:
            no.append(struct)
        else:
            yes.append(struct)

    for a in yes:
        print(f"{bcolors.OKGREEN}[IMPLEMENTED]: {a}{bcolors.ENDC}")

    print("\n")

    for a in no:
        print(f"{bcolors.FAIL}[NOT IMPLEMENTED]: {a}{bcolors.ENDC}")







