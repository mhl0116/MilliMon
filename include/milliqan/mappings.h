#ifndef Mappings_h
#define Mappings_h

#include <map>
#include "milliqan/constants.h"

// define your own namespace to holdi mappings 
class Mappings {
public : 

    int channel[constants::nDigitizers][constants::nChannels] = {-1};
    int supermodule[constants::nDigitizers][constants::nChannels] = {-1};
    int layer[constants::nDigitizers][constants::nChannels] = {-1};
    int row[constants::nDigitizers][constants::nChannels] = {-1};
    int column[constants::nDigitizers][constants::nChannels] = {-1};
    int row_ext[constants::nDigitizers][constants::nChannels] = {-1};
    int column_ext[constants::nDigitizers][constants::nChannels] = {-1};

    std::map<int, int> MAP_ROW{
        {0, 1}, {1, 2}, {4, 3}, {5, 4},
        {2, 1}, {3, 2}, {6, 3}, {7, 4},
        {8, 1}, {9, 2}, {12, 3}, {13, 4},
        {10, 1}, {11, 2}, {14, 3}, {15, 4}
    };

    std::map<int, int> MAP_COL{
        {0, 4}, {1, 4}, {4, 4}, {5, 4},
        {2, 3}, {3, 3}, {6, 3}, {7, 3},
        {8, 2}, {9, 2}, {12, 2}, {13, 2},
        {10, 1}, {11, 1}, {14, 1}, {15, 1}
    };

    Mappings();
    virtual ~Mappings();
    int SetChannelID(int digitizerID, int channelID);
    int SetSupermoduleID(int channelID);
    int SetRowExt(int digitizerID, int channelID);
    int SetColumnExt(int digitizerID, int channelID);

};
#endif
