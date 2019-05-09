#include "tournament_deme.hh"
#include <cassert>
#include <algorithm>

// tournaments the vector into a smaller vector
void tournament(std::vector<Chromosome *>& pop)
{

    assert(!(pop.size() & 1)); // check that it is even

    std::vector<Chromosome *> winners;

    for(unsigned i = 0; i < pop.size(); i += 2)
    {
        if(pop[i]->get_fitness() > pop[i + 1]->get_fitness()){
            winners.push_back(pop[i]);
        }else{
            winners.push_back(pop[i + 1]);
        }
    }

    assert(pop.size() > winners.size());

    pop = winners;
}

void cleanup(std::vector<Chromosome *>& v){
    for(Chromosome * c : v){
        delete c;
    }
    v.clear();
}

Chromosome * TournamentDeme::select_parent()
{
    std::vector<Chromosome *> sampled;
    std::sample(pop_.cbegin(), pop_.cend(), std::back_inserter(sampled), to_tournament_, generator_);
    std::shuffle(sampled.begin(), sampled.end(), generator_);           // gets the elements to tournament together

    while(sampled.size() > 1){
        tournament(sampled);    // runs the tournament until there is only one element
    }

    return sampled.front();
}

// slightly cleaner than the previous since the mutate function automatically picks the fittest one.
void TournamentDeme::compute_next_generation()
{
    std::vector<Chromosome *> next_gen;

    for( unsigned i = 0; i < pop_.size() / 2; i++){
        std::uniform_real_distribution<double> fn(0.0, 1.0);
        const double random1 = fn(generator_);
        const double random2 = fn(generator_);

        auto parent1 = select_parent();
        auto parent2 = select_parent();

        if(random1 < mut_rate_){
            parent1->mutate();
        }
        if(random2 < mut_rate_){
            parent2->mutate();
        }

        auto children = parent1->recombine(parent2);
        next_gen.push_back(children.first);
        next_gen.push_back(children.second);
    }

    cleanup(pop_);
    pop_ = next_gen;
    next_gen.clear();
}
