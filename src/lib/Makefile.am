AUTOMAKE_OPTIONS = subdir-objects

AM_CPPFLAGS = -std=c++11
AM_CPPFLAGS +=  -I../../include/ \
				-I/home/ezfun/server/frame/src/common/autogen/ \
				-I/home/ezfun/server/frame/src/comm/shm_channel

AM_LDFLAGS = -ldl -lprotobuf -lprotoc -ljsoncpp -lbfd \
			 -L/home/ezfun/server/frame/src/comm/shm_channel/ -lshmchannel

lib_LTLIBRARIES = libliq.la                                                
libliq_la_SOURCES = \
					io.cc  \
					liq.cc  \
					logger.cc  \
					module.cc  \
					rpc.cc  \
					service.cc  \
					thread_pool.cc \
					timer.cc
