AUTOMAKE_OPTIONS = subdir-objects

AM_CXXFLAGS = --std=gnu++11
AM_CXXFLAGS += -fPIC  \
			-I../../include/ \
			-I ../../proto/cpp/proto/
AM_LDFLAGS = -ljsoncpp

soldir = ${libdir}/liq/sol

bin_PROGRAMS = sol 
sol_PROGRAMS = sc_client.so
sol_SOURCES = command_line.cc sol.cc
sol_LDADD = ../lib/libliq.la

sc_client_so_SOURCES = sc_client.cc
sc_client_so_LDFLAGS = -fPIC --shared


