//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Mon Dec 12 23:12:26 2022 by ROOT version 6.26/06
// from TTree Events/Trigger Board Events
// found on file: /home/milliqan/data/TriggerBoard_Run591.1.root
//////////////////////////////////////////////////////////

#ifndef TriggerBoardOutput_h
#define TriggerBoardOutput_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TH1.h>

#include <fstream>

#include "milliqan/constants.h"
// Header file for the classes stored in the TTree if any.

class TriggerBoardOutput {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

// Fixed size dimensions of array or collections stored in the TTree if any.

   // Declaration of leaf types
   ULong_t         clockCycles;
   ULong_t         trigger;
   Float_t         time;
   ULong_t         startTime;
   std::string runnum_;
   std::string subrunnum_;
   std::string outputfilename;
   std::fstream fout;

   // List of branches
   TBranch        *b_clockCycles;   //!
   TBranch        *b_trigger;   //!
   TBranch        *b_time;   //!
   TBranch        *b_startTime;   //!

   TH1D* h_triggerCounts;
   float time_start;
   float time_stop;

   TriggerBoardOutput(TTree *tree=0);
   virtual ~TriggerBoardOutput();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);

   void SetOutputFile(std::string outputpathbase, std::string runnum, std::string subrunnum);
   void GetTriggerCounts();
   void DumpToFile();
};

#endif

#ifdef TriggerBoardOutput_cxx
TriggerBoardOutput::TriggerBoardOutput(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("/home/milliqan/data/TriggerBoard_Run591.1.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("/home/milliqan/data/TriggerBoard_Run591.1.root");
      }
      f->GetObject("Events",tree);

   }
   Init(tree);
}

TriggerBoardOutput::~TriggerBoardOutput()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t TriggerBoardOutput::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t TriggerBoardOutput::LoadTree(Long64_t entry)
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

void TriggerBoardOutput::Init(TTree *tree)
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

   fChain->SetBranchAddress("clockCycles", &clockCycles, &b_clockCycles);
   fChain->SetBranchAddress("trigger", &trigger, &b_trigger);
   fChain->SetBranchAddress("time", &time, &b_time);
   fChain->SetBranchAddress("startTime", &startTime, &b_startTime);
   
   h_triggerCounts = new TH1D("h_triggerCounts", "", constants::nTriggerBits, 0, constants::nTriggerBits);

   Notify();
}

Bool_t TriggerBoardOutput::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void TriggerBoardOutput::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t TriggerBoardOutput::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef TriggerBoardOutput_cxx
