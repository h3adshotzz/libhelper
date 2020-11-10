
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

CFLAGS		= -Wall -O2 -Iinclude


# Make rules

library: $(BUILD_DIR)/libhelper.1.dylib
version: $(BUILD_DIR)/libhelper-version
toolset: $(BUILD_DIR)/macho-toolset

all: library version toolset
tools: version toolset

############################################################

.PHONY: libhelper

$(BUILD_DIR)/libhelper.1.dylib: $(OBJ)
	@mkdir -p "$(@D)"
	$(info [ LIB ] Building libhelper)
	$(CC) -shared -o $(BUILD_DIR)/libhelper.1.dylib $(OBJ)
	$(AR) -rv $(BUILD_DIR)/libhelper.a $(OBJ)
	

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
	$(info [ TOOL ] Building libhelper-version)
	$(info [ CC ] $(LIBHELPER_VERSION_SRC))
	$(CC) $(CFLAGS) $(LIBHELPER_VERSION_SRC) -o $(BUILD_DIR)/libhelper-version
	
############################################################

.PHONY: macho-toolset
	
MACHO_TOOLSET_SRC			= $(TOOLS_DIR)/macho_toolset.c
MACHO_TOOLSET_VERS			= 1.0.1
MACHO_TOOLSET_VERS_LONG		= 101.52.6

CFLAGS						+= -DTOOLSET_VERS_LONG='"$(MACHO_TOOLSET_VERS_LONG)"' -DTOOLSET_VERS='"$(MACHO_TOOLSET_VERS)"'
LFLAGS						+= $(BUILD_DIR)/libhelper.a

$(BUILD_DIR)/macho-toolset:
	$(info [ TOOL ] Building macho-toolset)
	$(info [ CC ] $(MACHO_TOOLSET_SRC))
	
	$(CC) $(CFLAGS) $(MACHO_TOOLSET_SRC) -DTOOL_SECT=1 -DTOOL_NAME='"macho-section"' $(LFLAGS) -o $(BUILD_DIR)/macho-section
	$(CC) $(CFLAGS) $(MACHO_TOOLSET_SRC) -DTOOL_SPLIT=1 -DTOOL_NAME='"macho-split"' $(LFLAGS) -o $(BUILD_DIR)/macho-split
	$(CC) $(CFLAGS) $(MACHO_TOOLSET_SRC) -DTOOL_DUMP=1 -DTOOL_NAME='"macho-dump"' $(LFLAGS) -o $(BUILD_DIR)/macho-dump
	
	
	
	
	
	
	
	
	
	
	
	