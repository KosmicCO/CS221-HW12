/*
 * Main file for Traveling-Salesperson solver.
 * Read a cities file in TSV format into a Cities object, then run
 * of several solver algorithms on it and output the best result.
 */

#include "cities.hh"
#include "deme.hh"
#include "tournament_deme.hh"

#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <numeric>
#include <thread>

using namespace std;

//////////////////////////////////////////////////////////////////////////////
// Check whether a specific ordering reduces the total path distance in cities
// from a previous best distance. If so, print out the iteration number and
// new distance, update the best distance, and return true.
// Returns false if no improvement was found.
bool
is_improved(const Cities& cities,
            const Cities::permutation_t& ordering,
            double& best_dist,
            uint64_t iter)
{
  const auto dist = cities.total_path_distance(ordering);
  if (dist < best_dist) {
    std::cout << iter << "\t" << dist << std::endl;
    best_dist = dist;
    return true;
  }
  return false;
}

//////////////////////////////////////////////////////////////////////////////
// exhaustive_search searches niter randomized operdinges on the given cities
// The best cities permutation is returned.
Cities::permutation_t
randomized_search(const Cities& cities, uint64_t niter)
{
  auto best_ordering = Cities::permutation_t(cities.size());
  auto best_dist = 1e100;

  for (uint64_t i = 0; i < niter; ++i) {
    auto ordering = random_permutation(cities.size());
    if (is_improved(cities, ordering, best_dist, i)) {
      best_ordering = ordering;
    }
  }

  return best_ordering;
}

//////////////////////////////////////////////////////////////////////////////
// exhaustive_search searches every single permutation of the cities.
// The best cities permutation is returned.
Cities::permutation_t
exhaustive_search(const Cities& cities)
{
  auto ordering = Cities::permutation_t(cities.size());
  std::iota(ordering.begin(), ordering.end(), 0);
  auto best_ordering = ordering;
  auto best_dist = 1e100;
  uint64_t i = 0;

  do {
    i++;
    if (is_improved(cities, ordering, best_dist, i)) {
      best_ordering = ordering;
    }
  } while (std::next_permutation(ordering.begin(), ordering.end()));

  return best_ordering;
}


//////////////////////////////////////////////////////////////////////////////
// ga_search uses a genetic algorithm to solve the traveling salesperson
// problem for a given list of cities.
// This function then creates a randomly generated population of permutations
// for traveling to those cities.
// The function also requires a population size and mutation rate.
// indicates how aggressively the population's individuals mutate.  The
// function then repeatedly evolves the population to generate increasingly
// better (i.e. shorter total distances) city permutations.
// The best cities permutation is returned.
Cities::permutation_t
ga_search(const Cities& cities,
          unsigned iters,
          unsigned pop_size,
          double mutation_rate)
{
  auto best_dist = 1e100;
  auto best_ordering = Cities::permutation_t(cities.size());

  TournamentDeme deme(&cities, pop_size, mutation_rate);

  // Evolve the population to make it fitter and keep track of
  // the shortest distance generated
  for (long i = 1; i <= iters/pop_size; ++i) 
  {
    deme.compute_next_generation();    // generate next generation
    // Find best individual in this population
    const auto ordering = deme.get_best()->get_ordering();
    if (is_improved(cities, ordering, best_dist, i * pop_size)) 
    {
      best_ordering = ordering;
    }

  }


return best_ordering;
}

//////////////////////////////////////////////////////////////////////////////
int main(int argc, char** argv)
{
    if (argc != 5) {
        std::cerr << "Required arguments: filename for cities, population size, mutation rate, and thread count.\n";
        return -1;
    }

    const auto cities = Cities(argv[1]);
    const auto pop_size = atoi(argv[2]);
    const auto mut_rate = atof(argv[3]);
    const auto num_thrd = atoi(argv[4]);
    constexpr unsigned NUM_ITER = 800000;
    assert(cities.size() > 0 && "Did you actually read the input file successfully?");


    std::vector<Cities::permutation_t> thread_results(num_thrd);
    std::vector<std::thread> threads(num_thrd);
    const auto num_iter_per = NUM_ITER / num_thrd;

    for(auto i = 0; i < num_thrd; i++){
        threads[i] = std::thread([=, &thread_results]() -> void {
            thread_results[i] = ga_search(cities, num_iter_per, pop_size, mut_rate);
        });
    }

    for(long unsigned int i = 0; i < threads.size(); i++){
        threads[i].join();
    }

//    const auto best_ordering = ga_search(cities, NUM_ITER, pop_size, mut_rate);

    const auto best_ordering = * std::min_element(thread_results.cbegin(), thread_results.cend(),
        [&cities](Cities::permutation_t a, Cities::permutation_t b) -> bool { return cities.total_path_distance(a) < cities.total_path_distance(b); });

    auto out = std::ofstream("shortest.tsv");
    if (!out.is_open()) {
        std::cerr << "Can't open output file to record shortest path!\n";
        return -2;
    }

    cout << cities.total_path_distance(best_ordering) << endl;
    out << cities.reorder(best_ordering);

    out.close();

    return 0;
}

