objs:
	g++ -std=c++11 -I./ -c binfile.cc
	g++ -std=c++11 -I./ -c ringbuf.cc
	g++ -std=c++11 -I./ -c compress.cc
all: objs
