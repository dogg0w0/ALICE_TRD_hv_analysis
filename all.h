//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Fri Oct 30 22:27:42 2020 by ROOT version 6.22/03
// from TTree TOF_T0_Tree/data from matched csv
// found on file: create_csv/output_all/date_0/current_lumi.csv.root
//////////////////////////////////////////////////////////

#ifndef all_h
#define all_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.

class all
{
public:
   TTree *fChain;  //!pointer to the analyzed TTree or TChain
   Int_t fCurrent; //!current Tree number in a TChain

   // Fixed size dimensions of array or collections stored in the TTree if any.

   // Declaration of leaf types
   Int_t fSec;
   Int_t fNanoSec;
   Double_t TOF_average_current;
   Double_t TOF_rms_current;
   Double_t Luminosity;
   Double_t T0_Luminosity;
   Double_t HV;
   Double_t current;
   Double_t mean_current;
   Double_t std_current;
   Double_t pressure;

   // List of branches
   TBranch *b_fSec;                //!
   TBranch *b_fNanoSec;            //!
   TBranch *b_TOF_average_current; //!
   TBranch *b_TOF_rms_current;     //!
   TBranch *b_Luminosity;          //!
   TBranch *b_T0_Luminosity;       //!
   TBranch *b_HV;                  //!
   TBranch *b_current;             //!
   TBranch *b_mean_current;        //!
   TBranch *b_std_current;         //!
   TBranch *b_pressure;

   all(std::string filename);
   virtual ~all();
   virtual Int_t Cut(Long64_t entry);
   virtual Int_t GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void Init(TTree *tree);
   virtual void Loop();
   virtual Bool_t Notify();
   virtual void Show(Long64_t entry = -1);
};

#endif

#ifdef all_cxx
all::all(std::string filename)
{
   // if parameter tree is not specified (or zero), connect the file
   // used to generate this class and read the Tree.
   TTree *tree = 0;
   auto f = new TFile(filename.c_str(), "READ");
   if (!f || !f->IsOpen())
   {
      std::cerr << "cannot open file:\t" << filename << std::endl;
   }
   f->GetObject("TOF_T0_Tree", tree);

   Init(tree);
}

all::~all()
{
   if (!fChain)
      return;
   delete fChain->GetCurrentFile();
}

Int_t all::GetEntry(Long64_t entry)
{
   // Read contents of entry.
   if (!fChain)
      return 0;
   return fChain->GetEntry(entry);
}
Long64_t all::LoadTree(Long64_t entry)
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

void all::Init(TTree *tree)
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

   fChain->SetBranchAddress("fSec", &fSec, &b_fSec);
   fChain->SetBranchAddress("fNanoSec", &fNanoSec, &b_fNanoSec);
   fChain->SetBranchAddress("TOF_average_current", &TOF_average_current, &b_TOF_average_current);
   fChain->SetBranchAddress("TOF_rms_current", &TOF_rms_current, &b_TOF_rms_current);
   fChain->SetBranchAddress("Luminosity", &Luminosity, &b_Luminosity);
   fChain->SetBranchAddress("T0_Luminosity", &T0_Luminosity, &b_T0_Luminosity);
   fChain->SetBranchAddress("HV", &HV, &b_HV);
   fChain->SetBranchAddress("current", &current, &b_current);
   fChain->SetBranchAddress("mean_current", &mean_current, &b_mean_current);
   fChain->SetBranchAddress("std_current", &std_current, &b_std_current);
   fChain->SetBranchAddress("pressure", &pressure, &b_pressure);
   Notify();
}

Bool_t all::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void all::Show(Long64_t entry)
{
   // Print contents of entry.
   // If entry is not specified, print current entry
   if (!fChain)
      return;
   fChain->Show(entry);
}
Int_t all::Cut(Long64_t entry)
{
   // This function may be called from Loop.
   // returns  1 if entry is accepted.
   // returns -1 otherwise.
   return 1;
}
#endif // #ifdef all_cxx
