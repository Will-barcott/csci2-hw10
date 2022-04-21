CXX=g++-9
CXXFLAGS=-Wall -Wextra -pedantic -std=c++17 -O0 -g
LDFLAGS=$(CXXFLAGS)
OBJ=$(SRC:.cc=.o)

all:  test_chromo test_deme tsp

test_chromo: chromosome.o test_chromosome.o cities.o
	$(CXX) $(LDFLAGS) -o $@ $^

test_deme: chromosome.o cities.o deme.o test_deme.o
	$(CXX) $(LDFLAGS) -o $@ $^

tsp: tsp.o chromosome.o deme.o cities.o
	$(CXX) $(LDFLAGS) -o $@ $^

%.o: %.cc %.hh
	$(CXX) $(CXXFLAGS) $(OPTFLAGS) -c -o $@ $<

clean:
	rm -rf *.o tsp test_chromo test_deme

test:	all
		./test_chromo
		./test_deme