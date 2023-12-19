#include "TSP.h"
#include <fstream>
#include <algorithm>
#include <cmath>
#include <iterator>
#include <numeric>
#include <iostream>
#include <sstream>
#include <math.h>

void Population::generate_pop(int number_of_locations, std::mt19937& engine, int popsize) {
    
    //create shuffled vector and add to locations
    
    mMembers.resize(popsize);
    
    std::generate(mMembers.begin(), mMembers.end(), [number_of_locations, &engine](){
        // Create an initial vector that contains the sequential values from 0…n – 1, where n is the number of locations in the location file.
        std::vector<int> location_order (number_of_locations);
        std::fill(location_order.begin(), location_order.end(), 0);
        std::iota (std::begin(location_order), std::end(location_order), 0);
        
        // Use std::shuffle to shuffle from begin() + 1 to end().
        std::shuffle(location_order.begin() + 1, location_order.end(), engine);
        
        return location_order;
    });
}

std::vector<std::pair<int, double>> Population::calculate_fitness(std::vector<Location>& location_info) {

    std::vector<std::pair<int, double>> fitness;
    
    //index counter that goes in vector pair
    int i = 0;
    
    std::transform(mMembers.begin(), mMembers.end(), std::back_inserter(fitness), [&location_info, &i](std::vector<int> m)
    {
        std::vector<double> all_distances;
        
        //account for the return segment.
        m.push_back(0);
        
        std::adjacent_difference(m.begin(), m.end(), std::back_inserter(all_distances), [&location_info](double next, double first)
        {
            double lon1 = location_info[first].mLongitude;
            double lat1 = location_info[first].mLatitude;
            double lon2 = location_info[next].mLongitude;
            double lat2 = location_info[next].mLatitude;

            double dlon = lon2 - lon1;
            double dlat = lat2 - lat1;
            double a = pow(sin(dlat / 2.0), 2.0) + cos(lat1) * cos(lat2) * pow(sin(dlon / 2.0), 2.0);
            double c = 2.0 * atan2(sqrt(a), sqrt(1.0 - a));
            return 3961.0 * c;
        });
        double distance_summed = std::accumulate(all_distances.begin(), all_distances.end(), 0.0, [] (const double& a, const double& b)
        {
            return a + b;
        });
        std::pair<int, double> pair = std::make_pair(i, distance_summed);
        ++i;
        return pair;
    });
    return fitness;
}

//First, you need to generate a probability vector based on the fitness results:
std::vector<double> generate_probabilities(std::vector<std::pair<int, double>> fit) {
    //Create a vector of probabilities that’s the size of the population.
    int popsize = fit.size();
    std::vector<double> probabilities(popsize);
    
    //Use std::sort to sort the fitness vector by fitness (greatest least) so the numbers go IN ASCENDING ORDER.
    
    //use lambda function to access second item in the pair and sort by that
    std::sort(fit.begin(), fit.end(), [] (auto &less, auto &more) {
        return less.second < more.second;
    });
    
    //Initialize each element in this vector to 1.0 / popsize
    std::generate(probabilities.begin(), probabilities.end(), [&popsize] () {
        return 1.0 / popsize;
    });
    
    //Multiply the probability for the two fittest individuals by 6.0.
    // the indices in this vector should correspond to the individual’s population index number, not their order in the sorted vector.... meaning.... access the first element of the sorted vector and take the first value in the pair and make that the index of the probability vector (wow i should NOT be a lawyer)
    probabilities[fit[0].first] *= 6.0;
    probabilities[fit[1].first] *= 6.0;
    
    //The remainder of the top half of the fit individuals (eg. from rank 2 to rank size / 2 – 1) should have their probability multiplied by 3.0.
    
    std::for_each(fit.begin() + 2, fit.begin() + (fit.size() / 2 ), [&probabilities] (std::pair<int, double>& p) {
        probabilities[p.first] *= 3;
        return p;
    });
    
    //Renormalize the probability vector to sum to 1.0 (use std::accumulate to compute the sum, then divide using std::transform).
    double sum = std::accumulate(probabilities.begin(),  probabilities.end(), 0.0, [] (const double& a, const double& b) {
        return a + b;
    });
    
    std::vector<double> probabilities_final;
    
    std::transform(probabilities.begin(),  probabilities.end(),std::back_inserter(probabilities_final), [sum] (const double& a) {
        return a / sum;
    });
    
    return probabilities_final;
}


std::vector <std::pair<int, int>> form_parents (std::vector<double> probabilities, std::mt19937& engine, std::vector<std::pair<int, double>> fit) {
    

    std::vector <std::pair<int, int>> parents(probabilities.size());
    
    std::generate(parents.begin(), parents.end(), [&engine, &probabilities] () {
        
        std::vector<int> pair(2);
        
        std::generate(pair.begin(), pair.end(), [&engine, &probabilities] () {
            std::uniform_real_distribution<double> dist(0.0, 1.0);
    
            
            double rand = dist(engine);
            int parent = -1;
            double running_sum = 0;
            
            for (int i = 0; i < probabilities.size(); i++) {
                running_sum += probabilities[i];
                if (parent == -1 && running_sum >= rand) {
                    parent = i;
                }
            }
            
            return parent;
        });
        return std::make_pair(pair[0], pair[1]);
    });
    
    
    return parents;
}

Population crossover(Population pop, std::vector <std::pair<int, int>> parents, int num_locations, std::mt19937& engine, double mutationchance) {
    
    Population new_pop;
    
    int parents_index = 0;
    
    std::generate_n(std::back_inserter(new_pop.mMembers), pop.mMembers.size(), [&parents, &num_locations, &engine, &parents_index, &pop, &mutationchance] () {
        std::vector<int> child(num_locations);
        
        int parentA = parents[parents_index].first;
        int parentB = parents[parents_index].second;
        
        
        std::uniform_int_distribution<int> dist(1, num_locations - 2);
        int crossover_index = dist(engine);
        std::uniform_int_distribution<int> dist2(0, 1);
        int coin_toss = dist2(engine);
        
        int copy_first;
        int copy_next;
        
        if (coin_toss == 1) {
            copy_first = parentA;
            copy_next = parentB;
        }
        else {
            copy_first = parentB;
            copy_next = parentA;
        }
        
        std::copy_n(pop.mMembers[copy_first].begin(), crossover_index + 1, child.begin());

        std::copy_if(pop.mMembers[copy_next].begin(), pop.mMembers[copy_next].end(), child.begin() + crossover_index + 1, [&child] (int i) {
            return std::find(child.begin(), child.end(), i) == child.end();
        });
        
        //If we mutate, pick two random indices (not including index 0), and swap the values
        
        std::uniform_real_distribution<double> dd(0.0, 1);
        double uniform_mutation_chance = dd(engine);
        if(uniform_mutation_chance <= mutationchance) {
            std::uniform_int_distribution<int> random_indices(1, child.size() - 1);
            
            int rand_i1 = random_indices(engine);
            int rand_i2 = random_indices(engine);
            
            std::swap(child[rand_i1], child[rand_i2]);
        }
        
        parents_index += 1;
        
        return child;
    });
    
    return new_pop;
}




