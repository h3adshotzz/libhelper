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
//  Copyright (C) 2021, Is This On? Holdings Limited
//  
//  Harry Moulton <me@h3adsh0tzz.com>
//
//===----------------------------------------------------------------------===//

/* libhelper-version */

#include <stdio.h>
#include <libhelper.h>

int main ()
{
	printf ("Copyright (C) 2021, Is This On? Holdings Limited\n\n");
	printf ("The Libhelper Project (%s)\n", libhelper_get_version());
	printf ("  Build Time:\t\t" __TIMESTAMP__ "\n");
	printf ("  Default Target:\t%s-%s\n", libhelper_get_build_target(), libhelper_get_build_arch());
	printf ("  Libhelper:\t\t%s\n", libhelper_get_version_string());
	
	return 0;
}