#pragma once

#include "chromosome.hh"
#include "cities.hh"

class ClimbChromosome : public Chromosome {

  public:

    ClimbChromosome(const Cities * c)
        : Chromosome(c) {}

    virtual Chromosome * clone() const;

    virtual void mutate();

    virtual double get_fitness(); // overrides to make it depend on the below function.

    virtual double get_fitness(Cities::permutation_t& ordering); // finds the fitness for an arbitrary ordering.
};
