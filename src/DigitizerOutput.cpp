#define DigitizerOutput_cxx
#include "milliqan/DigitizerOutput.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <iostream>     

std::vector<Pulse> DigitizerOutput::MakePulses(std::vector< std::pair<double,double> > pulseBounds,
        TH1D* wave, int digitizerID, int channelID){

    std::vector<Pulse> pulses;

    int npulses = pulseBounds.size();

    for(int ipulse = 0; ipulse<npulses; ipulse++){

        //std::cout << pulseBounds[ipulse].first << ", " << pulseBounds[ipulse].second << std::endl;
	wave->SetAxisRange(pulseBounds[ipulse].first,pulseBounds[ipulse].second);

        // get row, col, layer, type, channel, supermoduleID
        Pulse pulse = Pulse(mappings, digitizerID, channelID);

	float height = wave->GetMaximum();
        float pulseTime = pulseBounds[ipulse].first;  // in ns
	//outputTreeContents.v_time_module_calibrated.push_back(pulseBounds[ipulse].first+timingCalibrations[ic]);
	float area = wave->Integral();
	//outputTreeContents.v_nPE.push_back((waves[ic]->Integral()/(speAreas[ic]))*(0.4/sampleRate));
	float duriation = (pulseBounds[ipulse].second - pulseBounds[ipulse].first); // in ns
        float delay = 9999;
	if(ipulse>0) delay = (pulseBounds[ipulse].first - pulseBounds[ipulse-1].second); // in ns

        pulse.SetPulseProperty(height, area, duriation, pulseTime, delay);
        pulse.SetTDC(digitizers_TDC[digitizerID][channelID], digitizers_TDCRollovers[digitizerID][channelID]);

        pulses.push_back(pulse);
        //std::cout << "height: " << height << ", area: " << area << ", duriation: " << duriation << ", pulseTime: " << pulseTime << std::endl;
        //pulse.Print();
    }    

    return pulses;
}

std::vector< std::pair<double,double> > DigitizerOutput::FindPulsesBounds(TH1D* wave){

    // taken from https://github.com/milliQan-sw/milliqanOffline/blob/master/Run3Detector/src/OfflineFactory.cc

    std::vector<std::pair<double,double> > bounds;
    double tstart = constants::sideband_range.second+1;
    int istart = wave->FindBin(tstart);
    // int istart = 1;
    bool inpulse = false;
    int nover = 0;
    int nunder = 0;
    int i_begin = istart;
    //int i_begin = 0;
    int i_stop_searching = wave->GetNbinsX()-constants::nConsecSamples;
    int i_stop_final_pulse = wave->GetNbinsX();


    for (int i=istart; i<i_stop_searching || (inpulse && i<i_stop_final_pulse); i++) {
	double v = wave->GetBinContent(i);
	if (!inpulse) {
	    if (v<constants::lowThresh) {   
		nover = 0;     // If v dips below the low threshold, store the value of the sample index as i_begin
		i_begin = i;
	    }
	    else if (v>=constants::highThresh){
		nover++;       // If v is above the threshold, start counting the number of sample indices
	    }
	    else{
		i_begin = i;
	    }
	    if (nover>=constants::nConsecSamples){   // If v is above threshold for long enough, we now have a pulse!
		inpulse = true;    // Also reset the value of nunder
		nunder = 0;
	    }
	}
	else {  // Called if we have a pulse
	    if (v<constants::highThresh) nunder++;   // If the pulse dips below the threshold, sum the number of sample indices for wconstants::hich tconstants::his is true
	    else if (v >= constants::highThresh){
		nunder = 0;           // If the pulse stays above threshold, set nunder back to zero
	    }
	    // If the nunder is above or equal to 12 (or we reach the end of the file) store the values of the pulse bounds
	    if (nunder>=constants::nConsecSamplesEnd || i==(i_stop_final_pulse-1)) { 
		bounds.push_back({(double)wave->GetBinLowEdge(i_begin), (double)wave->GetBinLowEdge(i+1)-0.01});
                //std::cout<<"i_begin, i: "<<i_begin<<" "<<i<<std::endl;       // i_begin is the 
		inpulse = false;
		nover = 0;
		nunder = 0;
		i_begin = i;
	    }
	}
    }
    return bounds;
}

