#include "climb_chromosome.hh"
#include "cities.hh"

Chromosome * ClimbChromosome::clone() const
{
    return new ClimbChromosome(cities_ptr_);
}

double ClimbChromosome::get_fitness(Cities::permutation_t& ordering)
{
    return 1000.0 / (cities_ptr_->total_path_distance(ordering));
}

double ClimbChromosome::get_fitness()
{
    return get_fitness(order_); // basing it off of the other function.
}

// a mod n for -n < a
int modulus(int a, int n)
{
    return (a + n) % n;
}

void swap_nearby(Cities::permutation_t& ord, int p) // helper function for swaping the near ones
{
    std::swap(ord[modulus(p - 1, ord.size())], ord[modulus(p, ord.size())]);
}

void ClimbChromosome::mutate()
{

    if(order_.size() < 2){
        return;
    }

    const double o_fit = this->get_fitness(); // original
    
    int p = generator_() % order_.size();

    Cities::permutation_t o1(order_);
    swap_nearby(o1, p);

    Cities::permutation_t o2(order_);
    swap_nearby(o2, p + 1);

    const double o1_fit = this->get_fitness(o1);    // children to test
    const double o2_fit = this->get_fitness(o2);

    if(!(o_fit >= o1_fit && o_fit >= o2_fit)){
        if(o1_fit > o2_fit){                        
            order_ = o1;
        } else{
            order_ = o2;
        }
    }
}
