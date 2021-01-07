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
    auto h_0 = new TH1D("h_0", "Stack 0", 32, 0.02, 0.15);
    auto h_1 = new TH1D("h_1", "Stack 1", 30, 0.02, 0.15);
    auto h_2 = new TH1D("h_2", "Stack 2", 32, 0.02, 0.15);
    auto h_3 = new TH1D("h_3", "Stack 3", 20, 0.02, 0.15);
    auto h_4 = new TH1D("h_4", "Stack 4", 28, 0.02, 0.15);

    TTree *tree = 0;
    TBranch *para_bBranch = 0;
    Double_t para_a, para_b;
    Long64_t nentries;

    vector<Double_t>
        fit_a(540, 0.0),
        fit_b(540, 0.0);

    TFile *s[numfiles];
    for (Int_t i = 0; i < numfiles; i++)
    {
        s[i] = TFile::Open(root_File_Names[i], "READ");

        tree = (TTree *)s[i]->Get("fits/fit_params");
        tree->SetMakeClass(1);
        tree->SetBranchAddress("fit_b", &para_b, &para_bBranch);
        nentries = tree->GetEntries();

        // Then loop over all of them.
        for (Long64_t j = 0; j < nentries; j++)
        {
            tree->GetEntry(j);
            fit_a[30 * i + j] = para_a;
            fit_b[30 * i + j] = para_b;
        }

        auto h0 = (TH1D *)s[i]->Get("fits/h_b_0");
        auto h1 = (TH1D *)s[i]->Get("fits/h_b_1");
        auto h2 = (TH1D *)s[i]->Get("fits/h_b_2");
        auto h3 = (TH1D *)s[i]->Get("fits/h_b_3");
        auto h4 = (TH1D *)s[i]->Get("fits/h_b_4");
    }

    Int_t layers[] = {0,1,2, 3,4,5};

    for (Int_t sector = 0; sector < numfiles; sector++)
    {
        for (auto &&layer : layers)
        {
            h_0->Fill(fit_b[30 * sector + 6 * 0 + layer]);
            h_1->Fill(fit_b[30 * sector + 6 * 1 + layer]);
            h_2->Fill(fit_b[30 * sector + 6 * 2 + layer]);
            h_3->Fill(fit_b[30 * sector + 6 * 3 + layer]);
            h_4->Fill(fit_b[30 * sector + 6 * 4 + layer]);
        }
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

    h_0->GetXaxis()->SetTitle("slope parameter (#muA/(Hz/#mub))");
    h_0->GetYaxis()->SetTitle("nor. entries");
    // FIT
    Bool_t kfit = kTRUE;
    if (kfit)
    {
        TF1 *g0 = new TF1("g0", "gaus", 0.01, 0.16);
        TF1 *g1 = new TF1("g1", "gaus", 0.01, 0.16);
        TF1 *g2 = new TF1("g2", "gaus", 0.01, 0.16);
        TF1 *g3 = new TF1("g3", "gaus", 0.01, 0.16);
        TF1 *g4 = new TF1("g4", "gaus", 0.01, 0.16);
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

    h_0->GetYaxis()->SetRangeUser(0, 0.25);
    //h_0->GetXaxis()->SetRangeUser(0.01, 0.12);

    h_0->Draw("9 PLC PMC");
    h_1->Draw("9 SAME PLC PMC");
    h_2->Draw("9 SAME PLC PMC");
    h_3->Draw("9 SAME PLC PMC");
    h_4->Draw("9 SAME PLC PMC");
    gPad->BuildLegend(0.80, 0.65, 0.9, 0.9);

    if (kfit)
    {
        TLatex *tex = new TLatex();
        tex->SetNDC(kTRUE);
        tex->SetTextSize(0.02);
        tex->SetTextColor(kBlack);
        tex->DrawLatex(0.65, 0.88, "Gauss Fit Stack 0");
        char buffer_M[100],
            buffer_S[100],
            buffer_Chi[100];
        sprintf(buffer_M, "#mu = %.3f #pm %.3f", g0->GetParameter(1), g0->GetParError(1));
        sprintf(buffer_S, "#sigma = %.3f #pm %.3f", g0->GetParameter(2), g0->GetParError(2));
        sprintf(buffer_Chi, "#chi^{2}_{red} = %.3f", g0->GetChisquare() / g0->GetNDF());
        tex->DrawLatex(0.65, 0.85, buffer_M);
        tex->DrawLatex(0.65, 0.82, buffer_S);
        tex->DrawLatex(0.65, 0.79, buffer_Chi);

        tex->DrawLatex(0.65, 0.75, "Gauss Fit Stack 1");
        sprintf(buffer_M, "#mu = %.3f #pm %.3f", g1->GetParameter(1), g1->GetParError(1));
        sprintf(buffer_S, "#sigma = %.3f #pm %.3f", g1->GetParameter(2), g1->GetParError(2));
        sprintf(buffer_Chi, "#chi^{2}_{red} = %.3f", g1->GetChisquare() / g1->GetNDF());
        tex->DrawLatex(0.65, 0.72, buffer_M);
        tex->DrawLatex(0.65, 0.69, buffer_S);
        tex->DrawLatex(0.65, 0.66, buffer_Chi);

        tex->DrawLatex(0.65, 0.62, "Gauss Fit Stack 2");
        sprintf(buffer_M, "#mu = %.3f #pm %.3f", g2->GetParameter(1), g2->GetParError(1));
        sprintf(buffer_S, "#sigma = %.3f #pm %.3f", g2->GetParameter(2), g2->GetParError(2));
        sprintf(buffer_Chi, "#chi^{2}_{red} = %.3f", g2->GetChisquare() / g2->GetNDF());
        tex->DrawLatex(0.65, 0.59, buffer_M);
        tex->DrawLatex(0.65, 0.56, buffer_S);
        tex->DrawLatex(0.65, 0.53, buffer_Chi);

        tex->DrawLatex(0.65, 0.49, "Gauss Fit Stack 3");
        sprintf(buffer_M, "#mu = %.3f #pm %.3f", g3->GetParameter(1), g3->GetParError(1));
        sprintf(buffer_S, "#sigma = %.3f #pm %.3f", g3->GetParameter(2), g3->GetParError(2));
        sprintf(buffer_Chi, "#chi^{2}_{red} = %.3f", g3->GetChisquare() / g3->GetNDF());
        tex->DrawLatex(0.65, 0.46, buffer_M);
        tex->DrawLatex(0.65, 0.43, buffer_S);
        tex->DrawLatex(0.65, 0.40, buffer_Chi);

        tex->DrawLatex(0.65, 0.36, "Gauss Fit Stack 4");
        sprintf(buffer_M, "#mu = %.3f #pm %.3f", g4->GetParameter(1), g4->GetParError(1));
        sprintf(buffer_S, "#sigma = %.3f #pm %.3f", g4->GetParameter(2), g4->GetParError(2));
        sprintf(buffer_Chi, "#chi^{2}_{red} = %.3f", g4->GetChisquare() / g4->GetNDF());
        tex->DrawLatex(0.65, 0.33, buffer_M);
        tex->DrawLatex(0.65, 0.30, buffer_S);
        tex->DrawLatex(0.65, 0.27, buffer_Chi);
    }
}