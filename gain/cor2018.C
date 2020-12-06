#define cor_cxx
#include "cor2018.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>

void cor::Loop()
{
   //   In a ROOT session, you can do:
   //      root> .L cor.C
   //      root> cor t
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

   //gROOT->SetStyle("Pub");
   //gROOT->ForceStyle();

   auto h_p_g = new TH2F("h_p_g", "Gain vs. Pressure; Pressure (hPa); Gain (arb. u.)", 100, 967, 974, 100, 0.35, 0.6);
   auto h1 = new TH1F("h1", "Gain;Gain (arb. u.);entries", 100, 0.35, 0.6);
   auto h2 = new TH1F("h2", "Gain;Gain (arb. u.);entries", 100, 0.35, 0.6);
   auto h3 = new TH1F("h3", "Gain;Gain (arb. u.);entries", 100, 0.35, 0.6);
   auto h4 = new TH1F("h4", "Gain;Gain (arb. u.);entries", 100, 0.35, 0.6);
   h1->SetTitle("run 292274");
   h2->SetTitle("run 292430");
   h3->SetTitle("run 292524");
   h4->SetTitle("run 292693");

   Double_t n2_1_i, n2_2_i, n2_3_i, n2_4_i;
   Double_t p1, p2, p3, p4;
   Double_t c02_1, c02_2, c02_3, c02_4;
   Double_t xe1, xe2, xe3, xe4;

   Long64_t nentries = fChain->GetEntriesFast();

   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry = 0; jentry < nentries; jentry++)
   {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0)
         break;
      nb = fChain->GetEntry(jentry);
      nbytes += nb;
      if (goodchamber)
      {
         if (run == 292274)
         {
            h1->Fill(gain);
            n2_1_i = N2;
            p1 = pressure;
            c02_1 = C02;
            xe1 = Xe;
         }
         if (run == 292430)
         {
            h2->Fill(gain);
            n2_2_i = N2;
            p2 = pressure;
            c02_2 = C02;
            xe2 = Xe;
         }
         if (run == 292524)
         {
            h3->Fill(gain);
            n2_3_i = N2;
            p3 = pressure;
            c02_3 = C02;
            xe3 = Xe;
         }
         if (run == 292693)
         {
            h4->Fill(gain);
            n2_4_i = N2;
            p4 = pressure;
            c02_4 = C02;
            xe4 = Xe;
         }
         h_p_g->Fill(pressure, gain);
      }
   }

   TLatex *tex = new TLatex();
   tex->SetNDC(kTRUE);
   tex->SetTextSize(0.05);
   tex->SetTextColor(kBlack);
   char n2_1[100],
       n2_2[100],
       n2_3[100],
       n2_4[100],
       p1_[100],
       p2_[100],
       p3_[100],
       p4_[100],
       c02_1_[100], c02_2_[100], c02_3_[100], c02_4_[100],
       xe1_[100], xe2_[100], xe3_[100], xe4_[100];
   sprintf(n2_1, "N_{2} = %.3f ppm", n2_1_i);
   sprintf(n2_2, "N_{2} = %.3f ppm", n2_2_i);
   sprintf(n2_3, "N_{2} = %.3f ppm", n2_3_i);
   sprintf(n2_4, "N_{2} = %.3f ppm", n2_4_i);
   sprintf(p1_, "p = %.2f hPa", p1);
   sprintf(p2_, "p = %.2f hPa", p2);
   sprintf(p3_, "p = %.2f hPa", p3);
   sprintf(p4_, "p = %.2f hPa", p4);
   sprintf(c02_1_, "C02 = %.2f %%", c02_1);
   sprintf(c02_2_, "C02 = %.2f %%", c02_2);
   sprintf(c02_3_, "C02 = %.2f %%", c02_3);
   sprintf(c02_4_, "C02 = %.2f %%", c02_4);
   sprintf(xe1_, "Xe = %.2f %%", xe1);
   sprintf(xe2_, "Xe = %.2f %%", xe2);
   sprintf(xe3_, "Xe = %.2f %%", xe3);
   sprintf(xe4_, "Xe = %.2f %%", xe4);

   auto c = new TCanvas();
   c->Divide(2, 2);
   c->cd(1);
   h1->Draw();
   tex->DrawLatex(0.15, 0.85, n2_1);
   tex->DrawLatex(0.15, 0.78, p1_);
   tex->DrawLatex(0.15, 0.71, c02_1_);
   tex->DrawLatex(0.15, 0.64, xe1_);

   c->cd(2);
   h2->Draw();
   tex->DrawLatex(0.15, 0.85, n2_2);
   tex->DrawLatex(0.15, 0.78, p2_);
   tex->DrawLatex(0.15, 0.71, c02_2_);
   tex->DrawLatex(0.15, 0.64, xe2_);

   c->cd(3);
   h3->Draw();
   tex->DrawLatex(0.15, 0.85, n2_3);
   tex->DrawLatex(0.15, 0.78, p3_);
   tex->DrawLatex(0.15, 0.71, c02_3_);
   tex->DrawLatex(0.15, 0.64, xe3_);

   c->cd(4);
   h4->Draw();
   tex->DrawLatex(0.15, 0.85, n2_4);
   tex->DrawLatex(0.15, 0.78, p4_);
   tex->DrawLatex(0.15, 0.71, c02_4_);
   tex->DrawLatex(0.15, 0.64, xe4_);

   c->SaveAs("gain_2018.pdf");

   auto c1 = new TCanvas();
   h_p_g->Draw();
   c1->SaveAs("gain_pressure_2018.pdf");
}
