#include <iostream>
#include <numeric>

void getv0root(string filename, string corAnodefile)
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

    auto g = new TGraphErrors();
    g->SetMarkerStyle(4);
    g->SetMarkerSize(1);
    g->SetMarkerColor(2);

    //----------------------------------------------------
    // Declaration of leaf types
    Double_t HV;
    //TTimeStamp      *time;
    Int_t fSec;
    Int_t fNanoSec;

    // List of branches
    TBranch *b_HV;            //!
    TBranch *b_time_fSec;     //!
    TBranch *b_time_fNanoSec; //!

    TTimeStamp *ttime = new TTimeStamp();

    TFile *f2 = TFile::Open(corAnodefile.c_str(), "READ");
    TTree *tree2 = new TTree();
    tree2 = (TTree *)f2->Get("Tree_TRD_HV_cor");
    tree2->SetMakeClass(1);
    tree2->SetBranchAddress("HV", &HV, &b_HV);
    tree2->SetBranchAddress("fSec", &fSec, &b_time_fSec);
    tree2->SetBranchAddress("fNanoSec", &fNanoSec, &b_time_fNanoSec);
    auto g2 = new TGraphErrors();
    g2->SetMarkerStyle(4);
    g2->SetMarkerSize(1);
    g2->SetMarkerColor(2);
    Int_t g2entry = 0;

    //_____________________________________________________________
     //Int_t start_time = 1501171600;
     //Int_t end_time = 1501172600;

    Int_t start_time = 1501172800;
    Int_t end_time = 1501173800;

    //__________________________________________________________
    Long64_t nentries = tree1->GetEntries();
    Double_t v0rate_sum_cor = 0.0;

    Long64_t nentries2 = tree2->GetEntries();
    Long64_t gentry = 0;
    for (Long64_t jentry = 0; jentry < nentries; jentry++)
    {
        v0rate_sum = 0.0;
        tree1->GetEntry(jentry);
        if (aqflag)
            continue;
        if (time <= start_time)
        {
            continue;
        }
        if (time >= end_time)
        {
            break;
        }
        if (plane != 2)
        {
            continue;
        }
        if (kTRUE)
        {
            for (Long64_t ientry = 0; ientry < nentries2; ientry++)
            {
                tree2->GetEntry(ientry);
                if (fSec <= start_time)
                {
                    continue;
                }
                if (fSec >= end_time)
                {
                    break;
                }
                ttime->SetSec(fSec);
                ttime->SetNanoSec(fNanoSec);
                if (abs(time - ttime->AsDouble()) <= 1.0)
                {
                    g2->SetPoint(g2entry, nsep, HV);
                    //g2->SetPointError(g2entry, 0.0, HV*1e-5);
                    g2entry++;
                }
            }
        }
        v0rate_sum = accumulate(v0rate, v0rate + 3564, v0rate_sum);
        v0rate_sum_cor = v0rate_sum  -0.150456 * (1501171000 - time);
        g->SetPoint(gentry, nsep, v0rate_sum_cor / 10000);
        //g->SetPointError(gentry, 0.0, v0rate_sum_cor * 1e-5 / 10000);
        gentry++;
    }

    TLatex *tex = new TLatex();
    tex->SetNDC(kTRUE);
    tex->SetTextSize(0.035);
    tex->SetTextColor(kBlack);
    char buffer_mu[100],
        buffer_S[100],
        buffer_Chi[100],
        buffer_r[100],
        buffer_h[100];
    g->Fit("gaus", "Q");
    g2->Fit("gaus", "Q");

    auto c = new TCanvas();
    c->Divide(2, 1);
    c->cd(1);
    g->GetXaxis()->SetTitle("nominal separation (#Deltay)");
    g->GetYaxis()->SetTitle("V0TOT trigger rate/10000");
    g->Draw("AP");

    TF1 *fgauss = g->GetFunction("gaus");
    tex->DrawLatex(0.60, 0.85, "Gauss Fit");
    sprintf(buffer_mu, "#mu = %.3f #pm %.3f", fgauss->GetParameter(1), fgauss->GetParError(1));
    sprintf(buffer_S, "#sigma = %.3f #pm %.3f", fgauss->GetParameter(2), fgauss->GetParError(2));
    sprintf(buffer_Chi, "#chi^{2}_{red} = %.3f / %d", fgauss->GetChisquare(), fgauss->GetNDF());
    sprintf(buffer_r, "R(0,0) = %.3f", fgauss->GetMaximum() *10000);
    sprintf(buffer_h, "h_{y} = #splitline{%.3f}{#pm %.3f} (mm)", fgauss->Integral(fgauss->GetXmin(), fgauss->GetXmax()) / fgauss->GetMaximum() , fgauss->IntegralError(fgauss->GetXmin(), fgauss->GetXmax()) / fgauss->GetMaximum() );
    tex->DrawLatex(0.60, 0.80, buffer_mu);
    tex->DrawLatex(0.60, 0.75, buffer_S);
    //tex->DrawLatex(0.60, 0.70, buffer_Chi);
    tex->DrawLatex(0.60, 0.65, buffer_h);
    tex->DrawLatex(0.60, 0.60, buffer_r);
    fgauss = 0;

    c->cd(2);
    g2->GetXaxis()->SetTitle("nominal separation (#Deltay)");
    g2->GetYaxis()->SetTitle("Anode Current (#muA)");
    g2->Draw("AP");
    fgauss = g2->GetFunction("gaus");
    tex->DrawLatex(0.60, 0.85, "Gauss Fit");
    sprintf(buffer_mu, "#mu = %.3f #pm %.3f", fgauss->GetParameter(1), fgauss->GetParError(1));
    sprintf(buffer_S, "#sigma = %.3f #pm %.3f", fgauss->GetParameter(2), fgauss->GetParError(2));
    sprintf(buffer_Chi, "#chi^{2}_{red} = %.3f / %d", fgauss->GetChisquare(), fgauss->GetNDF());
    sprintf(buffer_r, "R(0,0) = %.3f", fgauss->GetMaximum());
    sprintf(buffer_h, "h_{y} = #splitline{%.3f}{#pm %.3f} (mm)", fgauss->Integral(fgauss->GetXmin(), fgauss->GetXmax()) / fgauss->GetMaximum() , fgauss->IntegralError(fgauss->GetXmin(), fgauss->GetXmax()) / fgauss->GetMaximum());
    tex->DrawLatex(0.60, 0.80, buffer_mu);
    tex->DrawLatex(0.60, 0.75, buffer_S);
    //tex->DrawLatex(0.60, 0.70, buffer_Chi);
    tex->DrawLatex(0.60, 0.65, buffer_h);
    tex->DrawLatex(0.60, 0.60, buffer_r);
}