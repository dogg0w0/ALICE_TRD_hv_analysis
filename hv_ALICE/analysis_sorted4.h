//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Wed Sep 16 09:20:23 2020 by ROOT version 6.22/03
// from TTree Tree_TRD_HV/Tree_TRD_HV
// found on file: sorted_4.csv.root
//////////////////////////////////////////////////////////

#ifndef analysis_sorted4_h
#define analysis_sorted4_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.
#include "TTimeStamp.h"

class analysis_sorted4 {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

// Fixed size dimensions of array or collections stored in the TTree if any.

   // Declaration of leaf types
   Double_t        HV;
 //TTimeStamp      *time;
   Int_t           fSec;
   Int_t           fNanoSec;

   // List of branches
   TBranch        *b_HV;   //!
   TBranch        *b_time_fSec;   //!
   TBranch        *b_time_fNanoSec;   //!

   analysis_sorted4(TTree *tree=0);
   virtual ~analysis_sorted4();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef analysis_sorted4_cxx
analysis_sorted4::analysis_sorted4(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("sorted_4.csv.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("sorted_4.csv.root");
      }
      f->GetObject("Tree_TRD_HV",tree);

   }
   Init(tree);
}

analysis_sorted4::~analysis_sorted4()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t analysis_sorted4::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t analysis_sorted4::LoadTree(Long64_t entry)
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

void analysis_sorted4::Init(TTree *tree)
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

   fChain->SetBranchAddress("HV", &HV, &b_HV);
   fChain->SetBranchAddress("fSec", &fSec, &b_time_fSec);
   fChain->SetBranchAddress("fNanoSec", &fNanoSec, &b_time_fNanoSec);
   Notify();
}

Bool_t analysis_sorted4::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void analysis_sorted4::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t analysis_sorted4::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef analysis_sorted4_cxx
