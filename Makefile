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
##  Copyright (C) 2021, Is This On? Holdings
##  
##  Harry Moulton <me@h3adsh0tzz.com>
##
##===----------------------------------------------------------------------===//

## Makefile for The Libhelper Project library and it's tests ##

# Libhelper output
LIBHELPER_OUTPUT_DYNAMIC	= libhelper.1.dylib
LIBHELPER_OUTPUT_STATIC		= libhelper.a

# RELEASE or DEVELOPMENT
ifndef ($(BUILDTYPE))
BUILDTYPE	= DEVELOPMENT
endif

# Project directory structure
SRC_DIR						= src
OBJ_DIR						= .obj
TOOLS_DIR					= tools
TESTS_DIR					= tests
BUILD_DIR					= build

# Source code
rwildcard 	= $(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))

SRC			= $(call rwildcard,$(SRC_DIR),*.c)
OBJ			= $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# Compiler Configuration
CC			= @clang
AR			= @ar

CFLAGS		= -Wall -Wextra -g -Iinclude -std=c11

# Makefile Rules
library:	$(BUILD_DIR)/libhelper
version:	$(BUILD_DIR)/libhelper-version
tests:		$(BUILD_DIR)/libhelper-core-test
#toolset:	$(BUILD_DIR)/macho-toolset

all:		library version tests #toolset

###############################################################################

# Add -O3 and lto for Release builds

.PHONY: libhelper

$(BUILD_DIR)/libhelper: $(OBJ)
	@mkdir -p "$(@D)"
	$(info [ LIB ] Building libhelper)
	$(CC) -shared -o $(BUILD_DIR)/libhelper.1.dylib $(OBJ)
	$(AR) -rv $(BUILD_DIR)/libhelper.a $(OBJ)

	cd build && dsymutil libhelper.1.dylib

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p "$(@D)"
	$(info [ CC ] $<)
	$(CC) $(CFLAGS) -c $< -o $@

install:
	$(info [ INFO ] Makefile option "install" not implemented)

clean:
	$(info [ CLEAN ] Cleaning build and object directories)
	rm -rf $(OBJ_DIR) $(BUILD_DIR) || true

###############################################################################

.PHONY: libhelper-version

LIBHELPER_VERSION_SRC		= $(TOOLS_DIR)/libhelper_version.c

$(BUILD_DIR)/libhelper-version:
	@mkdir -p "$(@D)"
	$(info [ TOOL ] Building libhelper-version)
	$(info [ CC ] $(LIBHELPER_VERSION_SRC))
	$(CC) $(CFLAGS) $(LIBHELPER_VERSION_SRC) -o $(BUILD_DIR)/libhelper-version build/libhelper.a -I$(SRC_DIR)

###############################################################################

.PHONY: tests

$(BUILD_DIR)/libhelper-core-test:
	@mkdir -p "$(@D)"
	$(info [ TEST ] Building libhelper-core-test)
	$(CC) $(CFLAGS) tests/libhelper-core-test.c -o $(BUILD_DIR)/libhelper-core-test build/libhelper.a