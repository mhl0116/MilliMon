#include "milliqan/mappings.h"

Mappings::Mappings() {

    for (int digitizerID = 0; digitizerID < constants::nDigitizers; digitizerID++) {
        for (int channelID = 0; channelID < constants::nChannels; channelID++) {

            channel[digitizerID][channelID] = SetChannelID(digitizerID, channelID);
            supermodule[digitizerID][channelID] = SetSupermoduleID(channelID);
            layer[digitizerID][channelID] = digitizerID;
            row[digitizerID][channelID] = MAP_ROW[channelID] - 1;
            column[digitizerID][channelID] = MAP_COL[channelID] - 1;
            row_ext[digitizerID][channelID] = SetRowExt(digitizerID, channelID); 
            column_ext[digitizerID][channelID] = SetColumnExt(digitizerID, channelID);

        }
    }
    
}

Mappings::~Mappings() {
    return;
}

int Mappings::SetChannelID(int digitizerID, int channelID) {

    return ((digitizerID * 16) +  channelID);

}

int Mappings::SetSupermoduleID(int channelID) {

    return int((channelID%16)/4);

}

int Mappings::SetRowExt(int digitizerID, int channelID) {

    return MAP_ROW[channelID] + 4*digitizerID - 1;

}

int Mappings::SetColumnExt(int digitizerID, int channelID) {

    return MAP_COL[channelID] + 4*digitizerID - 1;

}

