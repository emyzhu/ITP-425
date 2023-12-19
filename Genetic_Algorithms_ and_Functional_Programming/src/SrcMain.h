#pragma once
#include <vector>
#include <string>
#include "TSP.h"



void ProcessCommandArgs(int argc, const char* argv[]);

std::vector<Location> parse_location_info(const std::string& inputfile);

std::vector<std::string> get_locations(const std::string& inputfile);

void output_locations(std::ofstream& file, std::vector<std::vector<int>> locations, std::vector<std::pair<int, double>> fit, std::vector <std::pair<int, int>> parents, int generation_number);

void output_final_gen(std::ofstream& file, std::vector<std::vector<int>> locations, std::vector<std::pair<int, double>> fit, std::vector<Location> location_info, int generation_number);
