#===--------------------------- libhelper ----------------------------===//
#
#                         The Libhelper Project
#
#  This program is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or
#  (at your option) any later version.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with this program.  If not, see <http://www.gnu.org/licenses/>.
#
#
#  Copyright (C) 2019, Is This On?, @h3adsh0tzz
#  me@h3adsh0tzz.com.
#
#
#===------------------------------------------------------------------===//

# Libhelper version
LIBHELPER_VERSION			= 1.1.0
LIBHELPER_OUTPUT			= libhelper.1.dylib


# File structure
SRC_DIR			= src
OBJ_DIR			= .obj
TOOLS_DIR		= tools
BUILD_DIR		= build


# Source code
rwildcard 	= $(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))

SRC			= $(call rwildcard,$(SRC_DIR),*.c)
OBJ			= $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)


# Compiler
CC			= clang
AR			= ar

CFLAGS		= -Wall -Wextra -g -Iinclude -std=c11


# Make rules

library: $(BUILD_DIR)/libhelper.1.dylib
version: $(BUILD_DIR)/libhelper-version
tests: $(BUILD_DIR)/libhelper-general $(BUILD_DIR)/libhelper-macho
#toolset: $(BUILD_DIR)/macho-toolset

all: library version tests #toolset
	
############################################################

.PHONY: libhelper

$(BUILD_DIR)/libhelper.1.dylib: $(OBJ)
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
	cp build/libhelper.1.dylib /usr/local/lib/libhelper.1.dylib
	cp build/libhelper.a /usr/local/lib/libhelper.a
	
clean:
	rm -rf $(OBJ_DIR) $(BUILD_DIR) || true
	
############################################################

.PHONY: libhelper-version
	
LIBHELPER_VERSION_SRC	= $(TOOLS_DIR)/libhelper_version.c
	
$(BUILD_DIR)/libhelper-version:
	@mkdir -p "$(@D)"
	$(info [ TOOL ] Building libhelper-version)
	$(info [ CC ] $(LIBHELPER_VERSION_SRC))
	$(CC) $(CFLAGS) $(LIBHELPER_VERSION_SRC) -o $(BUILD_DIR)/libhelper-version build/libhelper.a
	
############################################################

.PHONY: tests
	
$(BUILD_DIR)/libhelper-general:
	@mkdir -p "$(@D)"
	$(info [ TEST ] Building libhelper-general)
	$(CC) $(CFLAGS) tests/libhelper-general.c -o $(BUILD_DIR)/test-libhelper-general build/libhelper.a
	dsymutil $(BUILD_DIR)/test-libhelper-general

$(BUILD_DIR)/libhelper-macho:
	@mkdir -p "$(@D)"
	$(info [ TEST ] Building libhelper-macho)
	$(CC) $(CFLAGS) tests/libhelper-macho.c -o $(BUILD_DIR)/test-libhelper-macho build/libhelper.a
	dsymutil $(BUILD_DIR)/test-libhelper-macho
	
	
#$(BUILD_DIR)/tests: $(TESTS_)#
#	@mkdir -p "$(@D)"
#	$(info [ TOOL ] Building $<)
#	$(info [ CC ] $(LIBHELPER_TESTING_SRC))
#	$(CC) $(LIBHELPER_TESTING_SRC) -o $(BUILD_DIR)/$<.o build/libhelper.a
	
############################################################





















