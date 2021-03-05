CXX = g++

CPPFLAGS = -std=c++17 \
           -O3 \
           -pthread
           
RESOURCES= $(wildcard *.cpp)

all:
	$(CXX) $(CPPFLAGS) $(RESOURCES) -o runner

clean:
	rm runner
	all

