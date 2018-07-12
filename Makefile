all: lzss simple

lzss: LZSS.C
	cc -g -Wall -O3 -o $@ $<

simple: simple.cpp
	c++ -g -Wall -O3 -o $@ $<
