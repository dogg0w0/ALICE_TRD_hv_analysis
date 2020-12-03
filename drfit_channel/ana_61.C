#define ana_61_cxx
#include "ana_61.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>

void ana_61::Loop()
{
   //   In a ROOT session, you can do:
   //      root> .L ana_61.C
   //      root> ana_61 t
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
   auto c = new TCanvas();
   c->Divide(2, 2);
   auto gI = new TGraph();
   auto gL = new TGraph();
   auto gIL = new TGraph();
   auto gH = new TGraph();
   if (fChain == 0)
      return;

   Long64_t nentries = fChain->GetEntriesFast();
   TTimeStamp ttime(0, 0);
   Int_t gentry = 0;
   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry = 0; jentry < nentries; jentry++)
   {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0)
         break;
      nb = fChain->GetEntry(jentry);
      nbytes += nb;
      // if (Cut(ientry) < 0) continue;
      if (Luminosity < 60 && current <166.5)
      {
         ttime.SetSec(fSec);
         ttime.SetNanoSec(fNanoSec);
         gI->SetPoint(gentry, ttime.AsDouble(), current);
         gH->SetPoint(gentry, ttime.AsDouble(), HV);
         gL->SetPoint(gentry, ttime.AsDouble(), Luminosity);
         gIL->SetPoint(gentry, Luminosity, current);
         gentry++;
      }
   }
   gI->GetXaxis()->SetTimeOffset(0, "gmt");
   gI->GetXaxis()->SetTimeDisplay(1);
   gI->GetXaxis()->SetLabelOffset(0.02);
   gI->GetXaxis()->SetTimeFormat("#splitline{%Y}{#splitline{%d\/%m}{%H\:%M\:%S}}");
   gI->GetXaxis()->SetTitle("Time");
   gI->GetYaxis()->SetTitle("Current (#muA)");
   gI->SetMarkerStyle(8);
   gI->SetMarkerSize(0.5);
   gI->SetMarkerColorAlpha(kRed, 0.35);

   gH->GetXaxis()->SetTimeOffset(0, "gmt");
   gH->GetXaxis()->SetTimeDisplay(1);
   gH->GetXaxis()->SetLabelOffset(0.02);
   gH->GetXaxis()->SetTimeFormat("#splitline{%Y}{#splitline{%d\/%m}{%H\:%M\:%S}}");
   gH->GetXaxis()->SetTitle("Time");
   gH->GetYaxis()->SetTitle("HV (V)");
   gH->SetMarkerStyle(8);
   gH->SetMarkerSize(0.5);
   gH->SetMarkerColorAlpha(kRed, 0.35);

   gL->GetXaxis()->SetTimeOffset(0, "gmt");
   gL->GetXaxis()->SetTimeDisplay(1);
   gL->GetXaxis()->SetLabelOffset(0.02);
   gL->GetXaxis()->SetTimeFormat("#splitline{%Y}{#splitline{%d\/%m}{%H\:%M\:%S}}");
   gL->GetXaxis()->SetTitle("Time");
   gL->GetYaxis()->SetTitle("Luminosity (Hz/#mub)");
   gL->SetMarkerStyle(8);
   gL->SetMarkerSize(0.5);
   gL->SetMarkerColorAlpha(kRed, 0.35);

   gIL->GetXaxis()->SetTitle("Luminosity (Hz/#mub)");
   gIL->GetYaxis()->SetTitle("Current (#muA)");
   gIL->SetMarkerStyle(8);
   gIL->SetMarkerSize(0.5);
   gIL->SetMarkerColorAlpha(kRed, 0.35);

   c->cd(1);
   gI->Draw("AP");
   c->cd(2);
   gH->Draw("AP");
   c->cd(3);
   gL->Draw("AP");
   c->cd(4);
   gIL->Draw("AP");
}
