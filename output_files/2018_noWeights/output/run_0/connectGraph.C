Double_t G2(Double_t *x, Double_t *par)
{
    Double_t PDF;
    Double_t g1;
    Double_t g2;

    Double_t arg1 = (par[2] != 0.0) ? (x[0] - par[1]) / (par[2]) : 0.0;
    Double_t arg2 = (par[4] != 0.0) ? (x[0] - par[3]) / (par[4]) : 0.0;

    g1 = exp(-0.5 * arg1 * arg1) / (par[2] * sqrt(2.0 * TMath::Pi()));
    g2 = exp(-0.5 * arg2 * arg2) / (par[4] * sqrt(2.0 * TMath::Pi()));

    PDF = par[0] * (par[5]*g1 + (1-par[5])*g2);

    return PDF;
}

void connectGraph()
{
    gROOT->SetStyle("Pub");
    gROOT->ForceStyle();
    ROOT::Math::MinimizerOptions::SetDefaultMinimizer("Minuit2");
    gStyle->SetOptFit(0);
    gStyle->SetOptStat(0);
    std::vector<TString> root_File_Names;
    for (int i = 0; i < 18; i++)
    {
        root_File_Names.push_back(TString::Format("sm_%d.root", i));
    }
    const int numfiles = 18;

    auto c0 = new TCanvas("c0", "Slope Distribution", 10, 10, 1800, 1600);
    auto hist = new TH1D("hist", "Add of all Slope Hists", 30, 0, 0.15);

    TFile *s[numfiles];
    for (Int_t i = 0; i < numfiles; i++)
    {
        s[i] = TFile::Open(root_File_Names[i], "READ");

        auto h1 = (TH1D *)s[i]->Get("fits/h_b");
        hist->Add(h1);
    }

    Double_t par[6];
    TF1 *g1 = new TF1("g1", "gaus", 0.02, 0.1);
    hist->Fit(g1, "R");
    TF1 *g2 = new TF1("g2", "gaus", 0.08, 0.15);
    hist->Fit(g2, "R");
    g1->GetParameters(&par[0]);
    g2->GetParameters(&par[3]);
    TF1 *g3 = new TF1("g3", "gaus(0)+gaus(3)", 0.02, 0.15);
    g3->SetParameters(par);
    hist->Fit(g3, "R");

    hist->GetXaxis()->SetTitle("Slope Parameter (#muA/(Hz/#mub))");
    hist->GetYaxis()->SetTitle("entries");
    hist->GetXaxis()->SetRangeUser(0.02, 0.15);
    hist->Draw("");

    // Fit results
    TLatex *tex = new TLatex();
    tex->SetNDC(kTRUE);
    tex->SetTextSize(0.035);
    tex->SetTextColor(kBlack);
    char buffer_M[100],
        buffer_S[100],
        buffer_M1[100],
        buffer_S1[100],
        buffer_Chi[100];
    tex->DrawLatex(0.2, 0.85, "Gaussian Fit");
    sprintf(buffer_M, "#mu_1 = %.3f #pm %.3f", g3->GetParameter(1), g3->GetParError(1));
    sprintf(buffer_S, "#sigma_1 = %.3f #pm %.3f", g3->GetParameter(2), g3->GetParError(2));
    sprintf(buffer_M1, "#mu_2 = %.3f #pm %.3f", g3->GetParameter(4), g3->GetParError(4));
    sprintf(buffer_S1, "#sigma_2 = %.3f #pm %.3f", g3->GetParameter(5), g3->GetParError(5));
    sprintf(buffer_Chi, "#chi^{2}_{red} = %.1f", g3->GetChisquare() / g3->GetNDF());
    tex->DrawLatex(0.2, 0.80, buffer_M);
    tex->DrawLatex(0.2, 0.75, buffer_S);
    tex->DrawLatex(0.2, 0.70, buffer_M1);
    tex->DrawLatex(0.2, 0.65, buffer_S1);
    tex->DrawLatex(0.2, 0.60, buffer_Chi);
}
