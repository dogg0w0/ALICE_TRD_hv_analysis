#include <iostream>
#include <numeric>

void drawdecay(string filename)
{
    gROOT->SetStyle("Pub");
    // Declaration of leaf types
    Double_t v0rate[3564];
    Double_t v0tot_setv0;
    Double_t time;
    Double_t nsep;
    Int_t plane;
    Int_t aqflag;

    // List of branches
    TBranch *b_v0rate; //!
    TBranch *b_time;   //!
    TBranch *b_v0tot_setv0;
    TBranch *b_nsep;
    TBranch *b_plane;
    TBranch *b_aqflag;

    TFile *f1 = TFile::Open(filename.c_str(), "READ");
    TTree *tree1 = new TTree();
    tree1 = (TTree *)f1->Get("VdM");
    tree1->SetMakeClass(1);
    tree1->SetBranchAddress("v0rate", &v0rate, &b_v0rate);
    tree1->SetBranchAddress("time", &time, &b_time);
    tree1->SetBranchAddress("v0tot_setv0", &v0tot_setv0, &b_v0tot_setv0);
    tree1->SetBranchAddress("nsep", &nsep, &b_nsep);
    tree1->SetBranchAddress("plane", &plane, &b_plane);
    tree1->SetBranchAddress("aqflag", &aqflag, &b_aqflag);

    Double_t v0rate_sum = 0.0;

    auto g = new TGraph();
    g->SetMarkerStyle(8);
    g->SetMarkerSize(.5);
    g->SetMarkerColor(1);

    //----------------------------------------------------

    //_____________________________________________________________
    //__________________________________________________________
    Long64_t nentries = tree1->GetEntries();
    Double_t v0rate_sum_cor = 0.0;
    Long64_t gentry = 0;
    for (Long64_t jentry = 0; jentry < nentries; jentry++)
    {
        v0rate_sum = 0.0;
        tree1->GetEntry(jentry);
        if (aqflag)
            continue;
        v0rate_sum = accumulate(v0rate, v0rate + 3564, v0rate_sum);
        v0rate_sum_cor = v0rate_sum;
        g->SetPoint(gentry, time, v0rate_sum_cor);
        gentry++;
    }

    auto c = new TCanvas();
    c->cd();
    g->GetXaxis()->SetTitle("");
    g->GetYaxis()->SetTitle("V0TOT trigger rate");
    g->GetXaxis()->SetTimeOffset(0, "gmt");
    g->GetXaxis()->SetTimeDisplay(1);
    g->GetXaxis()->SetLabelOffset(0.02);
    g->GetXaxis()->SetTimeFormat("#splitline{%Y}{#splitline{%d\/%m}{%H\:%M}}");
    g->Draw("AP");

}