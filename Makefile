CXX=g++-8
CXXFLAGS=-Wall -pthread -Wextra -pedantic -std=c++17 -O0 -g
LDFLAGS=$(CXXFLAGS)
OBJ=$(SRC:.cc=.o)

all:  tsp

test_climb_chromosome: chromosome.o test_climb_chromosome.o cities.o climb_chromosome.o
	$(CXX) $(LDFLAGS) -o $@ $^

test_chromosome: chromosome.o test_chromosome.o cities.o
	$(CXX) $(LDFLAGS) -o $@ $^

tsp: tsp.o tournament_deme.o chromosome.o deme.o cities.o climb_chromosome.o
	$(CXX) $(LDFLAGS) -o $@ $^

%.o: %.cc %.hh
	$(CXX) $(CXXFLAGS) $(OPTFLAGS) -c -o $@ $<

clean:
	rm -rf *.o tsp
