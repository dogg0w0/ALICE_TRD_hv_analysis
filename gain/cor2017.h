//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Tue Oct 27 14:31:11 2020 by ROOT version 6.22/03
// from TTree Results/Results
// found on file: trendingOCDB_2017.root
//////////////////////////////////////////////////////////

#ifndef cor2017_h
#define cor2017_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.

class cor2017 {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

// Fixed size dimensions of array or collections stored in the TTree if any.

   // Declaration of leaf types
   Double_t        meanExB;
   Double_t        rmsExB;
   Double_t        meanGainFactor;
   Double_t        rmsGainFactor;
   Double_t        meanT0;
   Double_t        rmsT0;
   Double_t        meanVdrift;
   Double_t        rmsVdrift;
   Float_t         gain;
   Float_t         vd;
   Float_t         exb;
   Float_t         t0;
   Int_t           detector;
   Double_t        temperature;
   Double_t        pressure;
   Double_t        gvd;
   Double_t        gg;
   Double_t        H20;
   Double_t        N2;
   Double_t        C02;
   Double_t        Xe;
   Double_t        O2;
   Double_t        anodev;
   Double_t        driftv;
   Double_t        run;
   Int_t           fillnumber;
   Double_t        luminosity;
   Int_t           NbOfInteractingBunch;
   Int_t           goodchamber;

   // List of branches
   TBranch        *b_meanExB;   //!
   TBranch        *b_rmsExB;   //!
   TBranch        *b_meanGainFactor;   //!
   TBranch        *b_rmsGainFactor;   //!
   TBranch        *b_meanT0;   //!
   TBranch        *b_rmsT0;   //!
   TBranch        *b_meanVdrift;   //!
   TBranch        *b_rmsVdrift;   //!
   TBranch        *b_gain;   //!
   TBranch        *b_vd;   //!
   TBranch        *b_exb;   //!
   TBranch        *b_t0;   //!
   TBranch        *b_detector;   //!
   TBranch        *b_temperature;   //!
   TBranch        *b_pressure;   //!
   TBranch        *b_gvd;   //!
   TBranch        *b_gg;   //!
   TBranch        *b_H20;   //!
   TBranch        *b_N2;   //!
   TBranch        *b_C02;   //!
   TBranch        *b_Xe;   //!
   TBranch        *b_02;   //!
   TBranch        *b_anodev;   //!
   TBranch        *b_driftv;   //!
   TBranch        *b_run;   //!
   TBranch        *b_fillnumber;   //!
   TBranch        *b_luminosity;   //!
   TBranch        *b_NbOfInteractingBunch;   //!
   TBranch        *b_goodchamber;   //!

   cor2017(TTree *tree=0);
   virtual ~cor2017();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef cor2017_cxx
cor2017::cor2017(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("trendingOCDB_2017.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("trendingOCDB_2017.root");
      }
      f->GetObject("Results",tree);

   }
   Init(tree);
}

cor2017::~cor2017()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t cor2017::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t cor2017::LoadTree(Long64_t entry)
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

void cor2017::Init(TTree *tree)
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

   fChain->SetBranchAddress("meanExB", &meanExB, &b_meanExB);
   fChain->SetBranchAddress("rmsExB", &rmsExB, &b_rmsExB);
   fChain->SetBranchAddress("meanGainFactor", &meanGainFactor, &b_meanGainFactor);
   fChain->SetBranchAddress("rmsGainFactor", &rmsGainFactor, &b_rmsGainFactor);
   fChain->SetBranchAddress("meanT0", &meanT0, &b_meanT0);
   fChain->SetBranchAddress("rmsT0", &rmsT0, &b_rmsT0);
   fChain->SetBranchAddress("meanVdrift", &meanVdrift, &b_meanVdrift);
   fChain->SetBranchAddress("rmsVdrift", &rmsVdrift, &b_rmsVdrift);
   fChain->SetBranchAddress("gain", &gain, &b_gain);
   fChain->SetBranchAddress("vd", &vd, &b_vd);
   fChain->SetBranchAddress("exb", &exb, &b_exb);
   fChain->SetBranchAddress("t0", &t0, &b_t0);
   fChain->SetBranchAddress("detector", &detector, &b_detector);
   fChain->SetBranchAddress("temperature", &temperature, &b_temperature);
   fChain->SetBranchAddress("pressure", &pressure, &b_pressure);
   fChain->SetBranchAddress("gvd", &gvd, &b_gvd);
   fChain->SetBranchAddress("gg", &gg, &b_gg);
   fChain->SetBranchAddress("H20", &H20, &b_H20);
   fChain->SetBranchAddress("N2", &N2, &b_N2);
   fChain->SetBranchAddress("C02", &C02, &b_C02);
   fChain->SetBranchAddress("Xe", &Xe, &b_Xe);
   fChain->SetBranchAddress("02", &O2, &b_02);
   fChain->SetBranchAddress("anodev", &anodev, &b_anodev);
   fChain->SetBranchAddress("driftv", &driftv, &b_driftv);
   fChain->SetBranchAddress("run", &run, &b_run);
   fChain->SetBranchAddress("fillnumber", &fillnumber, &b_fillnumber);
   fChain->SetBranchAddress("luminosity", &luminosity, &b_luminosity);
   fChain->SetBranchAddress("NbOfInteractingBunch", &NbOfInteractingBunch, &b_NbOfInteractingBunch);
   fChain->SetBranchAddress("goodchamber", &goodchamber, &b_goodchamber);
   Notify();
}

Bool_t cor2017::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void cor2017::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t cor2017::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef cor2017_cxx
