{
    gROOT->SetStyle("Pub");
    gROOT->ForceStyle();

    ROOT::Math::MinimizerOptions::SetDefaultMinimizer("Minuit2");
    Int_t palette[5];
    palette[0] = 2;
    palette[1] = 3;
    palette[2] = 4;
    palette[3] = 6;
    palette[4] = 7;
    gStyle->SetPalette(5, palette);
    std::vector<TString> root_File_Names;
    for (int i = 0; i < 18; i++)
    {
        root_File_Names.push_back(TString::Format("sm_%d.root", i));
    }
    const int numfiles = 18;

    auto C = new TCanvas();

    auto h_0 = new TH1D("h_0", "Slope Fit Param Hists in Stack 0", 30, 0, 0.20);
    auto h_1 = new TH1D("h_1", "Slope Fit Param Hists in Stack 1", 30, 0, 0.20);
    auto h_2 = new TH1D("h_2", "Slope Fit Param Hists in Stack 2", 30, 0, 0.20);
    auto h_3 = new TH1D("h_3", "Slope Fit Param Hists in Stack 3", 30, 0, 0.20);
    auto h_4 = new TH1D("h_4", "Slope Fit Param Hists in Stack 4", 30, 0, 0.20);

    TFile *s[numfiles];
    for (Int_t i = 0; i < numfiles; i++)
    {
        s[i] = TFile::Open(root_File_Names[i], "READ");

        auto h0 = (TH1D *)s[i]->Get("fits/h_b_0");
        auto h1 = (TH1D *)s[i]->Get("fits/h_b_1");
        auto h2 = (TH1D *)s[i]->Get("fits/h_b_2");
        auto h3 = (TH1D *)s[i]->Get("fits/h_b_3");
        auto h4 = (TH1D *)s[i]->Get("fits/h_b_4");

        h_0->Add(h0);
        h_1->Add(h1);
        h_2->Add(h2);
        h_3->Add(h3);
        h_4->Add(h4);
    }
    //Norm
    Double_t scale_0 = 1 / h_0->Integral();
    Double_t scale_1 = 1 / h_1->Integral();
    Double_t scale_2 = 1 / h_2->Integral();
    Double_t scale_3 = 1 / h_3->Integral();
    Double_t scale_4 = 1 / h_4->Integral();
    h_0->Scale(scale_0);
    h_1->Scale(scale_1);
    h_2->Scale(scale_2);
    h_3->Scale(scale_3);
    h_4->Scale(scale_4);

    h_0->SetMarkerStyle(kFullCircle);
    h_1->SetMarkerStyle(kFullSquare);
    h_2->SetMarkerStyle(kFullTriangleUp);
    h_3->SetMarkerStyle(kFullTriangleDown);
    h_4->SetMarkerStyle(kOpenCircle);

    h_0->GetXaxis()->SetTitle("slope parameter (A/(Hz/#mub))");
    h_0->GetYaxis()->SetTitle("nor. entries");
    // FIT
    Bool_t kfit = kTRUE;
    if (kfit)
    {
        TF1 *g0 = new TF1("g0", "gaus", 0.02, 0.14);
        TF1 *g1 = new TF1("g1", "gaus", 0.02, 0.14);
        TF1 *g2 = new TF1("g2", "gaus", 0.02, 0.14);
        TF1 *g3 = new TF1("g3", "gaus", 0.02, 0.14);
        TF1 *g4 = new TF1("g4", "gaus", 0.02, 0.14);
        h_0->Fit(g0, "R W");
        h_1->Fit(g1, "R W");
        h_2->Fit(g2, "R W");
        h_3->Fit(g3, "R W");
        h_4->Fit(g4, "R W");
        h_0->GetFunction("g0")->SetLineColor(palette[0]);
        h_1->GetFunction("g1")->SetLineColor(palette[1]);
        h_2->GetFunction("g2")->SetLineColor(palette[2]);
        h_3->GetFunction("g3")->SetLineColor(palette[3]);
        h_4->GetFunction("g4")->SetLineColor(palette[4]);
    }

    h_0->GetYaxis()->SetRangeUser(0, 0.5);
    //h_0->GetXaxis()->SetRangeUser(0.01, 0.12);

    h_0->Draw("9 PLC PMC");
    h_1->Draw("9 SAME PLC PMC");
    h_2->Draw("9 SAME PLC PMC");
    h_3->Draw("9 SAME PLC PMC");
    h_4->Draw("9 SAME PLC PMC");
    gPad->BuildLegend(0.75, 0.75, 0.9, 0.9);

    if (kfit)
    {
        TLatex *tex = new TLatex();
        tex->SetNDC(kTRUE);
        tex->SetTextSize(0.02);
        tex->SetTextColor(kBlack);
        tex->DrawLatex(0.25, 0.88, "Gauss Fit Stack 0");
        char buffer_M[100],
            buffer_S[100],
            buffer_Chi[100];
        sprintf(buffer_M, "#mu = %.3f #pm %.3f", g0->GetParameter(1), g0->GetParError(1));
        sprintf(buffer_S, "#sigma = %.3f #pm %.3f", g0->GetParameter(2), g0->GetParError(2));
        sprintf(buffer_Chi, "#chi^{2}_{red} = %.3f", g0->GetChisquare() / g0->GetNDF());
        tex->DrawLatex(0.25, 0.85, buffer_M);
        tex->DrawLatex(0.25, 0.82, buffer_S);
        tex->DrawLatex(0.25, 0.79, buffer_Chi);

        tex->DrawLatex(0.25, 0.75, "Gauss Fit Stack 1");
        sprintf(buffer_M, "#mu = %.3f #pm %.3f", g1->GetParameter(1), g1->GetParError(1));
        sprintf(buffer_S, "#sigma = %.3f #pm %.3f", g1->GetParameter(2), g1->GetParError(2));
        sprintf(buffer_Chi, "#chi^{2}_{red} = %.3f", g1->GetChisquare() / g1->GetNDF());
        tex->DrawLatex(0.25, 0.72, buffer_M);
        tex->DrawLatex(0.25, 0.69, buffer_S);
        tex->DrawLatex(0.25, 0.66, buffer_Chi);

        tex->DrawLatex(0.25, 0.62, "Gauss Fit Stack 2");
        sprintf(buffer_M, "#mu = %.3f #pm %.3f", g2->GetParameter(1), g2->GetParError(1));
        sprintf(buffer_S, "#sigma = %.3f #pm %.3f", g2->GetParameter(2), g2->GetParError(2));
        sprintf(buffer_Chi, "#chi^{2}_{red} = %.3f", g2->GetChisquare() / g2->GetNDF());
        tex->DrawLatex(0.25, 0.59, buffer_M);
        tex->DrawLatex(0.25, 0.56, buffer_S);
        tex->DrawLatex(0.25, 0.53, buffer_Chi);

        tex->DrawLatex(0.25, 0.49, "Gauss Fit Stack 3");
        sprintf(buffer_M, "#mu = %.3f #pm %.3f", g3->GetParameter(1), g3->GetParError(1));
        sprintf(buffer_S, "#sigma = %.3f #pm %.3f", g3->GetParameter(2), g3->GetParError(2));
        sprintf(buffer_Chi, "#chi^{2}_{red} = %.3f", g3->GetChisquare() / g3->GetNDF());
        tex->DrawLatex(0.25, 0.46, buffer_M);
        tex->DrawLatex(0.25, 0.43, buffer_S);
        tex->DrawLatex(0.25, 0.40, buffer_Chi);

        tex->DrawLatex(0.25, 0.36, "Gauss Fit Stack 4");
        sprintf(buffer_M, "#mu = %.3f #pm %.3f", g4->GetParameter(1), g4->GetParError(1));
        sprintf(buffer_S, "#sigma = %.3f #pm %.3f", g4->GetParameter(2), g4->GetParError(2));
        sprintf(buffer_Chi, "#chi^{2}_{red} = %.3f", g4->GetChisquare() / g4->GetNDF());
        tex->DrawLatex(0.25, 0.33, buffer_M);
        tex->DrawLatex(0.25, 0.30, buffer_S);
        tex->DrawLatex(0.25, 0.27, buffer_Chi);
    }
}