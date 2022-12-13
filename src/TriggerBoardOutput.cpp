#define TriggerBoardOutput_cxx
#include "milliqan/TriggerBoardOutput.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>

void TriggerBoardOutput::SetOutputFile(std::string outputpathbase, std::string runnum, std::string subrunnum) {

    //outputfilename =  outputpathbase + "TriggerBoard_run" + runnum + "_subrun" + subrunnum + ".csv";
    runnum_ = runnum;
    subrunnum_ = subrunnum;
    outputfilename =  outputpathbase + "TriggerBoard_run" + runnum + ".csv";

    // opens an existing csv file or creates a new file.
    fout.open(outputfilename, std::ios::out | std::ios::app);

    if (subrunnum == "1") {

        /*
        fout << "triggerRate_tot,  "
             << "triggerRate_bit1, "
             << "triggerRate_bit2, "
             << "triggerRate_bit3, "
             << "triggerRate_bit4, "
             << "triggerRate_bit5, "
             << "triggerRate_bit6, "
             << "triggerRate_bit7, "
             << "triggerRate_bit8, "
             << "triggerCounts_tot,  "
             << "triggerCounts_bit1, "
             << "triggerCounts_bit2, "
             << "triggerCounts_bit3, "
             << "triggerCounts_bit4, "
             << "triggerCounts_bit5, "
             << "triggerCounts_bit6, "
             << "triggerCounts_bit7, "
             << "triggerCounts_bit8  "
             << "\n";
         */
        fout << "subrunnum, ";
        for (int iTrigger = 0; iTrigger < constants::nTriggerBits; iTrigger++) {

            fout << "triggerRate_bit" << iTrigger + 1 << ", "
                 << "triggerCounts_bit" << iTrigger + 1 << ", ";

        }

        fout << "triggerRate_tot,  "   
             << "triggerCounts_tot  " << "\n";
    }
}

void TriggerBoardOutput::GetTriggerCounts() {

      for (int iTrigger = 0; iTrigger < constants::nTriggerBits; iTrigger++) {
          // check which trigger bit is fired
          if ( ((trigger >> iTrigger) & 1) == true) {
              h_triggerCounts->Fill(iTrigger);
          }
      }

}

void TriggerBoardOutput::DumpToFile() {

    fout << subrunnum_ << ", ";
    float time_diff = time_stop - time_start;
    for (int iTrigger = 0; iTrigger < constants::nTriggerBits; iTrigger++) {

        fout << h_triggerCounts->GetBinContent(iTrigger+1)/time_diff << ", "  
             << h_triggerCounts->GetBinContent(iTrigger+1) << ", ";

    }

    fout << h_triggerCounts->Integral()/time_diff << ", " 
         << h_triggerCounts->Integral() << "\n";
}

void TriggerBoardOutput::Loop()
{

   if (fChain == 0) return;

   Long64_t nentries = fChain->GetEntriesFast();

   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;

      if (ientry == 0) time_start = clockCycles * constants::secondsPerCycle;
      if (ientry == nentries-1) time_stop = clockCycles * constants::secondsPerCycle;

      GetTriggerCounts();

      // if (Cut(ientry) < 0) continue;
   }

   DumpToFile();

}
