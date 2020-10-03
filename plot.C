#define plot_cxx
#include "plot.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>

void plot::Loop()
{
   //   In a ROOT session, you can do:
   //      root> .L plot.C
   //      root> plot t
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

   Long64_t nentries = fChain->GetEntriesFast();
   TMultiGraph *grs = new TMultiGraph();
   auto g1 = new TGraph();
   auto g2 = new TGraph();
   auto g3 = new TGraph();
   Long64_t gentry = 0;
   TTimeStamp *ttime = new TTimeStamp();
   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry = 0; jentry < nentries; jentry++)
   {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0)
         break;
      nb = fChain->GetEntry(jentry);
      nbytes += nb;
      // if (Cut(ientry) < 0) continue;
      ttime->SetSec(fSec);
      ttime->SetNanoSec(fNanoSec);
      g1->SetPoint(gentry, ttime->AsDouble(), current);
      g2->SetPoint(gentry, ttime->AsDouble(), T0_Luminosity);
      g3->SetPoint(gentry, ttime->AsDouble(), Luminosity);
      gentry++;
   }
   g1->SetMarkerStyle(8);
   g1->SetMarkerSize(1);
   g1->SetMarkerColorAlpha(kRed, 1);
   g2->SetMarkerStyle(20);
   g2->SetMarkerSize(0.5);
   g2->SetMarkerColorAlpha(kBlue, 0.4);
   g3->SetMarkerStyle(7);
   g3->SetMarkerSize(0.5);
   g3->SetMarkerColorAlpha(kGreen, 0.35);
   grs->Add(g1);
   grs->Add(g2);
   grs->Add(g3);
   grs->SetTitle("CHANNEL_NAME");
   grs->GetXaxis()->SetTimeOffset(0, "gmt");
   grs->GetXaxis()->SetTimeDisplay(1);
   grs->GetXaxis()->SetLabelOffset(0.02);
   grs->GetXaxis()->SetTimeFormat("#splitline{%Y}{#splitline{%d\/%m}{%H\:%M\:%S}}");
   grs->GetXaxis()->SetTitle("Time");
   grs->GetYaxis()->SetTitle("HV [V]");
   grs->Draw("AP");
}
