#   SCE CONFIDENTIAL                                      
#   PlayStation(R)3 Programmer Tool Runtime Library 370.001
#   Copyright (C) 2007 Sony Computer Entertainment Inc.   
#   All Rights Reserved. 

CELL_SDK ?= /usr/local/cell
CELL_SAMPLE_DIR ?= $(CELL_SDK)/samples
CELL_MK_DIR ?= $(CELL_SAMPLE_DIR)/mk
include $(CELL_MK_DIR)/sdk.makedef.mk

BUILD_TAG = tinyxml

PPU_OPTIMIZE_LV := -O2
PPU_INCDIRS		:= -Iinclude -I../include
PPU_CPPFLAGS	:= -D_DEBUG 
PPU_SRCS		:=  src/tinystr.cpp \
					src/tinyxml.cpp \
					src/tinyxmlerror.cpp \
					src/tinyxmlparser.cpp
		

PPU_LIB_TARGET	:= libtinyxml.a

include $(CELL_MK_DIR)/sdk.target.mk

