//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Thu Dec  8 22:39:11 2022 by ROOT version 6.26/06
// from TTree Events/Events
// found on file: /home/milliqan/data/MilliQan_Run591.1_default.root
//////////////////////////////////////////////////////////

#ifndef DigitizerOutput_h
#define DigitizerOutput_h

#include <utility>
#include <vector>
#include <algorithm>
#include <fstream>

#include "milliqan/constants.h"
#include "milliqan/mappings.h"
#include "milliqan/Pulse.h"

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TH1.h>

// Header file for the classes stored in the TTree if any.
#include "TObject.h"
#include "TTimeStamp.h"

class DigitizerOutput {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

// Fixed size dimensions of array or collections stored in the TTree if any.
   static constexpr Int_t kMaxdigitizers = 5;

   // Declaration of leaf types
 //mdaq::GlobalEvent *event;
   UInt_t          fUniqueID;
   UInt_t          fBits;
   UInt_t          DAQEventNumber;
   Int_t           digitizers_;
   UInt_t          digitizers_fUniqueID[kMaxdigitizers];   //[digitizers_]
   UInt_t          digitizers_fBits[kMaxdigitizers];   //[digitizers_]
   Int_t           digitizers_DAQTimeStamp_fSec[kMaxdigitizers];   //[digitizers_]
   Int_t           digitizers_DAQTimeStamp_fNanoSec[kMaxdigitizers];   //[digitizers_]
   UShort_t        digitizers_TriggerCount[kMaxdigitizers][16];   //[digitizers_]
   UShort_t        digitizers_TimeCount[kMaxdigitizers][16];   //[digitizers_]
   UChar_t         digitizers_EventId[kMaxdigitizers][8];   //[digitizers_]
   ULong_t         digitizers_TDC[kMaxdigitizers][8];   //[digitizers_]
   UInt_t          digitizers_EventSize[kMaxdigitizers];   //[digitizers_]
   UInt_t          digitizers_BoardId[kMaxdigitizers];   //[digitizers_]
   UInt_t          digitizers_Pattern[kMaxdigitizers];   //[digitizers_]
   UInt_t          digitizers_ChannelMask[kMaxdigitizers];   //[digitizers_]
   UInt_t          digitizers_EventCounter[kMaxdigitizers];   //[digitizers_]
   UInt_t          digitizers_TriggerTimeTag[kMaxdigitizers];   //[digitizers_]
   ULong_t         digitizers_TriggerTimeTagRollovers[kMaxdigitizers];   //[digitizers_]
   ULong_t         digitizers_TDCRollovers[kMaxdigitizers][8];   //[digitizers_]
   Bool_t          digitizers_DataPresent[kMaxdigitizers];   //[digitizers_]
   Float_t         digitizers_waveform[kMaxdigitizers][16][1024];   //[digitizers_]
   Double_t        digitizers_triggerClockTick[kMaxdigitizers];   //[digitizers_]
   Double_t        digitizers_tdcClockTick[kMaxdigitizers];   //[digitizers_]
   Float_t         digitizers_nanosecondsPerSample[kMaxdigitizers];   //[digitizers_]

   // List of branches
   TBranch        *b_event_fUniqueID;   //!
   TBranch        *b_event_fBits;   //!
   TBranch        *b_event_DAQEventNumber;   //!
   TBranch        *b_event_digitizers_;   //!
   TBranch        *b_digitizers_fUniqueID;   //!
   TBranch        *b_digitizers_fBits;   //!
   TBranch        *b_digitizers_DAQTimeStamp_fSec;   //!
   TBranch        *b_digitizers_DAQTimeStamp_fNanoSec;   //!
   TBranch        *b_digitizers_TriggerCount;   //!
   TBranch        *b_digitizers_TimeCount;   //!
   TBranch        *b_digitizers_EventId;   //!
   TBranch        *b_digitizers_TDC;   //!
   TBranch        *b_digitizers_EventSize;   //!
   TBranch        *b_digitizers_BoardId;   //!
   TBranch        *b_digitizers_Pattern;   //!
   TBranch        *b_digitizers_ChannelMask;   //!
   TBranch        *b_digitizers_EventCounter;   //!
   TBranch        *b_digitizers_TriggerTimeTag;   //!
   TBranch        *b_digitizers_TriggerTimeTagRollovers;   //!
   TBranch        *b_digitizers_TDCRollovers;   //!
   TBranch        *b_digitizers_DataPresent;   //!
   TBranch        *b_digitizers_waveform;   //!
   TBranch        *b_digitizers_triggerClockTick;   //!
   TBranch        *b_digitizers_tdcClockTick;   //!
   TBranch        *b_digitizers_nanosecondsPerSample;   //!

   // 2D arrays containing waveforms from all channels
   TH1D* waves[constants::nDigitizers][constants::nChannels];
   Mappings mappings;
   std::string outputfilename;
   std::fstream fout;

