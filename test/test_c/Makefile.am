AUTOMAKE_OPTIONS = subdir-objects


AM_CXXFLAGS = --std=gnu++11 
AM_CXXFLAGS += -I../../include/  \
			   -I../..//proto/cpp/proto
testdir=${libdir}/liq/test

test_PROGRAMS = test_c.so

test_c_so_SOURCES = test_c.cc
test_c_so_CXXFLAGS = $(AM_CXXFLAGS) -fPIC 
test_c_so_LDFLAGS = -fPIC --shared
