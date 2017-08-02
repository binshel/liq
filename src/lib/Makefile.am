

lib_LTLIBRARIES = libliq.la
libliq_la_SOURCES = \
					io.cc  \
					liq.cc  \
					module.cc  \
					rpc.cc  \
					service.cc  \
					thread_pool.cc
AM_CPPFLAGS =  -I$(top_srcdir)/include -I/home/ezfun/server/frame/src/common/autogen/ \
				  -I/home/ezfun/server/frame/src/comm/shm_channel
AM_LDFLAGS = -ldl -lprotobuf -lprotoc -lboost_filesystem -L/home/ezfun/server/frame/src/comm/shm_channel/ -lshmchannel



