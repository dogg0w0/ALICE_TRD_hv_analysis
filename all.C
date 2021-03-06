#define all_cxx
#include "all.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>

void all::Loop()
{
   //   In a ROOT session, you can do:
   //      root> .L all.C
   //      root> all t
   //      root> t.GetEntry(12); // Fill t data members with entry number 12
   //      root> t.Show();       // Show values of entry 12
   //      root> t.Show(16);     // Read and show values of entry 16
   //      root> t.Loop();       // Loop on all entries
   //

   //     This is the loop skeleton where:
   //    jentry is the global entry number in the chain
   //    ientry is the entry number in the mean_current Tree
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
   gROOT->SetStyle("Pub");
   if (fChain == 0)
      return;
   //invokeStyle();
   //auto style = gROOT->GetStyle("Pub");
   //style->SetOptTitle(1);
   //style->SetPadTopMargin(0.1);
   //gROOT->SetStyle("Pub");

   Double_t delta = 0.3;

   Long64_t nentries = fChain->GetEntriesFast();
   auto c = new TCanvas();
   c->Divide(2, 1);
   auto *grs = new TMultiGraph();
   auto *gr45 = new TMultiGraph();
   auto g0 = new TGraph();
   auto g1 = new TGraph();
   auto g2 = new TGraph();
   auto g3 = new TGraph();
   auto g4 = new TGraphErrors();
   auto g5 = new TGraphErrors();
   auto g6 = new TGraph();
   auto gT0Errors = new TGraph();
   auto gECALErrors = new TGraph();
   auto dummyhist = new TH1D();
   auto dummygraphT0 = new TGraphErrors();
   auto dummygraphECAL = new TGraphErrors();
   auto current_tof_cur = new TGraph();
   auto press = new TGraph();

   //Bool_t markT0[nentries],
   //      markECAL[nentries];
   Long64_t gentry = 0, gerrorsentryT0 = 0, gerrorsentryECAL = 0, g4entry = 0, g5entry = 0;
   TTimeStamp *ttime = new TTimeStamp();
   for (Long64_t jentry = 0; jentry < nentries; jentry++)
   {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0)
         break;
      fChain->GetEntry(jentry);
      if (HV < 0)
         continue;
      // if (Cut(ientry) < 0) continue;
      dummygraphT0->SetPoint(gentry, Luminosity, mean_current);
      dummygraphECAL->SetPoint(gentry, T0_Luminosity, mean_current);
      gentry++;
   }
   dummygraphT0->SetPoint(gentry, 0, 0);
   dummygraphECAL->SetPoint(gentry, 0, 0);

   // Mark Outliers
   dummygraphT0->Fit("pol1", "Q");
   dummygraphECAL->Fit("pol1", "Q");
   auto fT0 = dummygraphT0->GetFunction("pol1");
   auto fECAL = dummygraphECAL->GetFunction("pol1");
   Double_t maxLumi = 0;

   gentry = 0;
   for (Long64_t jentry = 0; jentry < nentries; jentry++)
   {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0)
         break;
      fChain->GetEntry(jentry);
      // if (Cut(ientry) < 0) continue;

      ttime->SetSec(fSec);
      ttime->SetNanoSec(fNanoSec);
      if (TMath::Abs(fECAL->Eval(Luminosity) - mean_current) < delta)
      {
         g4->SetPoint(g4entry, Luminosity, mean_current);
         g4->SetPointError(g4entry, 0.01, TMath::Sqrt(std_current));
         g4entry++;
      }
      else
      {
         gECALErrors->SetPoint(gerrorsentryECAL, Luminosity, mean_current);
         gerrorsentryECAL++;
      }

      if (TMath::Abs(fT0->Eval(T0_Luminosity) - mean_current) < delta)
      {
         g5->SetPoint(g5entry, T0_Luminosity, mean_current);
         g5->SetPointError(g5entry,0.01, TMath::Sqrt(std_current));
         g5entry++;
      }
      else
      {
         gT0Errors->SetPoint(gerrorsentryT0, T0_Luminosity, mean_current);
         gerrorsentryT0++;
      }
      if (TMath::Abs(fT0->Eval(T0_Luminosity) - mean_current) < delta && TMath::Abs(fECAL->Eval(Luminosity) - mean_current) < delta)
      {
         g0->SetPoint(gentry, ttime->AsDouble(), HV);
         g1->SetPoint(gentry, ttime->AsDouble(), mean_current);
         g2->SetPoint(gentry, ttime->AsDouble(), T0_Luminosity);
         g3->SetPoint(gentry, ttime->AsDouble(), Luminosity);
         g6->SetPoint(gentry, T0_Luminosity, Luminosity);
         current_tof_cur->SetPoint(gentry, mean_current, TOF_average_current);
         press->SetPoint(gentry, pressure, mean_current);
         gentry++;
         if (maxLumi < Luminosity)
            maxLumi = Luminosity;
      }
   }
   g4->SetPoint(gentry, 0, 0);
   g5->SetPoint(gentry, 0, 0);

   c->cd(1);
   dummyhist->Draw();
   //TPad *pad1 = new TPad("pad1","",0,0,1,1);
   //TPad *pad2 = new TPad("pad2","",0,0,1,1);
   //pad1->Draw();
   //pad1->cd();

   //pad2->SetFrameFillStyle(0);
   //pad2->SetFillStyle(4000);
   //pad2->Draw();
   //pad2->cd();
   g2->SetMarkerStyle(20);
   g2->SetMarkerSize(0.7);
   g2->SetMarkerColorAlpha(8, 1);
   g3->SetMarkerStyle(21);
   g3->SetMarkerSize(0.7);
   g3->SetMarkerColorAlpha(9, 1);
   //grs->Add(g2);
   grs->Add(g3);
   grs->SetTitle("All");
   grs->GetXaxis()->SetTimeOffset(0, "gmt");
   grs->GetXaxis()->SetTimeDisplay(1);
   grs->GetXaxis()->SetLabelOffset(0.02);
   grs->GetXaxis()->SetTimeFormat("#splitline{%H}{%M}");
   grs->GetXaxis()->SetTitle("");
   grs->GetYaxis()->SetTitle("Luminostiy (HZ/#mub)  #color[2]{Current (#muA)}");
   grs->Draw("AP SAME");

   g1->SetMarkerStyle(2);
   g1->SetMarkerSize(0.7);
   g1->SetMarkerColorAlpha(kRed, 1);

   g1->GetXaxis()->SetTimeOffset(0, "gmt");
   g1->GetXaxis()->SetTimeDisplay(1);
   g1->GetXaxis()->SetLabelOffset(0.03);
   g1->GetXaxis()->SetTimeFormat("%H\:%M");
   //g1->GetXaxis()->SetTitle("");
   //g1->GetYaxis()->SetTitle("Current (#muA)");
   g1->Draw("P SAME");

   //Legend
   auto legend = new TLegend(0.55, 0.3, 0.8, 0.45);
   //legend->SetHeader("small offset in times","C"); // option "C" allows to center the header
   legend->AddEntry(g1, "mean_current", "p");
   //legend->AddEntry(g2, "T0 Luminosity", "p");
   legend->AddEntry(g3, "ECal Luminosity", "p");
   legend->Draw();

   c->cd(2);
   g4->SetMarkerColorAlpha(8, 0.2);
   g4->SetMarkerSize(0.5);
   g4->SetMarkerStyle(20);

   g5->SetMarkerColorAlpha(9, 0.2);
   g5->SetMarkerSize(0.5);
   g5->SetMarkerStyle(21);

   g4->Fit("pol1");
   //g5->Fit("pol1");
   g4->GetFunction("pol1")->SetLineColor(8);
   //g5->GetFunction("pol1")->SetLineColor(9);

   gT0Errors->SetMarkerColorAlpha(kRed, 0.2);
   gT0Errors->SetMarkerSize(0.3);
   gT0Errors->SetMarkerStyle(3);

   gECALErrors->SetMarkerColorAlpha(kRed, 0.2);
   gECALErrors->SetMarkerSize(0.3);
   gECALErrors->SetMarkerStyle(4);

   //gr45->Add(g4);
   //gr45->Add(g5);
   //gr45->Add(gT0Errors);
   //gr45->Add(gECALErrors);
   g4->SetTitle("Luminosity Current Correlation");
   g4->GetXaxis()->SetTitle("Luminostiy (HZ/#mub)");
   g4->GetYaxis()->SetTitle("Current (#muA)");
   g4->Draw("AP");
   gECALErrors->Draw("P SAME");
   auto legend1 = new TLegend(0.55, 0.3, 0.8, 0.45);
   legend1->AddEntry(g4, "ECal Luminosity", "p");
   //legend1->AddEntry(g5, "T0 Luminosity", "p");
   //legend1->AddEntry(gT0Errors, "T0 Errors", "p");
   legend1->AddEntry(gECALErrors, "ECal Errors", "p");
   legend1->Draw();

   TF1 *f1 = (TF1 *)g4->GetFunction("pol1");
   TLatex *tex = new TLatex();
   tex->SetNDC(kTRUE);
   tex->SetTextSize(0.035);
   tex->SetTextColor(kBlack);
   char buffer_M[100],
       buffer_S[100],
       buffer_Chi[100];
   tex->DrawLatex(0.25, 0.85, "Fit ECAL");
   sprintf(buffer_M, "p0 = %.3f #pm %.3f (#muA)", f1->GetParameter(0), f1->GetParError(0));
   sprintf(buffer_S, "p1 = %.3f #pm %.3f (#muA/(Hz/#mub))", f1->GetParameter(1), f1->GetParError(1));
   sprintf(buffer_Chi, "#chi^{2}_{red} = %.3f / %d", f1->GetChisquare(), f1->GetNDF());
   tex->DrawLatex(0.25, 0.80, buffer_M);
   tex->DrawLatex(0.25, 0.75, buffer_S);
   tex->DrawLatex(0.25, 0.70, buffer_Chi);
   tex->Draw();
}
