#include <iostream>
#include <numeric>

void gettimes(string filename)
{
    gROOT->SetStyle("Pub");
    // Declaration of leaf types
    Double_t bunch1[3564];
    Double_t bunch2[3564];
    Double_t v0rate[3564];
    Double_t time;
    Double_t nsep;
    Int_t plane;
    Int_t aqflag;

    // List of branches
    TBranch *b_time; //!
    TBranch *b_bunch1;
    TBranch *b_bunch2;
    TBranch *b_nsep;
    TBranch *b_plane;
    TBranch *b_aqflag;
    TBranch *b_v0rate; //!

    TFile *f1 = TFile::Open(filename.c_str(), "READ");
    TTree *tree1 = new TTree();
    tree1 = (TTree *)f1->Get("VdM");
    tree1->SetMakeClass(1);
    tree1->SetBranchAddress("bunch1", &bunch1, &b_bunch1);
    tree1->SetBranchAddress("bunch2", &bunch2, &b_bunch2);
    tree1->SetBranchAddress("time", &time, &b_time);
    tree1->SetBranchAddress("nsep", &nsep, &b_nsep);
    tree1->SetBranchAddress("plane", &plane, &b_plane);
    tree1->SetBranchAddress("aqflag", &aqflag, &b_aqflag);
    tree1->SetBranchAddress("v0rate", &v0rate, &b_v0rate);

    Double_t N_1 = 0.0;
    Double_t N_2 = 0.0;
    Double_t v0rate_sum = 0.0;

    auto g = new TGraph();
    g->SetMarkerStyle(4);
    g->SetMarkerSize(1);
    g->SetMarkerColor(2);

    //__________________________________________________________16418
    Long64_t nentries = tree1->GetEntries();
    Long64_t gentry = 0;
    for (Long64_t jentry = 0; jentry < nentries; jentry++)
    {
        N_1 = 0.0;
        N_2 = 0.0;
        v0rate_sum = 0.0;
        //cout << "This entry in tree1:\t" << jentry << endl;
        tree1->GetEntry(jentry);
        //if (aqflag)
        //    continue;
        v0rate_sum = accumulate(v0rate, v0rate + 3564, v0rate_sum);
        //N_1 = accumulate(bunch1, bunch1 + 3564, N_1);
        //N_2 = accumulate(bunch2, bunch2 + 3564, N_2);
        g->SetPoint(gentry, time, v0rate_sum);
        gentry++;
    }
    g->GetYaxis()->SetTitle("V0 rate");
    g->GetXaxis()->LabelsOption("v");
    auto c = new TCanvas();
    g->Draw("AP");
}