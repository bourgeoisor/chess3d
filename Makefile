CC=gcc
CXX=g++
RM=rm -f
CPPFLAGS=-g $(shell root-config --cflags)
LDFLAGS=-g $(shell root-config --ldflags)
LDLIBS=$(shell root-config --libs) -lglut -lGL -lGLU

SRCS=vao.cpp rules.cpp game.cpp main.cpp 
OBJS=$(subst .cc,.o,$(SRCS))

all: chess

chess: $(OBJS)
	$(CXX) $(LDFLAGS) -o chess $(OBJS) $(LDLIBS) 

depend: .depend

.depend: $(SRCS)
	$(RM) ./.depend
	$(CXX) $(CPPFLAGS) -MM $^>>./.depend;

clean:
	$(RM) $(OBJS)

distclean: clean
	$(RM) *~ .depend

include .depend