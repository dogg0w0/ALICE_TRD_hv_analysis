#define readroot_csv_cxx
#include "readroot_csv.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <iostream>
#include <fstream>

void readroot_csv::Loop(Int_t start_time, Int_t end_time)
{
   //   In a ROOT session, you can do:
   //      root> .L readroot_csv.C
   //      root> readroot_csv t
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
   if (fChain == 0)
      return;

   // create outfile
   std::ofstream out;
   out.open(outfile);
   out << "fSec"
       << ","
       << "fNanoSec"
       << ","
       << "HV" << std::endl;

   Long64_t nentries = fChain->GetEntriesFast();
   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry = 0; jentry < nentries; jentry++)
   {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0)
         break;
      nb = fChain->GetEntry(jentry);
      nbytes += nb;
      if (fSec < start_time)
      {
         continue;
      }
      if (fSec > end_time)
         break;

      out << fSec << "," << fNanoSec << "," << HV << std::endl;
   }
   out.close();
}

int main(int argc, char const *argv[])
{
   readroot_csv t((std::string)argv[1], (std::string)argv[2]);
   t.Loop(std::stoi(argv[3]), std::stoi(argv[4]));
   return 0;
}
