//===----------------------------------------------------------------------===//
//
//                         The Libhelper Project
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
//  Copyright (C) 2023, Is This On? Holdings Limited
//
//  Harry Moulton <me@h3adsh0tzz.com>
//
//===----------------------------------------------------------------------===//

#include <stdlib.h>
#include <getopt.h>


int main (int argc, char *argv[])
{
    if (argc < 2) {
        help ();
        return -1;
    }

    for (int i = 0; i < argc; i++) {
        if (!strcmp (argv[i], "-h")) {
            help ();
            return 0;
        } else if (!strcmp (argv[i], "-v")) {
            version ();
            return 0;
        }
    }

    char *filename = argv[1];
    file_t *file = file_load (filename);

    
}