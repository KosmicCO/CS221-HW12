/*
 * Declarations for Deme class to evolve a genetic algorithm for the
 * travelling-salesperson problem.  A deme is a population of individuals.
 */

#include "chromosome.hh"
#include "climb_chromosome.hh"
#include "deme.hh"
#include <random>
#include <iostream>
#include <vector>
#include <algorithm>
#include <cassert>

using namespace std;
// Generate a Deme of the specified size with all-random chromosomes.
// Also receives a mutation rate in the range [0-1].
Deme::Deme(const Cities* cities_ptr, unsigned pop_size, double mut_rate)
{
  mut_rate_ = mut_rate;//mut_rate was also pre-defined
  //generator_.seed(time(NULL));

  for (unsigned index = 0; index < pop_size ; index++) // building a deme with a population of pop_size individuals
  {
  	auto Individual = new ClimbChromosome(cities_ptr);
  	pop_.push_back(Individual);
  }

}

// properly removes each chromosome from the vector and dealocates them
void cleanup(vector<Chromosome *> v){
    for(Chromosome * c : v){
        delete c;
    }
    v.clear();
}

// Clean up as necessary
Deme::~Deme()
{
    cleanup(pop_);
}

// Evolve a single generation of new chromosomes, as follows:
// We select pop_size/2 pairs of chromosomes (using the select() method below).
// Each chromosome in the pair can be randomly selected for mutation, with
// probability mut_rate, in which case it calls the chromosome mutate() method.
// Then, the pair is recombined once (using the recombine() method) to generate
// a new pair of chromosomes, which are stored in the Deme.
// After we've generated pop_size new chromosomes, we delete all the old ones.
void Deme::compute_next_generation()
{
 vector<Chromosome*> newGen;
  for (unsigned int index = 0;index < (pop_.size())/2; index++)
  {
    uniform_real_distribution<double> fn(0.0, 1.0);
    double random1 = fn(generator_); //Implicit Type Conversion, Generating a random number between 0 and 1
    double random2 = fn(generator_);

    auto FirstParent = select_parent();
    auto SecondParent = select_parent();


    auto fp = FirstParent;
    if (random1 < mut_rate_)
    {
        FirstParent->mutate();
        if(FirstParent->get_fitness()<fp->get_fitness())
           FirstParent = fp;
      
    }

    auto sp = SecondParent;
    if(random2 < mut_rate_)
    {
        SecondParent->mutate();
        if(SecondParent->get_fitness()<sp->get_fitness())
           SecondParent = sp;
      
    }
    
   
    auto Children = FirstParent->recombine(SecondParent); //Creating a new Generation of Children by 
    newGen.push_back(Children.first);// recombining the parents and adding them to the deme
    newGen.push_back(Children.second);
    

  }
    

  cleanup(pop_);
  pop_ = newGen;
  newGen.clear();
}

// Return a copy of the chromosome with the highest fitness.
const Chromosome* Deme::get_best() const
{
  auto CurrentBestIndividual = pop_[0];//Initially setting the first individual to be the most fit
  double CurrentBestFitScore = CurrentBestIndividual->get_fitness();
  for (unsigned int index = 0; index < pop_.size();index++)
  {
  	if(pop_[index]->get_fitness()>CurrentBestFitScore)
  	{
  		CurrentBestFitScore = pop_[index]->get_fitness();
  		CurrentBestIndividual = pop_[index];
  	}
  }

  return CurrentBestIndividual;
}

// Randomly select a chromosome in the population based on fitness and
// return a pointer to that chromosome.
//Implementing the Roulette Wheel Selection
Chromosome* Deme::select_parent()
{
    auto sum = std::accumulate(pop_.cbegin(), pop_.cend(), static_cast<double>(0.0),
        [](double d, Chromosome * c) { return d + c->get_fitness(); });

    vector<Chromosome*> newobj;
    double avg = sum/pop_.size();

    copy_if(pop_.begin(), pop_.end(), back_inserter(newobj),
        [=](Chromosome * chm){return chm->get_fitness() >= avg;});

    if(newobj.empty()){ // they are all the same -> premature convergeance
        return pop_[generator_() % pop_.size()];
    }

    assert(!newobj.empty());

    auto sum1 = 0.0;
    for (unsigned int index = 0;index < newobj.size(); index++)
    {
        sum1 = sum + newobj[index]->get_fitness();
    }

    uniform_real_distribution<double> fn(0.0, sum1); //Step 2 of Roulette Wheel
    auto RandomNumber = fn(generator_); 

    double PartialSum = 0;
    for (unsigned int index = 0; index < newobj.size(); index++)
    {
        PartialSum = PartialSum + newobj[index]->get_fitness();//Step3 of Roulette Wheel
    
  	if(PartialSum > RandomNumber)//Step 4 of Roulette Wheel
  	{
            return newobj[index];
  	}
    }

    

    return newobj.back();
    //Only reaches here if Random Number is in the last Partial Sum and hence we should return the last element.
  

}
