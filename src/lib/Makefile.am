

lib_LTLIBRARIES = libliq.la
libliq_la_SOURCES = \
					io.cc  \
					liq.cc  \
					module.cc  \
					rpc.cc  \
					service.cc  \
					thread_pool.cc
libliq_la_CPPFLAGS =  -I$(top_srcdir)/include -I/home/ezfun/server/frame/src/common/autogen/ \
				  -I/home/ezfun/server/frame/src/comm/shm_channel
libliq_la_LDFLAGS = -ldl -lprotobuf -lprotoc -lboost_filesystem -L/home/ezfun/server/frame/src/comm/shm_channel/ -lshmchannel



