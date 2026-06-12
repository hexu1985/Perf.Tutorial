RM = rm -f
CXX = g++
CXXFLAGS = -Wall -g -std=c++11 

PROGS =	example_before example_after

.PHONY: all
all: $(PROGS)
	@echo "build OK!"

clean:
	@$(RM) $(PROGS) *.o
	@echo "clean OK!"

%.o:%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@ 

example_before: example_before.o
	$(CXX) -o $@ $^

example_after: example_after.o
	$(CXX) -o $@ $^

