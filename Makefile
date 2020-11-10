
# Libhelper version
LIBHELPER_VERSION			= 1.1.0
LIBHELPER_VERSION_LONG		= libhelper-1200.643.21
LIBHELPER_VERSION_TYPE		= DEV						# DEV, BETA, RELEASE

LIBHELPER_OUTPUT			= libhelper.1.dylib

# File structure
SRC_DIR			= src
OBJ_DIR			= .obj
BUILD_DIR		= build

# Source code
rwildcard 	= $(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))

SRC			= $(call rwildcard,$(SRC_DIR),*.c)
OBJ			= $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# Compiler
CC			= clang

CFLAGS		= -Wall -O2 -Iinclude
CFLAGS		+= -DLIBHELPER_VERSION='"$(LIBHELPER_VERSION)"' -DLIBHELPER_VERSION_LONG='"$(LIBHELPER_VERSION_LONG)"' -DLIBHELPER_VERSION_TYPE='"$(LIBHELPER_VERSION_TYPE)"'


# Make rules

.PHONY: libhelper

all: $(BUILD_DIR)/libhelper.1.dylib

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p "$(@D)"
	$(warning [ CC ] $<)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/libhelper.1.dylib: $(OBJ)
	@mkdir -p "$(@D)"
	$(CC) -shared -o $(BUILD_DIR)/libhelper.1.dylib $(OBJ)

clean:
	rm -rf $(OBJ_DIR) $(BUILD_DIR) || true