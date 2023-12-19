#pragma once
#include <iostream>
#include <vector>

void ProcessCommandArgs(int argc, const char* argv[]);

void find_permutations(std::vector<std::string>& permutations, std::vector<std::string>& pass, int length, int pass_size, std::string combo);
