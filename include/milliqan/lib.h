#include <utility>      // std::pair, std::make_pair
#include <iostream>     // std::cout
#include <string>

#include <milliqan/DigitizerOutput.h>
#include <milliqan/TriggerBoardOutput.h>
#include "TFile.h"
#include "TTree.h"

std::pair<int, float> largest(float arr[], int n, bool debug);

void postprocess_digitizer(std::string runnum, std::string subrunnum);
void postprocess_triggerboard(std::string runnum, std::string subrunnum);
TTree* GetTree(std::string filename, std::string treename);
