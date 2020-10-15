#define plot_cxx
#include "plot.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>


void invokeStyle()
{
    //..BABAR style from RooLogon.C in workdir
    TStyle *babarStyle = new TStyle("BABAR", "BaBar approved plots style");

    // use plain black on white colors
    babarStyle->SetFrameBorderMode(0);
    babarStyle->SetCanvasBorderMode(0);
    babarStyle->SetPadBorderMode(0);
    babarStyle->SetPadColor(0);
    babarStyle->SetCanvasColor(0);
    babarStyle->SetStatColor(0);

    // set the paper & margin sizes
    babarStyle->SetPaperSize(20, 26);
    babarStyle->SetPadTopMargin(0.1);
    babarStyle->SetPadRightMargin(0.14);
    babarStyle->SetPadBottomMargin(0.1);
    babarStyle->SetPadLeftMargin(0.14);

    // use large Times-Roman fonts
    babarStyle->SetTextFont(132);
    babarStyle->SetTextSize(0.08);
    babarStyle->SetLabelFont(132, "x");
    babarStyle->SetLabelFont(132, "y");
    babarStyle->SetLabelFont(132, "z");
    babarStyle->SetLabelSize(0.05, "x");
    babarStyle->SetTitleSize(0.06, "x");
    babarStyle->SetLabelSize(0.05, "y");
    babarStyle->SetTitleSize(0.06, "y");
    babarStyle->SetLabelSize(0.05, "z");
    babarStyle->SetTitleSize(0.06, "z");

    // use bold lines and markers
    babarStyle->SetMarkerStyle(20);
    babarStyle->SetHistLineWidth(1.85);
    babarStyle->SetLineStyleString(2, "[12 12]"); // postscript dashes

    // get rid of X error bars and y error bar caps
    babarStyle->SetErrorX(0.001);

    // do not display any of the standard histogram decorations
    //babarStyle->SetOptTitle(0);
    babarStyle->SetOptStat(0);
    babarStyle->SetOptFit(0);

    // put tick marks on top and RHS of plots
    babarStyle->SetPadTickX(1);
    babarStyle->SetPadTickY(1);

    gROOT->SetStyle("BABAR");

    gROOT->ForceStyle();
    TH1::SetDefaultSumw2(kTRUE);
    TH2::SetDefaultSumw2(kTRUE);
}

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
   //invokeStyle();

   Long64_t nentries = fChain->GetEntriesFast();
   auto c = new TCanvas();
   c->Divide(2,1);
   TMultiGraph *grs = new TMultiGraph();
   TMultiGraph *gr45 = new TMultiGraph();
   auto g0 = new TGraph();
   auto g1 = new TGraph();
   auto g2 = new TGraph();
   auto g3 = new TGraph();
   auto g4 = new TGraph();
   auto g5 = new TGraph();
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
      g0->SetPoint(gentry, ttime->AsDouble(), HV);
      g1->SetPoint(gentry, ttime->AsDouble(), current);
      g2->SetPoint(gentry, ttime->AsDouble(), T0_Luminosity);
      g3->SetPoint(gentry, ttime->AsDouble(), Luminosity);
      g4->SetPoint(gentry, Luminosity, current);
      g5->SetPoint(gentry, T0_Luminosity, current);
      gentry++;
   }
   g0->Fit("pol0");
   cout << g0->GetFunction("pol0")->GetParameter(0) << endl;
   c->cd(1);
   TPad *pad1 = new TPad("pad1","",0,0,1,1);
   TPad *pad2 = new TPad("pad2","",0,0,1,1); 
   pad1->Draw();
   pad1->cd();  
   g1->SetMarkerStyle(20);
   g1->SetMarkerSize(0.5);
   g1->SetMarkerColorAlpha(kBlack, 1);
   
   g1->GetXaxis()->SetTimeOffset(0, "gmt");
   g1->GetXaxis()->SetTimeDisplay(1);
   g1->GetXaxis()->SetLabelOffset(0.02);
   g1->GetXaxis()->SetTimeFormat("%H\:%M");
   g1->GetXaxis()->SetTitle("");
   g1->GetYaxis()->SetTitle("Current (A)");
   g1->Draw("AP");
   
   pad2->SetFrameFillStyle(0);
   pad2->SetFillStyle(4000);
   pad2->Draw();  
   pad2->cd();  
   g2->SetMarkerStyle(20);
   g2->SetMarkerSize(0.5);
   g2->SetMarkerColorAlpha(kBlue, 0.4);
   g3->SetMarkerStyle(7);
   g3->SetMarkerSize(0.5);
   g3->SetMarkerColorAlpha(kGreen, 0.35);
   grs->Add(g2);
   grs->Add(g3);
   grs->SetTitle("CHANNEL_NAME");
   grs->GetXaxis()->SetTimeOffset(0, "gmt");
   grs->GetXaxis()->SetTimeDisplay(1);
   grs->GetXaxis()->SetLabelOffset(0.02);
   grs->GetXaxis()->SetTimeFormat("%H\:%M");
   grs->GetXaxis()->SetTitle("");
   grs->GetYaxis()->SetTitle("");
   grs->Draw("AP");
   
      // Draw the axis of the 2nd TMultiGraph
   TAxis *mg2Xaxis = grs->GetHistogram()->GetXaxis();
   TAxis *mg2Yaxis = grs->GetHistogram()->GetYaxis();
   Double_t xmin = mg2Xaxis->GetXmin();                       
   Double_t xmax = mg2Xaxis->GetXmax();
   Double_t ymin = mg2Yaxis->GetXmin();
   Double_t ymax = mg2Yaxis->GetXmax();
   mg2Xaxis->SetLabelSize(0);
   mg2Xaxis->SetTickLength(0);
   mg2Yaxis->SetLabelSize(0);
   mg2Yaxis->SetTickLength(0);
   TGaxis *yaxis = new TGaxis(xmax,ymin,xmax,ymax,ymin,ymax,510,"+L");   
   TGaxis *xaxis = new TGaxis(xmin,ymax,xmax,ymax,xmin,xmax,510,"-L");
   xaxis->SetLabelColor(kRed);   
   yaxis->SetTextColor(kRed);
   xaxis->SetTimeOffset(0, "gmt");
   xaxis->SetLabelOffset(0.005);
   xaxis->SetTimeFormat("%H\:%M");
   xaxis->SetTitle("");
   yaxis->SetTitle("Luminostiy (HZ/#mub)");   
   xaxis->Draw();  
   yaxis->SetLabelColor(kRed);
   yaxis->Draw();
   
   //Legend
   auto legend = new TLegend(0.55, 0.2,0.8,0.35);
   legend->SetHeader("small offset in times","C"); // option "C" allows to center the header
   legend->AddEntry(g1,"current","lp");
   legend->AddEntry(g2,"T0 Luminosity","lp");
   legend->AddEntry(g3,"ECal Luminosity","lp");
   legend->Draw();

   c->cd(2);
   g4->SetMarkerColorAlpha(kGreen, 0.2);
   g4->SetMarkerStyle(20);
   
   g5->SetMarkerColorAlpha(kBlue, 0.2);
   g5->SetMarkerStyle(22);
   
   gr45->Add(g4);
   gr45->Add(g5);
   gr45->GetXaxis()->SetTitle("Luminostiy (HZ/#mub)");
   gr45->GetYaxis()->SetTitle("Current (A)");
   gr45->SetTitle("Luminosity Current");
   gr45->Draw("AP");
    auto legend1 = new TLegend(0.55, 0.2,0.8,0.35);
   legend1->AddEntry(g4,"T0 Luminosity","lp");
   legend1->AddEntry(g5,"ECal Luminosity","lp");
   legend1->Draw();
}