std::pair<double,double> DigitizerOutput::MeasureSideband(TH1D* wave){

    // taken from https://github.com/milliQan-sw/milliqanOffline/blob/master/Run3Detector/src/OfflineFactory.cc

    double sum_sb=0.;
    double sum2_sb=0.;
    int startbin = wave->FindBin(constants::sideband_range.first);
    int endbin = wave->FindBin(constants::sideband_range.second);
    int n_sb = 0;
    for(int ibin=startbin; ibin <= endbin; ibin++){
	sum_sb = sum_sb + wave->GetBinContent(ibin);
	sum2_sb = sum2_sb + pow(wave->GetBinContent(ibin),2);
	n_sb++;
    }
    if(n_sb == 0) n_sb = 1.;
    double mean = sum_sb/n_sb;
    double RMS = pow( sum2_sb/n_sb - pow(mean,2), 0.5);

    return std::make_pair(mean,RMS);

}

void DigitizerOutput::ProcessWave(int digitizerID, int channelID, TString name) {

    TH1D* wave = GetWaveform(digitizerID, channelID, name);

    // get sideband mean/rms
    std::pair<double,double> sideband_mean_rms = MeasureSideband(wave);

    // get actual pulses
    std::vector< std::pair<double,double> > pulses_bounds = FindPulsesBounds(wave);
    //std::cout << "number of pulses: " << pulses_bounds.size() << std::endl;
    std::vector<Pulse> pulses = MakePulses(pulses_bounds, wave, digitizerID, channelID);

    // dump
    // https://stackoverflow.com/questions/14265581/parse-split-a-string-in-c-using-string-delimiter-standard-c
    std::string name_s(name.Data());
    std::string event_id = name_s.substr(0, name_s.find("_digi_")); // token is "scott"
                                                           
    if (pulses.size() > 0) {
        //std::cout << "sideband mean: " << sideband_mean_rms.first << ", rms: " << sideband_mean_rms.second << std::endl;
        DumpToFile(sideband_mean_rms, pulses, event_id);
    }

    // longer term furture, could be nice to save two dataframe, one per channel info (VMax, sideband etc), one per pulse info (height, area, duriation)

}

void DigitizerOutput::SetOutputFile(std::string outputpathbase, std::string runnum, std::string subrunnum) {

    outputfilename =  outputpathbase + "Digitizer_run" + runnum + "_subrun" + subrunnum + ".csv";

    // opens an existing csv file or creates a new file.
    fout.open(outputfilename, std::ios::out | std::ios::app);

    fout << "event_id,"
         << "channel,"
         << "layer,"
         << "supermodule,"
         << "row,"
         << "column,"
         << "sideband_mean,"
         << "sideband_rms,"
         << "nPulses,"
         << "pulseHeight_max,"
         << "pulseHeight_min,"
         << "pulseArea_max,"
         << "pulseArea_min,"
         << "pulseDuriation_max,"
         << "pulseDuriation_min,"
         << "TDC,"
         << "TDCRollovers"
         << "\n";

}

