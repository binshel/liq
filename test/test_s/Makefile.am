AUTOMAKE_OPTIONS = subdir-objects


AM_CXXFLAGS = --std=gnu++11 
AM_CXXFLAGS += -I../../include/  \
			   -I../../proto/cpp/proto

testdir = ${libdir}/liq/test
test_PROGRAMS = test_s.so

test_s_so_SOURCES = test_s.cc
test_s_so_CXXFLAGS = $(AM_CXXFLAGS) -fPIC 
test_s_so_LDFLAGS = -fPIC --shared
