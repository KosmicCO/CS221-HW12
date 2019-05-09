#pragma once

#include "deme.hh"
#include <math.h>
#include <limits>

class TournamentDeme : public Deme {
    
    unsigned to_tournament_; // the number of elements to take to tournament together

  public:

    TournamentDeme(const Cities * cities_ptr, unsigned pop_size, double mut_rate)
        : Deme(cities_ptr, pop_size, mut_rate) {
        unsigned high_pow = 1;
        while(high_pow < (std::numeric_limits<unsigned int>::max() / 2) && high_pow * 2 <= pop_size){ // finds the largest power of two which is less than the number of elements
            high_pow *= 2;
        }
        to_tournament_ = std::min(high_pow, static_cast<unsigned int>(8)); // uses 256 elements as the max to tournament togehter
    }

    virtual Chromosome * select_parent();

    virtual void compute_next_generation();
};
