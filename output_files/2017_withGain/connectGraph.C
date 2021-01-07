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
    auto hist = new TH1D("hist", "Add of all Slope Hists", 30, 0, 0.2);

    TFile *s[numfiles];
    for (Int_t i = 0; i < numfiles; i++)
    {
        s[i] = TFile::Open(root_File_Names[i], "READ");

        auto h1 = (TH1D *)s[i]->Get("fits/h_b");
        hist->Add(h1);
    }


    TF1 *g3 = new TF1("g3", "gaus", 0.02, 0.15);
    hist->Fit(g3, "R");

    hist->GetXaxis()->SetTitle("Slope Parameter (#muA/(Hz/#mub))");
    hist->GetYaxis()->SetTitle("entries");
    hist->GetXaxis()->SetRangeUser(0.02, 0.12);
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
    sprintf(buffer_M, "#mu = %.3f #pm %.3f", g3->GetParameter(1), g3->GetParError(1));
    sprintf(buffer_S, "#sigma = %.3f #pm %.3f", g3->GetParameter(2), g3->GetParError(2));
    sprintf(buffer_Chi, "#chi^{2}_{red} = %.1f", g3->GetChisquare() / g3->GetNDF());
    tex->DrawLatex(0.6, 0.85, "Gaussian Fit");
    tex->DrawLatex(0.6, 0.80, buffer_M);
    tex->DrawLatex(0.6, 0.75, buffer_S);
    tex->DrawLatex(0.6, 0.70, buffer_Chi);
}
