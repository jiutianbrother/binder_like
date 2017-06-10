CXXFLAGS += -std=c++11

exec_targets := LoggerTest \
		ThreadTest \
		MutexLockTest

exec_LoggerTest_srcs := LoggerTest.cpp
exec_LoggerTest_libs := binder_like cppunit

exec_ThreadTest_srcs := ThreadTest.cpp
exec_ThreadTest_libs := binder_like cppunit pthread

exec_MutexLockTest_srcs := MutexLockTest.cpp
exec_MutexLockTest_libs := binder_like cppunit pthread

include $(TOPDIR)/rules.mk
