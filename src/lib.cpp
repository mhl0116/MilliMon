#include <milliqan/lib.h>

std::pair<int, float> largest(float arr[], int n, bool debug=false)
{
    int i;
    int imax = 0;
     
    // Initialize maximum element
    float max = arr[0];
 
    // Traverse array elements
    // from second and compare
    // every element with current max
    for (i = 1; i < n; i++) {
        if (debug) std::cout << i << ", " << arr[i] << std::endl;
        if (arr[i] > max) {
            max = arr[i];
            imax = i;
        }
    }
 
    return std::make_pair(imax, max);
}

TTree* GetTree(std::string filename, std::string treename) {

    // open file, return a tree
    TFile* inputfile = TFile::Open(filename.data());
    TTree* tree = (TTree*)inputfile->Get(treename.data());
    return tree;

}

void postprocess_digitizer(std::string runnum, std::string subrunnum) {

    //TODO put location of file and pattern of filename into some global constant or function
    std::string filename = "/home/milliqan/data/MilliQan_Run" + runnum + "." + subrunnum + "_default.root";
    TTree* tree = GetTree(filename, "Events");
    DigitizerOutput digitizerOutput = DigitizerOutput(tree);
    digitizerOutput.Loop();

}

void postprocess_triggerboard(std::string runnum, std::string subrunnum) {

}
