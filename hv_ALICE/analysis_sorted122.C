#define analysis_sorted122_cxx
#include "analysis_sorted122.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>

void analysis_sorted122::Loop()
{
   //   In a ROOT session, you can do:
   //      root> .L analysis_sorted2.C
   //      root> analysis_sorted2 t
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
   auto c = new TCanvas("c", "Offset ", 1600, 900);
   // Called inside Loop
   auto g0 = new TGraph();
   TTimeStamp *ttime = new TTimeStamp();

   Long64_t nentries = fChain->GetEntriesFast();
   Long64_t nbytes = 0, nb = 0;
   Long64_t gentry = 0;
   for (Long64_t jentry = 0; jentry < nentries; jentry++)
   {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0)
         break;
      nb = fChain->GetEntry(jentry);
      nbytes += nb;
      if (fSec < 1534797124)
      {
         continue;
      }
      if (fSec > 1534833676)
         break;
      ttime->SetSec(fSec);
      ttime->SetNanoSec(fNanoSec);
      g0->SetPoint(gentry, ttime->AsDouble(), HV);
      gentry++;
   }
   g0->GetXaxis()->SetTimeOffset(0, "gmt");
   g0->GetXaxis()->SetTimeDisplay(1);
   g0->GetXaxis()->SetLabelOffset(0.02);
   g0->GetXaxis()->SetTimeFormat("#splitline{%Y}{#splitline{%d\/%m}{%H\:%M}}");
   g0->GetYaxis()->SetTitle("offset current (#muA)");
   g0->Fit("pol0", "Q");
   g0->Draw("AP");
}
