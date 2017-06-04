#
# File: Makefile
# Description: 总控Makefile
# Author: fanxiaoyu
#

#顶层目录
TOPDIR := $(shell pwd)
SRCTREE := $(TOPDIR)


#编译过程中间文件目录，默认当前目录
BUILDDIR := $(TOPDIR)/build
ifneq (,$(O))
BUILDDIR := $(shell \
  [ -d $(O) ] || mkdir -p $(O); \
  if [ $$? -eq 0 ]; then \
    cd $(O) && pwd; \
  else \
    echo ERROR; \
  fi)
ifeq ($(BUILDDIR),ERROR)
$(error can\'t access path: $(O))
endif
else
temp := $(shell \
  [ -d $(BUILDDIR) ] || mkdir -p $(BUILDDIR);\
    if [ $$? -eq 0 ]; then \
      echo "OK"; \
    else \
      echo "ERROR"; \
    fi)
ifeq ($(temp),"ERROR")
$(error can\'t create build path: $(BUILDDIR))
endif
endif
#$(error build dir: $(BUILDDIR))

export TOPDIR SRCTREE BUILDDIR


#编译工具
CC := gcc
CXX := g++
LD := ld
AR := ar

CPPFLAGS := -I$(TOPDIR)/include -DDEBUG
CFLAGS :=
CXXFLAGS :=
LDFLAGS := -L$(BUILDDIR)
ARFLAGS := crv
export CC CXX LD AR CPPFLAGS CFLAGS CXXFLAGS LDFLAGS ARFLAGS


subdirs := base		\
	utils

.PHONY: all $(subdirs) library binary clean

all : library binary

$(subdirs):
	$(MAKE) -C $(@)

#删除库文件路径临时文件
$(shell $(RM) /tmp/ar.tmp)
library: $(subdirs)
	$(AR) $(ARFLAGS) $(BUILDDIR)/libbinder_like.a $(shell cat /tmp/ar.tmp)

binary: $(addsuffix _binary,$(subdirs))
$(addsuffix _binary,$(subdirs)):
	$(MAKE) -C $(patsubst %_binary,%,$@) binary

clean: $(addsuffix _clean,$(subdirs))
ifneq ($(BUILDDIR),$(TOPDIR))
	$(RM) -rf $(BUILDDIR)
endif
$(addsuffix _clean,$(subdirs)):
	$(MAKE) -C $(patsubst %_clean,%,$@) clean
