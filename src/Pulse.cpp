#include "milliqan/Pulse.h"

Pulse::Pulse(Mappings mappings, int digitizerID, int channelID) {

    channel = mappings.channel[digitizerID][channelID];
    supermodule = mappings.supermodule[digitizerID][channelID];
    layer = mappings.layer[digitizerID][channelID];
    row = mappings.row[digitizerID][channelID];
    column = mappings.column[digitizerID][channelID];

}

void Pulse::SetPulseProperty(float ht, float a, float dur, float pulseT, float dly) {

    height = ht;
    area = a;
    duriation = dur;
    pulseTime = pulseT;
    delay = dly;

}

void Pulse::SetTDC(float TDC_, float TDCRollovers_) {

    TDC = TDC_;
    TDCRollovers = TDCRollovers_;

}

Pulse::~Pulse() {

}

void Pulse::Print() {

    std::cout << "ch: " << channel << ", supermodule: " << supermodule << ", la: " << layer <<
        ", row: " << row << ", col: " << column <<
        ", height: " << height << ", area: " << area << ", duriation: " << duriation << ", pulseTime: " << pulseTime << 
        ", delay: " << delay << ", TDC: " << TDC << ", TDCRollovers: " << TDCRollovers << std::endl;

}

    /*
bool Pulse::IsShortPulse() {

	int above20 = -2;
	int above80 = -1;
	for (int iStart = waves[ic]->FindBin(pulseBounds[ipulse].first); iStart <= waves[ic]->FindBin(pulseBounds[ipulse].second); iStart++){
	    if (waves[ic]->GetBinContent(iStart) > height*0.2 && above20 < 0){
		above20 = iStart;
	    }
	    if (waves[ic]->GetBinContent(iStart) > height*0.8 && above80 < 0){
		above80 = iStart;
	    }
	    if (above80 >= 0) break;
	}
	int riseSamples = above80-above20;
	outputTreeContents.v_riseSamples.push_back(above80-above20);
	above20 = -2;
	above80 = -1;

	for (int iFall = waves[ic]->FindBin(pulseBounds[ipulse].second); iFall >= waves[ic]->FindBin(pulseBounds[ipulse].first); iFall--){
	    if (waves[ic]->GetBinContent(iFall) > height*0.2 && above20 < 0){
		above20 = iFall;
	    }
	    if (waves[ic]->GetBinContent(iFall) > height*0.8 && above80 < 0){
		above80 = iFall;
	    }
	    if (above80 >= 0) break;
	}
	int fallSamples = above20-above80;
	outputTreeContents.v_fallSamples.push_back(above20-above80);

	bool qual = true;
	if (fallSamples < 2) qual=false;
	if (qual && riseSamples < 2) qual=false;
	if (qual && (height > 17. && height<100.) && (0.001*area < 0.04*(height-17.))) qual=false;
	if (qual && height < 25. && duration < 4.6*(height-12.)) qual=false;
	if (qual && height >= 25. && duration < 60. && fallSamples < 6) qual=false;
	if (qual && 0.001*area < 0.4 && duration < 150.*(0.001*area)) qual=false;
	if (qual && 0.001*area >= 0.4 && duration < 60.) qual=false;
	outputTreeContents.v_pickupFlag.push_back(!qual);
}
    */