void DigitizerOutput::DumpToFile(std::pair<double,double> sideband_mean_rms, std::vector<Pulse> pulses, TString event_id) {

    // could add options if needed, e.g. dump processed data (max/min), or per pulse, or per event etc
    
    double sideband_mean = sideband_mean_rms.first; 
    double sideband_rms = sideband_mean_rms.second; 

    Pulse first_pulse = pulses[0];
    int channel = first_pulse.channel;
    int supermodule = first_pulse.supermodule;
    int layer = first_pulse.layer;
    int row = first_pulse.row;
    int column = first_pulse.column;
    float TDC = first_pulse.TDC;
    float TDCRollovers = first_pulse.TDCRollovers;

    sort( pulses.begin( ), pulses.end( ), [ ]( const Pulse& lhs, const Pulse& rhs )
    {
       return lhs.height < rhs.height;
    });

    float pulseHeight_max = pulses.back().height; 
    float pulseHeight_min = pulses[0].height; 

    sort( pulses.begin( ), pulses.end( ), [ ]( const Pulse& lhs, const Pulse& rhs )
    {
       return lhs.area < rhs.area;
    });

    float pulseArea_max = pulses.back().area; 
    float pulseArea_min = pulses[0].area; 

    sort( pulses.begin( ), pulses.end( ), [ ]( const Pulse& lhs, const Pulse& rhs )
    {
       return lhs.duriation < rhs.duriation;
    });

    float pulseDuriation_max = pulses.back().duriation; 
    float pulseDuriation_min = pulses[0].duriation; 

    fout << event_id           << ","
         << channel            << ","
         << layer              << ","
         << supermodule        << ","
         << row                << ","
         << column             << ","
         << sideband_mean      << ","
         << sideband_rms       << ","
         << pulses.size()      << ","
         << pulseHeight_max    << ","
         << pulseHeight_min    << ","
         << pulseArea_max      << ","
         << pulseArea_min      << ","
         << pulseDuriation_max << ","
         << pulseDuriation_min << ","
         << TDC                << ","
         << TDCRollovers       << "\n";

}

TH1D * DigitizerOutput::GetWaveform(const unsigned int digitizerID, const unsigned int channelID, const TString name) const {

    TH1D * wave = new TH1D(name, name, constants::nSamples, 0, constants::nSamples * constants::nanosecondsPerSample);

    for(unsigned int i = 0; i < constants::nSamples; i++) {

        wave->SetBinContent(i+1, digitizers_waveform[digitizerID][channelID][i]);
    }

    return wave;
}

//void DigitizerOutput::ProcessWaves(int ientry, TH1D* waves[][constants::nChannels]) {
void DigitizerOutput::ProcessWaves(int ientry) {

      for (int digitizerID = 0; digitizerID < constants::nDigitizers; digitizerID++) {
          for (int channelID = 0; channelID < constants::nChannels; channelID++) {

                const TString name = Form("evt_%d_digi_%d_chan_%d", ientry, digitizerID, channelID); 
                //waves[digitizerID][channelID] = GetWaveform(digitizerID, channelID, name);
                ProcessWave(digitizerID, channelID, name);
                //std::cout << "VMax for digitizer " << digitizerID << ", channel " << channelID << " is: " << waves[digitizerID][channelID]->GetMaximum() << std::endl;
          }
      }

}

void DigitizerOutput::CheckMappings(Mappings mappings) {

      for (int digitizerID = 0; digitizerID < constants::nDigitizers; digitizerID++) {
          for (int channelID = 0; channelID < constants::nChannels; channelID++) {
              TString info = Form("Digitizer: %d, Channel: %d => ChannelID: %d, SupermoduleID: %d, Layer: %d, Row: %d, Column: %d, RowExt: %d, ColumnExt: %d", 
                      digitizerID, channelID,
                      mappings.channel[digitizerID][channelID],
                      mappings.supermodule[digitizerID][channelID],
                      mappings.layer[digitizerID][channelID],
                      mappings.row[digitizerID][channelID],
                      mappings.column[digitizerID][channelID],
                      mappings.row_ext[digitizerID][channelID],
                      mappings.column_ext[digitizerID][channelID]);

              std::cout << info << std::endl;
          }
      }
}

void DigitizerOutput::Loop()
{

   if (fChain == 0) return;

   Long64_t nentries = fChain->GetEntriesFast();

   // TODO check nDigitizers, nChannels, nSamples not exceeding requirement
   //Mappings mappings = Mappings();
   //CheckMappings(mappings);
   
   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;

      // for each event, save waveform for each channel (5 digitizers X 16 channels) into histograms (TH1D)
      //ProcessWaves(ientry, waves);
      ProcessWaves(ientry);

      // if (Cut(ientry) < 0) continue;

   }
}
