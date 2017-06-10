#
# File: rules.mk
# Description: Makefile通用规则文件
# Author: fanxiaoyu
#

#创建当前目录的build路径
curdir := $(shell pwd)
temp := $(subst $(TOPDIR)/,,$(curdir))

#$(error curdir: $(curdir); temp: $(temp))

build_path := $(addprefix $(BUILDDIR)/,$(temp))
result := $(shell \
	[ -d $(build_path) ] || mkdir -p $(build_path); \
	if [ $$? -eq 0 ]; then \
	    echo "OK"; \
	else \
	    echo "ERROR"; \
	fi)
ifeq ("ERROR",$(result))
$(error can\'t create build path: $(build_path))
endif
#$(error build path: $(build_path))


.PHONY: all $(subdirs) library binary clean

all: library $(subdirs)

#########1.subdirs定义了要递归的子目录##################

#########2.编译到静态库的源文件#########################
#lib_static_srcs定义了静态库源文件
lib_static_csrcs := $(filter %.c,$(lib_static_srcs))
ifeq (,$(lib_static_csrcs))
lib_static_cobjs :=
else
lib_static_cobjs := $(addprefix $(build_path)/,$(patsubst %.c,%.o,$(lib_static_csrcs)))
#lib_static_cdeps := $(addprefix $(build_path)/,$(patsubst %.c,%.d,$(lib_static_csrcs)))
endif

lib_static_cxxsrcs := $(filter %.cpp,$(lib_static_srcs))
ifeq (,$(lib_static_cxxsrcs))
lib_static_cxxobjs :=
else
lib_static_cxxobjs := $(addprefix $(build_path)/,$(patsubst %.cpp,%.o,$(lib_static_cxxsrcs)))
endif
#$(warning $(lib_static_cobjs); $(lib_static_cxxobjs))


#########3.编译到动态库的源文件########################
#lib_dynamic_srcs定义了动态库源文件


#########4.可执行文件#####################
#exec_targets定义了要编译的可执行文件
#exec_xxx_srcs定义了本目录下需要编译到xxx的源文件（xxx为可执行文件名字）
#exec_xxx_libs定义了编译exec_xxx_target需要的库
ifneq (,$(exec_targets))
$(warning exec_targets=$(exec_targets))
$(foreach target,$(exec_targets),$(shell \
	$(TOPDIR)/gen_exec_makefile.sh $(target) > $(build_path)/$(target).mk))
#temp := $(shell \
#	for f in $(exec_targets)  do\
#    		echo "$$f"; \
#    		source $(TOPDIR)/gen_exec_makefile.sh $$f > $(build_path)/$$f.mk; \
#  	done)
#    if [ $$? -ne 0 ]; then \
#	echo "ERROR"; \
#	exit 1; \
#    fi; \
$(warning temp==$(temp))
ifeq ("ERROR",$(temp))
$(error Can\'t generate execuable file Makefile)
endif
#包含脚本生成的可执行文件规则
temp := $(addprefix $(build_path)/,$(exec_targets))
temp := $(addsuffix .mk,$(temp))
include $(temp)
endif


###########通用规则#############################

#静态库规则
library : $(lib_static_cobjs) $(lib_static_cxxobjs)
	$(shell \
	  for f in $(lib_static_cobjs) $(lib_static_cxxobjs); do \
	    echo "$${f}" >> /tmp/ar.tmp; \
	  done)

ifeq (,$(lib_static_cobjs))
$(lib_static_cojbs):
else
#$(lib_static_cobjs): $(BUILDDIR)/%.o : $(BUILDDIR)/%.d
$(lib_static_cobjs): $(BUILDDIR)/%.o : $(TOPDIR)/%.c
	$(CC) -o $@ $(CPPFLAGS) $(CFLAGS) -c $<
$(lib_static_cdeps): $(BUILDDIR)/%.d : $(TOPDIR)/%.c
	$(CC) -MM $(CPPFLAGS) $< > $@

#include $(lib_static_cdeps)
endif

ifeq (,$(lib_static_cxxobjs))
$(lib_static_cxxobjs) :
else
#$(lib_static_cxxobjs): $(BUILDDIR)/%.o : $(BUILDDIR)/%.d
$(lib_static_cxxobjs) : $(BUILDDIR)/%.o : $(TOPDIR)/%.cpp
	$(CXX) -o $@ $(CPPFLAGS) $(CXXFLAGS) -c $<
$(lib_static_cxxdeps): $(BUILDDIR)/%.d : $(TOPDIR)/%.cpp
	$(CXX) -MM $(CPPFLAGS) $< > $@

#include $(lib_static_cxxdeps)
endif


#动态库规则


#子目录规则
$(subdirs):
	$(MAKE) -C $(@)


#可执行文件规则
binary: $(addsuffix _binary,$(subdirs))

$(addsuffix _binary,$(subdirs)):
	$(MAKE) -C $(patsubst %_binary,%,$@) binary

ifneq (,$(exec_targets))
binary: $(addprefix $(build_path)/,$(exec_targets))
endif
#$(warning binary: $(addprefix $(build_path)/,$(exec_targets)))


#清理规则
ifneq (,$(lib_static_cobjs)$(lib_static_cxxobjs))
clean:
	$(RM) $(lib_static_cobjs) $(lib_static_cxxobjs)
endif
clean: $(addsuffix _clean,$(subdirs))
$(addsuffix _clean,$(subdirs)):
	$(MAKE) -C $(patsubst %_clean,%,$@) clean
