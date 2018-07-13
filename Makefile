all: lzss simple huff

lzss: LZSS.C
	cc -g -Wall -O3 -o $@ $<

simple: simple.cpp
	c++ -g -Wall -O3 -o $@ $<

huff: huff.cpp
	c++ -std=c++11 -g -Wall -O3 -o $@ $<
