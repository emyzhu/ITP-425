#include "SrcMain.h"
#include <iostream>
#include <random>
#include "TSP.h"
#include <fstream>
#include <algorithm>
#include <sstream>
#include <experimental/iterator>

void ProcessCommandArgs(int argc, const char* argv[])
{
    Population pop;
    
	// Save these parameters into variables.
    std::string inputfile = argv[1];
    int popsize = std::stoi(argv[2]);
    int generations = std::stoi(argv[3]);
    double mutationchance = std::stod(argv[4]) / 100;
    int seed = std::stoi(argv[5]);
    
    //  construct a std::mt19937 generator and pass in the seed integer as a parameter to the constructor
    std::mt19937 randGen(seed);
    std::uniform_int_distribution<int> myDist(0, 10);
    
    //make a function that returns a std::vector of locations. This function needs to read in the locations from the input file.
    
    std::vector<Location> location_info = parse_location_info(inputfile);
    
    int num_locations = location_info.size();
    
    pop.generate_pop(num_locations, randGen, popsize);
    
    std::vector<double> probabilities;
    std::vector<std::pair<int, double>> fit;
    std::vector <std::pair<int, int>> parents;
    
    std::ofstream file ("log.txt");
    if (file.is_open()) {
        
        for (int i = 0; i <= generations; i++) {
            fit = pop.calculate_fitness(location_info);
            probabilities = generate_probabilities(fit);
            parents = form_parents(probabilities, randGen, fit);
            if (i == generations) {
                output_final_gen(file, pop.mMembers, fit, location_info, i);
            }
            else {
                output_locations(file, pop.mMembers, fit, parents, i);
            }
            pop = crossover(pop, parents, num_locations, randGen,  mutationchance);
        }
        
        file.close();
    }
        
    
//    for (int i = 0; i <= generations; i++) {
//        fit = pop.calculate_fitness(location_info);
//        probabilities = generate_probabilities(fit);
//        parents = form_parents(probabilities, randGen, fit);
//        if (i == generations) {
//            output_final_gen(pop.mMembers, fit, location_info, i);
//        }
//        else {
//            output_locations(pop.mMembers, fit, parents, i);
//        }
//        pop = crossover(pop, parents, num_locations, randGen,  mutationchance);
//    }
}

std::vector<Location> parse_location_info(const std::string& inputfile)
{
    std::vector<Location> location_info;
    std::string line;
    std::ifstream file (inputfile);
    
    if (file.is_open()) {
        while (getline (file, line)) {
            std::string location_string;
            std::stringstream ss(line);
            
            Location l;
            
            std::string latitude_sting;
            std::string longitude_string;
            
            std::getline(ss, l.mName, ',');
            std::getline(ss, latitude_sting, ',');
            std::getline(ss, longitude_string, ',');
            
            l.mLatitude = 0.0174533 * std::stod(latitude_sting);
            l.mLongitude = 0.0174533 * std::stod(longitude_string);
            
            location_info.push_back(l);
        }
        file.close();
    }
    return location_info;
}

void output_locations(std::ofstream& file, std::vector<std::vector<int>> locations, std::vector<std::pair<int, double>> fit, std::vector <std::pair<int, int>> parents, int generation_number) {
    if (file.is_open()) {
        if (generation_number == 0) {
            file << "INITIAL POPULATION:" <<std::endl;
        }
        else {
            file << "GENERATION: " << generation_number <<std::endl;
        }
        for(const auto& i : locations) {
            std::copy(i.cbegin(), i.cend(),
                   std::experimental::make_ostream_joiner(file, ","));
             file << '\n';
        }
        file << "FITNESS:" <<std::endl;
        for(int i = 0; i < fit.size(); i++)
        {
             file << fit[i].first << ":" << fit[i].second << std::endl;
        }
        file << "SELECTED PAIRS:" <<std::endl;
        for(int i = 0; i < fit.size(); i++)
        {
             file << "(" << parents[i].first << "," << parents[i].second << ")" << std::endl;
        }
    }
}

void output_final_gen(std::ofstream& file, std::vector<std::vector<int>> locations, std::vector<std::pair<int, double>> fit, std::vector<Location> location_info, int generation_number) {
    if (file.is_open()) {
        file << "GENERATION: " << generation_number << std::endl;
        for(const auto& i : locations) {
            std::copy(i.cbegin(), i.cend(),
                   std::experimental::make_ostream_joiner(file, ","));
             file << '\n';
        }
        file << "FITNESS:" <<std::endl;
        for(int i = 0; i < fit.size(); i++)
        {
             file << fit[i].first << ":" << fit[i].second << std::endl;
        }
        file << "SOLUTION:" <<std::endl;
        std::sort(fit.begin(), fit.end(), [] (auto &less, auto &more) {
            return less.second < more.second;
        });
        for(int i = 0; i < locations[fit[0].first].size(); i++) {
            std::vector<int> best_path = locations[fit[0].first];
            int location_info_index = best_path[i];
            file << location_info[location_info_index].mName << std::endl;
        }

        file << location_info[0].mName << std::endl;
        file << "DISTANCE: " << fit[0].second << " miles" << std::endl;
        file.close();
    }
}
