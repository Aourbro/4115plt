SRCS = $(wildcard *.cpp)

OBJS = $(SRCS:.cpp = .o)

CXXFLAGS = -Wall -g

CXX = g++

main : ${OBJS}
	${CXX} ${CXXFLAGS} -o $@ ${OBJS}

clean:
	-rm -f *.o main

%.o : %.cpp
	${CXX} -c $< ${CXXFLAGS}
