#define DigitizerOutput_cxx
#include "milliqan/DigitizerOutput.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <iostream>     // std::cout

TH1D * DigitizerOutput::GetWaveform(const unsigned int digitizerID, const unsigned int channelID, const TString name) const {

    TH1D * wave = new TH1D(name, name, constants::nSamples, 0, constants::nSamples * constants::nanosecondsPerSample);

    for(unsigned int i = 0; i < constants::nSamples; i++) {
        wave->SetBinContent(i+1, digitizers_waveform[digitizerID][channelID][i]);
    }

    return wave;
}

void DigitizerOutput::Loop()
{
//   In a ROOT session, you can do:
//      root> .L DigitizerOutput.C
//      root> DigitizerOutput t
//      root> t.GetEntry(12); // Fill t data members with entry number 12
//      root> t.Show();       // Show values of entry 12
//      root> t.Show(16);     // Read and show values of entry 16
//      root> t.Loop();       // Loop on all entries
//

//     This is the loop skeleton where:
//    jentry is the global entry number in the chain
//    ientry is the entry number in the current Tree
//  Note that the argument to GetEntry must be:
//    jentry for TChain::GetEntry
//    ientry for TTree::GetEntry and TBranch::GetEntry
//
//       To read only selected branches, Insert statements like:
// METHOD1:
//    fChain->SetBranchStatus("*",0);  // disable all branches
//    fChain->SetBranchStatus("branchname",1);  // activate branchname
// METHOD2: replace line
//    fChain->GetEntry(jentry);       //read all branches
//by  b_branchname->GetEntry(ientry); //read only this branch

   if (fChain == 0) return;

   Long64_t nentries = fChain->GetEntriesFast();

   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;

      // FIXME condense into a function 
      for (int digitizerID = 0; digitizerID < constants::nDigitizers; digitizerID++) {
          for (int channelID = 0; channelID < constants::nChannels; channelID++) {

                const TString name = Form("evt_%d_digi_%d_chan_%d", ientry, digitizerID, channelID); 
                waves[digitizerID][channelID] = GetWaveform(digitizerID, channelID, name);
                // FIXME change to spdlog
                std::cout << "VMax for digitizer " << digitizerID << ", channel " << channelID << " is: " << waves[digitizerID][channelID]->GetMaximum() << std::endl;
          }
      }

      /*
         VMax, average/max/min(nPE, pulse height, pulse width, area) vs time
         pedestal mean and rms
         number of pulses per layer / supermodule / channel / detector ?
         event display when there is bit 1 or 2 fired
         matching situation
      */

      // make pulse
      // extra/save useful information (...)
      // dump info to file
      /*
        - [X] allPulseBounds = readWaveDataPerEvent(i);
        - [X] loadWavesMilliDAQ // get wave form using a function from millidaq (where is wave initialized?)
        - [ ] processChannel // find pulse for each channel
        - [ ] prepareWave // deals with pedestal subtraction and sideband mean and RMS calculation here , no pedestal for now
        - [ ] vector<pair<float,float>> pulseBounds = findPulses(ic);
      */

      // if (Cut(ientry) < 0) continue;

   }
}
