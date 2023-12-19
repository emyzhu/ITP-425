#pragma once
#include <string>
#include <vector>
#include <random>

struct Location
{
	std::string mName;
	double mLatitude;
	double mLongitude;
};

struct Population
{
	std::vector<std::vector<int>> mMembers;
    
    void generate_pop(int number_of_locations, std::mt19937& engine, int popsize);
    
    std::vector<std::pair<int, double>> calculate_fitness(std::vector<Location>& location_info);
};


std::vector <std::pair<int, int>> form_parents (std::vector<double> probabilities, std::mt19937& engine, std::vector<std::pair<int, double>> fit);


std::vector<double> generate_probabilities(std::vector<std::pair<int, double>> fit);

Population crossover(Population pop, std::vector <std::pair<int, int>> parents, int num_locations, std::mt19937& engine, double mutationchance);
