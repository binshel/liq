

all:
	g++ -g --share -fPIC -lprotobuf -o bin/libbase.so ./dsf/base.cc
	g++ -g --share -fPIC -lprotobuf -o bin/test.pb.so test.pb.cc
	g++ -g --share -fPIC -lprotobuf -o bin/test.so test.cc
	g++ -g --share -fPIC -lprotobuf -o bin/test.stub.so test.stub.cc
	g++ -g --share -fPIC -lprotobuf -o bin/test.skeleton.so test.skeleton.cc
	g++ -g -ldl -L./bin -lbase -o bin/test main.cc
