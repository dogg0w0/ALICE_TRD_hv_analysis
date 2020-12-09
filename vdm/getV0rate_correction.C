#include <iostream>
#include <numeric>

void getV0rate_correction(string filename)
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

    auto g2 = new TGraph();
    g2->SetMarkerStyle(4);
    g2->SetMarkerSize(1);
    g2->SetMarkerColor(2);

    auto gun = new TGraph();
    gun->SetMarkerStyle(4);
    gun->SetMarkerSize(1);
    gun->SetMarkerColor(1);

    auto mg = new TMultiGraph();

    //----------------------------------------------------
    Int_t start_time = 1501171600;
    Int_t end_time = 1501172600;

    //Int_t start_time = 1501172800;
    //Int_t end_time = 1501173800;

    Int_t time1 = 1501171100, time2 = 1501171470,
          time4 = 1501172740, time3 = 1501172630,
          time6 = 1501174000, time5 = 1501173900;

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
        if ((time >= time1 and time <= time2) or (time >= time3 and time <= time4) or (time >= time5 and time <= time6))
        {
            v0rate_sum = accumulate(v0rate, v0rate + 3564, v0rate_sum);
            //N_1 = accumulate(bunch1, bunch1 + 3564, N_1);
            //N_2 = accumulate(bunch2, bunch2 + 3564, N_2);
            g->SetPoint(gentry, time, v0rate_sum);
            gentry++;
        }
    }

    g->Fit("pol1", "");
    TF1 *pol1 = g->GetFunction("pol1");

    gentry = 0;
    Double_t v0rate_sum_cor = 0.0;
    for (Long64_t jentry = 0; jentry < nentries; jentry++)
    {
        N_1 = 0.0;
        N_2 = 0.0;
        v0rate_sum = 0.0;
        //cout << "This entry in tree1:\t" << jentry << endl;
        tree1->GetEntry(jentry);
        if (time >= 1501174150)
        {
            break;
        }
        v0rate_sum = accumulate(v0rate, v0rate + 3564, v0rate_sum);
        //N_1 = accumulate(bunch1, bunch1 + 3564, N_1);
        //N_2 = accumulate(bunch2, bunch2 + 3564, N_2);
        //cout << -expo->GetParameter(1) * time << endl;
        v0rate_sum_cor = v0rate_sum + pol1->GetParameter(1) * (1501171000-time);
        g2->SetPoint(gentry, time, v0rate_sum_cor);
        gun->SetPoint(gentry, time, v0rate_sum);
        gentry++;
    }
    mg->Add(g2);
    mg->Add(gun);
    mg->GetXaxis()->SetTimeOffset(0, "gmt");
    mg->GetXaxis()->SetTimeDisplay(1);
    mg->GetXaxis()->SetLabelOffset(0.02);
    mg->GetXaxis()->SetTimeFormat("#splitline{%H}{%M}");
    g->GetXaxis()->SetTimeOffset(0, "gmt");
    g->GetXaxis()->SetTimeDisplay(1);
    g->GetXaxis()->SetLabelOffset(0.02);
    g->GetXaxis()->SetTimeFormat("#splitline{%H}{%M}");
    g->GetYaxis()->SetTitle("V0 rate");
    mg->GetYaxis()->SetTitle("V0 rate");
    auto c = new TCanvas();
    c->Divide(2, 1);
    c->cd(1);
    g->Draw("AP");
    c->cd(2);
    mg->Draw("AP");
}