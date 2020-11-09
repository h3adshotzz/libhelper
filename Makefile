
# Libhelper version tags
LIBHELPER_VERSION			:= 1.1.0
LIBHELPER_VERSION_LONG		:= libhelper-1200.643.21

#
ROOT			:= $(shell pwd)
SRC				:= $(ROOT)/src
OBJ				:= $(ROOT)/.obj
INC				:= $(ROOT)/include
TOOLS			:= $(ROOT)/tools
TESTS			:= $(ROOT)/tests
BUILD			:= $(ROOT)/build

# Compiler
CC		:= clang
LD		:= ld

# General Options
LIBHELPER_CC_FLAGS		?= -DLIBHELPER_VERSION='"$(LIBHELPER_VERSION)"' -DLIBHELPER_VERSION_LONG='"$(LIBHELPER_VERSION_LONG)"' -I$(INC) $(LIBHELPER_LD_FLAGS)

# Source code groups
LIBHELPER_CORE			:= $(wildcat $(SRC)/*.c)
LIBHELPER_MACHO			:= $(wildcat $(SRC)/macho/*.c)
LIBHELPER_LZSS			:= $(wildcat $(SRC)/lzss/*.c)
LIBHELPER_LZFSE			:= $(wildcat $(SRC)/lzfse/*.c)
LIBHELPER_IMG4			:= $(wildcat $(SRC)/img4/*.c)
LIBHELPER_DYLD			:= $(wildcat $(SRC)/dyld/*.c)

.PHONY: all clean
	
all: $(BUILD)/libhelper.1.dylib | $(BUILD)
	
$(OBJ)/%.o: $(LIBHELPER_CORE)/%.c
	@mkdir -p "$(@D)"
	$(CC) $(CFLAGS) -c $< -o $@
	
$(OBJ)/%.o: $(LIBHELPER_MACHO)/%.c
	@mkdir -p "$(@D)"
	$(CC) $(CFLAGS) -c $< -o $@
	
$(OBJ)/%.o: $(LIBHELPER_LZSS)/%.c
	@mkdir -p "$(@D)"
	$(CC) $(CFLAGS) -c $< -o $@
	
$(OBJ)/%.o: $(LIBHELPER_LZFSE)/%.c
	@mkdir -p "$(@D)"
	$(CC) $(CFLAGS) -c $< -o $@
	
$(OBJ)/%.o: $(LIBHELPER_IMG4)/%.c
	@mkdir -p "$(@D)"
	$(CC) $(CFLAGS) -c $< -o $@
	
$(OBJ)/%.o: $(LIBHELPER_DYLD)/%.c
	@mkdir -p "$(@D)"
	$(CC) $(CFLAGS) -c $< -o $@
	
########################################################

$(BUILD)/libhelper.1.dylib: $(OBJ)
	$(CC) 
	
	
	
	
$(BUILD)/libhelper.1.dylib: $(LIBHELPER_CORE) $(LIBHELPER_MACHO) $(LIBHELPER_LZSS) $(LIBHELPER_LZFSE) $(LIBHELPER_IMG4) $(LIBHELPER_DYLD) | $(BUILD)
	$(CC) -o $@ $(LIBHELPER_CC_FLAGS) $(LIBHELPER_CORE) $(LIBHELPER_MACHO) $(LIBHELPER_LZSS) $(LIBHELPER_LZFSE) $(LIBHELPER_IMG4) $(LIBHELPER_DYLD)
	
$(BUILD):
	mkdir -p %@
	
clean:
	rm -rf $(BUILD)
	
	
	
	
	
	
	
	
	
	
	
	