   DigitizerOutput(TTree *tree=0);
   virtual ~DigitizerOutput();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);

   TH1D * GetWaveform(const unsigned int digitizerID, const unsigned int channelID, const TString name) const;
   void ProcessWaves(int ientry, TH1D* waves[][constants::nChannels]);
   void ProcessWave(int digitizerID, int channelID, TString name);
   std::pair<double,double> MeasureSideband(TH1D* wave);
   std::vector< std::pair<double,double> > FindPulsesBounds(TH1D* wave);
   std::vector<Pulse> MakePulses(std::vector< std::pair<double,double> > pulseBounds, TH1D* wave, int digitizerID, int channelID);
   void CheckMappings(Mappings mappings);
   void SetOutputFile(std::string outputpathbase, std::string runnum, std::string subrunnum);
   void DumpToFile(std::pair<double,double> sideband_mean_rms, std::vector<Pulse> pulses, TString event_id);

};

#endif

#ifdef DigitizerOutput_cxx
DigitizerOutput::DigitizerOutput(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("/home/milliqan/data/MilliQan_Run591.1_default.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("/home/milliqan/data/MilliQan_Run591.1_default.root");
      }
      f->GetObject("Events",tree);

   }
   Init(tree);
   mappings = Mappings();
}

DigitizerOutput::~DigitizerOutput()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t DigitizerOutput::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t DigitizerOutput::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void DigitizerOutput::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("fUniqueID", &fUniqueID, &b_event_fUniqueID);
   fChain->SetBranchAddress("fBits", &fBits, &b_event_fBits);
   fChain->SetBranchAddress("DAQEventNumber", &DAQEventNumber, &b_event_DAQEventNumber);
   fChain->SetBranchAddress("digitizers", &digitizers_, &b_event_digitizers_);
   fChain->SetBranchAddress("digitizers.fUniqueID", digitizers_fUniqueID, &b_digitizers_fUniqueID);
   fChain->SetBranchAddress("digitizers.fBits", digitizers_fBits, &b_digitizers_fBits);
   fChain->SetBranchAddress("digitizers.DAQTimeStamp.fSec", digitizers_DAQTimeStamp_fSec, &b_digitizers_DAQTimeStamp_fSec);
   fChain->SetBranchAddress("digitizers.DAQTimeStamp.fNanoSec", digitizers_DAQTimeStamp_fNanoSec, &b_digitizers_DAQTimeStamp_fNanoSec);
   fChain->SetBranchAddress("digitizers.TriggerCount[16]", digitizers_TriggerCount, &b_digitizers_TriggerCount);
   fChain->SetBranchAddress("digitizers.TimeCount[16]", digitizers_TimeCount, &b_digitizers_TimeCount);
   fChain->SetBranchAddress("digitizers.EventId[8]", digitizers_EventId, &b_digitizers_EventId);
   fChain->SetBranchAddress("digitizers.TDC[8]", digitizers_TDC, &b_digitizers_TDC);
   fChain->SetBranchAddress("digitizers.EventSize", digitizers_EventSize, &b_digitizers_EventSize);
   fChain->SetBranchAddress("digitizers.BoardId", digitizers_BoardId, &b_digitizers_BoardId);
   fChain->SetBranchAddress("digitizers.Pattern", digitizers_Pattern, &b_digitizers_Pattern);
   fChain->SetBranchAddress("digitizers.ChannelMask", digitizers_ChannelMask, &b_digitizers_ChannelMask);
   fChain->SetBranchAddress("digitizers.EventCounter", digitizers_EventCounter, &b_digitizers_EventCounter);
   fChain->SetBranchAddress("digitizers.TriggerTimeTag", digitizers_TriggerTimeTag, &b_digitizers_TriggerTimeTag);
   fChain->SetBranchAddress("digitizers.TriggerTimeTagRollovers", digitizers_TriggerTimeTagRollovers, &b_digitizers_TriggerTimeTagRollovers);
   fChain->SetBranchAddress("digitizers.TDCRollovers[8]", digitizers_TDCRollovers, &b_digitizers_TDCRollovers);
   fChain->SetBranchAddress("digitizers.DataPresent", digitizers_DataPresent, &b_digitizers_DataPresent);
   fChain->SetBranchAddress("digitizers.waveform[16][1024]", digitizers_waveform, &b_digitizers_waveform);
   fChain->SetBranchAddress("digitizers.triggerClockTick", digitizers_triggerClockTick, &b_digitizers_triggerClockTick);
   fChain->SetBranchAddress("digitizers.tdcClockTick", digitizers_tdcClockTick, &b_digitizers_tdcClockTick);
   fChain->SetBranchAddress("digitizers.nanosecondsPerSample", digitizers_nanosecondsPerSample, &b_digitizers_nanosecondsPerSample);
   Notify();
}

Bool_t DigitizerOutput::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void DigitizerOutput::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t DigitizerOutput::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef DigitizerOutput_cxx
