//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Wed Sep 16 09:32:24 2020 by ROOT version 6.22/03
// from TTree Tree_TRD_HV/Tree_TRD_HV
// found on file: sorted_1.csv.root
//////////////////////////////////////////////////////////

#ifndef readroot_csv_h
#define readroot_csv_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <iostream>
#include <fstream>

// Header file for the classes stored in the TTree if any.
#include "TTimeStamp.h"

class readroot_csv
{
public:
   TTree *fChain;  //!pointer to the analyzed TTree or TChain
   Int_t fCurrent; //!current Tree number in a TChain

   // Fixed size dimensions of array or collections stored in the TTree if any.

   // Declaration of leaf types
   Double_t HV;
   //TTimeStamp      *time;
   Int_t fSec;
   Int_t fNanoSec;

   std::string outfile;

       // List of branches
       TBranch *b_HV;        //!
   TBranch *b_time_fSec;     //!
   TBranch *b_time_fNanoSec; //!

   readroot_csv(std::string filename, std::string outfile_name);
   virtual ~readroot_csv();
   virtual Int_t Cut(Long64_t entry);
   virtual Int_t GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void Init(TTree *tree);
   virtual void Loop(Int_t start_time, Int_t end_time);
   virtual Bool_t Notify();
   virtual void Show(Long64_t entry = -1);
};

#endif

#ifdef readroot_csv_cxx
readroot_csv::readroot_csv(std::string filename, std::string outfile_name)
{
   outfile = outfile_name;
   TTree *tree = 0;
   TFile *f = new TFile(filename.c_str(), "READ");
   if (!f || !f->IsOpen())
   {
      std::cerr << "cannot open file:\t" << filename << std::endl;
   }
   f->GetObject("Tree_TRD_HV", tree);

   Init(tree);
}

readroot_csv::~readroot_csv()
{
   if (!fChain)
      return;
   delete fChain->GetCurrentFile();
}

Int_t readroot_csv::GetEntry(Long64_t entry)
{
   // Read contents of entry.
   if (!fChain)
      return 0;
   return fChain->GetEntry(entry);
}
Long64_t readroot_csv::LoadTree(Long64_t entry)
{
   // Set the environment to read one entry
   if (!fChain)
      return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0)
      return centry;
   if (fChain->GetTreeNumber() != fCurrent)
   {
      fCurrent = fChain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void readroot_csv::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set branch addresses and branch pointers
   if (!tree)
      return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("HV", &HV, &b_HV);
   fChain->SetBranchAddress("fSec", &fSec, &b_time_fSec);
   fChain->SetBranchAddress("fNanoSec", &fNanoSec, &b_time_fNanoSec);
   Notify();
}

Bool_t readroot_csv::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void readroot_csv::Show(Long64_t entry)
{
   // Print contents of entry.
   // If entry is not specified, print current entry
   if (!fChain)
      return;
   fChain->Show(entry);
}
Int_t readroot_csv::Cut(Long64_t entry)
{
   // This function may be called from Loop.
   // returns  1 if entry is accepted.
   // returns -1 otherwise.
   return 1;
}
#endif // #ifdef readroot_csv_cxx
