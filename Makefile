
# Libhelper version tags
LIBHELPER_VERSION			:= 1.1.0
LIBHELPER_VERSION_LONG		:= libhelper-1200.643.21
LIBHELPER_OUTPUT			:= libhelper.1.dylib

#
ROOT			:= $(shell pwd)
SRC				:= $(ROOT)/src
OBJDIR			:= $(ROOT)/.obj
INC				:= $(ROOT)/include
TOOLS			:= $(ROOT)/tools
TESTS			:= $(ROOT)/tests
BUILD			:= $(ROOT)/build

# Compiler
CC		:= clang
LD		:= ld

# General Options
CFLAGS				= -Wall -O2 -I$(INC)
CFLAGS				+= -DLIBHELPER_VERSION='"$(LIBHELPER_VERSION)"' -DLIBHELPER_VERSION_LONG='"$(LIBHELPER_VERSION_LONG)"'

# Source code groups
rwildcard 		= $(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))
LIBHELPER_SRC 	= $(call rwildcard,$(SRC),*.c)
OBJ 			= $(LIBHELPER_SRC:$(SRC)/%.c=$(OBJDIR)/%.o)



.PHONY: all clean
	
all: $(BUILD)/libhelper.1.dylib
	
$(BUILD)/libhelper.1.dylib: $(OBJ)
	@mkdir -p "$(@D)"
	$(CC) -shared -o $(LIBHELPER_OUTPUT) $(OBJ)
	
$(OBJDIR)/%.o: $(LIBHELPER_SRC)/%.c
	@mkdir -p "$(@D)"
	$(CC) $(CFLAGS) -c $< -o $@
	
########################################################
	 
clean:
	rm -rf $(BUILD) $(OBJDIR) || true
	
	
	
	
	
	
	
	
	
	
	
	