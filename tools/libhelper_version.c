//===--------------------------- libhelper ----------------------------===//
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
//
//  Copyright (C) 2019, Is This On?, @h3adsh0tzz
//  me@h3adsh0tzz.com.
//
//
//===------------------------------------------------------------------===//

#include <stdio.h>
#include "version.h"

int main ()
{
	printf ("%s Libhelper Version %s~%s (%s)\n", BUILD_TARGET_CAP, LIBHELPER_VERSION, LIBHELPER_VERSION_TYPE, LIBHELPER_VERSION_LONG);
	printf ("  Build Time:\t\t" __TIMESTAMP__ "\n");
	printf ("  Default Target:\t%s-%s\n", BUILD_TARGET, BUILD_ARCH);
	printf ("  Libhelper:\t\t%s\n", LIBHELPER_VERSION_LONG);
	
	return 0;
}