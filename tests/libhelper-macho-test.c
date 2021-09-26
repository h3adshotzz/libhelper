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
//  Copyright (C) 2021, Is This On? Holdings
//  
//  Harry Moulton <me@h3adsh0tzz.com>
//
//===----------------------------------------------------------------------===//

/* libhelper-macho: tests of macho api */

#include <stdio.h>

#include <libhelper.h>
#include <libhelper-macho.h>
#include <libhelper-logger.h>

///////////////////////////////////////////////////////////////////////////////

int main (int argc, char *argv[])
{
    macho_t *macho = (macho_t *) macho_load (argv[1]);
    if (!macho)
        errorf ("libhelper-macho-test: macho is NULL\n");

    return 0;
}