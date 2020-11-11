#define analysis_sorted2_cxx
#include "analysis_sorted2.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>

void analysis_sorted2::Loop()
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
   if (fChain == 0)
      return;

   Long64_t nentries = fChain->GetEntriesFast();
   //auto myfile = new TFile("output.root", "UPDATE");
   auto g = new TGraph();
   TTimeStamp *ttime = new TTimeStamp();
   auto c0 = new TCanvas("c0", "Kanal 06_0_0A", 10, 10, 1000, 1000);
   c0->cd();
   Long64_t gentry = 0;
   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry = 0; jentry < nentries; jentry++)
   {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0)
         break;
      nb = fChain->GetEntry(jentry);
      nbytes += nb;
      // if (Cut(ientry) < 0) continue;
      if (HV < 0)
         continue;
      //if (fSec < 1504399786 || fSec > 1504401590)
      if (fSec < 1530316800)
      {
         continue;
      }
      if (fSec > 1530403200)
         break;
      ttime->SetSec(fSec);
      ttime->SetNanoSec(fNanoSec);
      g->SetPoint(gentry, ttime->AsDouble(), HV);
      gentry++;
   }
   g->Draw();
   g->SetTitle("Kanal 06_0_0A");
   //g->GetXaxis()->SetRangeUser(1.5184e9, 1.5444e9);
   g->GetXaxis()->SetTimeOffset(0, "gmt");
   g->GetXaxis()->SetTimeDisplay(1);
   g->GetXaxis()->SetLabelOffset(0.02);
   g->GetXaxis()->SetTimeFormat("#splitline{%Y}{#splitline{%d\/%m}{%H\:%M\:%S}}");
   g->GetXaxis()->SetTitle("Time");
   g->GetYaxis()->SetTitle("Current [#muA]");
   g->SetMarkerStyle(8);
   g->SetMarkerSize(0.5);
   g->SetMarkerColorAlpha(kRed, 0.35);
   g->Draw("AP");
   c0->Draw();
   //c0->Write();
   //myfile->Close();
}
