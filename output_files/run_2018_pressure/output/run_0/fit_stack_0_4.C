{
    gROOT->SetStyle("Pub");
    gROOT->ForceStyle();

    Int_t palette[5];
    palette[0] = 2;
    palette[1] = 3;
    palette[2] = 4;
    palette[3] = 6;
    palette[4] = 7;
    gStyle->SetPalette(5, palette);
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
    auto h_0 = new TH1D("h_0", "Add of all Slope Hists in Stack 0", 30, 0, 0.15);
    auto h_4 = new TH1D("h_4", "Add of all Slope Hists in Stack 4", 30, 0, 0.15);

    TFile *s[numfiles];
    for (Int_t i = 0; i < numfiles; i++)
    {
        s[i] = TFile::Open(root_File_Names[i], "READ");

        auto h0 = (TH1D *)s[i]->Get("fits/h_b_0");
        auto h4 = (TH1D *)s[i]->Get("fits/h_b_4");

        h_0->Add(h0);
        h_4->Add(h4);
    }
    // Norm
    Double_t scale_0 = 1 / h_0->Integral();
    Double_t scale_4 = 1 / h_4->Integral();
    h_0->Scale(scale_0);
    h_4->Scale(scale_4);

    h_0->SetMarkerStyle(kFullCircle);
    h_4->SetMarkerStyle(kOpenCircle);

    h_0->GetXaxis()->SetTitle("slope parameter (#muA/(Hz/#mub))");
    h_0->GetYaxis()->SetTitle("nor. entries");

    h_0->GetYaxis()->SetRangeUser(0, 0.5);
    //h_0->GetXaxis()->SetRangeUser(0.02, 0.14);

    TF1 *g0 = new TF1("g0", "gaus", 0.02, 0.2);
    TF1 *g4 = new TF1("g4", "gaus", 0.02, 0.2);
    //g4->SetParLimits(0, 0, 17);
    h_0->Fit(g0, "R");
    h_4->Fit(g4, "R");

    h_0->GetFunction("g0")->SetLineColor(palette[0]);
    h_4->GetFunction("g4")->SetLineColor(palette[4]);

    h_0->Draw("9 PLC PMC");
    h_4->Draw("9 SAME PLC PMC");
    gPad->BuildLegend(0.75, 0.75, 0.9, 0.9);

    TLatex *tex = new TLatex();
    tex->SetNDC(kTRUE);
    tex->SetTextSize(0.035);
    tex->SetTextColor(kBlack);
    tex->DrawLatex(0.25, 0.85, "Gauss Fit Stack 0");
    char buffer_M[100],
        buffer_S[100],
        buffer_Chi[100];
    sprintf(buffer_M, "#mu = %.3f #pm %.3f", g0->GetParameter(1), g0->GetParError(1));
    sprintf(buffer_S, "#sigma = %.3f #pm %.3f", g0->GetParameter(2), g0->GetParError(2));
    sprintf(buffer_Chi, "#chi^{2}_{red} = %.1f", g0->GetChisquare() / g0->GetNDF());
    tex->DrawLatex(0.25, 0.80, buffer_M);
    tex->DrawLatex(0.25, 0.75, buffer_S);
    tex->DrawLatex(0.25, 0.70, buffer_Chi);
    
    tex->DrawLatex(0.25, 0.60, "Gauss Fit Stack 4");
    sprintf(buffer_M, "#mu = %.3f #pm %.3f", g4->GetParameter(1), g4->GetParError(1));
    sprintf(buffer_S, "#sigma = %.3f #pm %.3f", g4->GetParameter(2), g4->GetParError(2));
    sprintf(buffer_Chi, "#chi^{2}_{red} = %.1f", g4->GetChisquare() / g4->GetNDF());
    tex->DrawLatex(0.25, 0.55, buffer_M);
    tex->DrawLatex(0.25, 0.50, buffer_S);
    tex->DrawLatex(0.25, 0.45, buffer_Chi);
}