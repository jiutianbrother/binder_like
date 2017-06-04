#!/bin/bash

if [ $# -ne 1 ]; then
    echo "This script just need 1 args"
    exit 1
fi

echo -e "ifeq (,\$(exec_$1_srcs))"
echo -e "\$(error The execable file $1\'s input can\'t empty)"
#echo -e "else"
#echo -e "exec_$1_deps := \$(addsuffix .d,\$(basename \$(exec_$1_srcs)))"
#echo -e "exec_$1_deps := \$(addprefix \$(build_path)/,\$(exec_$1_deps))"
echo -e "endif\n"

echo -e "ifneq (,\$(exec_$1_libs))"
echo -e "LDFLAGS := \$(LDFLAGS) \$(addprefix -l,\$(exec_$1_libs))"
echo -e "endif\n"

#echo -e "ifneq (,\$(exec_$1_deps))"
#echo -e "\$(build_path)/$1 : \$(exec_$1_deps)"
#echo -e "\$(exec_$1_deps): \$(BUILDDIR)%.d: \$(TOPDIR)/%."
echo -e "\$(build_path)/$1 : \$(curdir)/\$(exec_$1_srcs)"
echo -e "\t\$(CXX) -o \$@ \$(CPPFLAGS) \$(CFLAGS) \$(CXXFLAGS) \$^ \$(LDFLAGS)"

#echo -e "clean:"
#echo -e "\t\$(RM) \$(build_path)/$1"
exit 0
