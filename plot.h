//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Sun Sep 27 16:39:02 2020 by ROOT version 6.22/03
// from TTree TOF_T0_Tree/data from matched csv
// found on file: create_csv/output/date_0/sorted_2.csv.root
//////////////////////////////////////////////////////////

#ifndef plot_h
#define plot_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.

class plot
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

   // List of branches
   TBranch *b_fSec;                //!
   TBranch *b_fNanoSec;            //!
   TBranch *b_TOF_average_current; //!
   TBranch *b_TOF_rms_current;     //!
   TBranch *b_Luminosity;          //!
   TBranch *b_T0_Luminosity;       //!
   TBranch *b_HV;                  //!
   TBranch *b_current;             //!

   plot(std::string filename);
   virtual ~plot();
   virtual Int_t Cut(Long64_t entry);
   virtual Int_t GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void Init(TTree *tree);
   virtual void Loop();
   virtual Bool_t Notify();
   virtual void Show(Long64_t entry = -1);
};

#endif

#ifdef plot_cxx
plot::plot(std::string filename)
{
   // if parameter tree is not specified (or zero), connect the file
   // used to generate this class and read the Tree.
   TTree *tree = 0;
   TFile *f = new TFile(filename.c_str(), "READ");
   if (!f || !f->IsOpen())
   {
      std::cerr << "cannot open file:\t" << filename << std::endl;
   }
   f->GetObject("TOF_T0_Tree", tree);

   Init(tree);
}

plot::~plot()
{
   if (!fChain)
      return;
   delete fChain->GetCurrentFile();
}

Int_t plot::GetEntry(Long64_t entry)
{
   // Read contents of entry.
   if (!fChain)
      return 0;
   return fChain->GetEntry(entry);
}
Long64_t plot::LoadTree(Long64_t entry)
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

void plot::Init(TTree *tree)
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
   Notify();
}

Bool_t plot::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void plot::Show(Long64_t entry)
{
   // Print contents of entry.
   // If entry is not specified, print current entry
   if (!fChain)
      return;
   fChain->Show(entry);
}
Int_t plot::Cut(Long64_t entry)
{
   // This function may be called from Loop.
   // returns  1 if entry is accepted.
   // returns -1 otherwise.
   return 1;
}
#endif // #ifdef plot_cxx
