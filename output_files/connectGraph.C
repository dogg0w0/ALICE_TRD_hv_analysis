{
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
    TF1 *g1 = new TF1("g1", "gaus", 0.05, 0.105);
    TF1 *g2 = new TF1("g2", "gaus", 0.1, 0.145);
    TF1 *total = new TF1("total", "gaus(0)+gaus(3)", 0.05, 0.145);
    total->SetLineColor(3);
    hist->Fit(g1, "R");
    hist->Fit(g2, "R+");
    g1->GetParameters(&par[0]);
    g2->GetParameters(&par[3]);
    total->SetParameters(par);
    hist->Fit(total, "R+");

    hist->GetXaxis()->SetTitle("Slope Parameter");
    hist->GetYaxis()->SetTitle("entries");
    hist->Draw("");

    // Fit results
    TLatex *tex = new TLatex();
    tex->SetNDC(kTRUE);
    tex->SetTextSize(0.035);
    tex->SetTextColor(kBlack);
    tex->DrawLatex(0.15, 0.85, "Gauss Fit Left");
    char buffer_M[100],
        buffer_S[100],
        buffer_Chi[100];
    sprintf(buffer_M, "M_{Z} = %.3f #pm %.3f", g1->GetParameter(1), g1->GetParError(1));
    sprintf(buffer_S, "#sigma = %.3f #pm %.3f", g1->GetParameter(2), g1->GetParError(2));
    sprintf(buffer_Chi, "#chi^{2}_{red} = %.1f", g1->GetChisquare() / g1->GetNDF());
    tex->DrawLatex(0.15, 0.80, buffer_M);
    tex->DrawLatex(0.15, 0.75, buffer_S);
    tex->DrawLatex(0.15, 0.70, buffer_Chi);

    tex->DrawLatex(0.15, 0.60, "Gauss Fit Right");
    sprintf(buffer_M, "M_{Z} = %.3f #pm %.3f", g2->GetParameter(1), g2->GetParError(1));
    sprintf(buffer_S, "#sigma = %.3f #pm %.3f", g2->GetParameter(2), g2->GetParError(2));
    sprintf(buffer_Chi, "#chi^{2}_{red} = %.1f", g2->GetChisquare() / g2->GetNDF());
    tex->DrawLatex(0.15, 0.55, buffer_M);
    tex->DrawLatex(0.15, 0.50, buffer_S);
    tex->DrawLatex(0.15, 0.45, buffer_Chi);
}
