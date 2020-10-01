{
    ROOT::Math::MinimizerOptions::SetDefaultMinimizer("Minuit2");
    std::vector<TString> root_File_Names;
    for (int i = 0; i < 18; i++)
    {
        root_File_Names.push_back(TString::Format("sm_%d.root", i));
    }
    const int numfiles = 18;

    auto C = new TCanvas();

    gStyle->SetOptTitle(kFALSE);
    gStyle->SetOptStat(0);
    gStyle->SetOptFit(0);

    TH1D *h_0 = new TH1D("h_0", "Slope Fit Param Hists in Stack 0", 30, 0, 0.15);
    TH1D *h_1 = new TH1D("h_1", "Slope Fit Param Hists in Stack 1", 30, 0, 0.15);
    TH1D *h_2 = new TH1D("h_2", "Slope Fit Param Hists in Stack 2", 30, 0, 0.15);
    TH1D *h_3 = new TH1D("h_3", "Slope Fit Param Hists in Stack 3", 30, 0, 0.15);
    TH1D *h_4 = new TH1D("h_4", "Slope Fit Param Hists in Stack 4", 30, 0, 0.15);

    TFile *s[numfiles];
    for (Int_t i = 0; i < numfiles; i++)
    {
        s[i] = TFile::Open(root_File_Names[i], "READ");

        TH1D *h0 = (TH1D *)s[i]->Get("fits/h_b_0");
        TH1D *h1 = (TH1D *)s[i]->Get("fits/h_b_1");
        TH1D *h2 = (TH1D *)s[i]->Get("fits/h_b_2");
        TH1D *h3 = (TH1D *)s[i]->Get("fits/h_b_3");
        TH1D *h4 = (TH1D *)s[i]->Get("fits/h_b_4");

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

    h_0->GetXaxis()->SetTitle("slope param");
    h_0->GetYaxis()->SetTitle("n. entries");
    // FIT
    Bool_t kfit = KFALSE;
    if (kfit)
    {
    TF1 *g0 = new TF1("g2", "gaus", 0.09, 0.145);
    TF1 *g4 = new TF1("g1", "gaus", 0.05, 0.105);
    //g4->SetParLimits(0, 0, 17);
    h_0->Fit(g0, "R");
    h_4->Fit(g4, "R");
    }


    h_0->Draw("PLC PMC");
    h_1->Draw("SAME PLC PMC");
    h_2->Draw("SAME PLC PMC");
    h_3->Draw("SAME PLC PMC");
    h_4->Draw("SAME PLC PMC");
    gPad->BuildLegend();

    
    //TLatex *tex = new TLatex();
    //tex->SetNDC(kTRUE);
    //tex->SetTextSize(0.035);
    //tex->SetTextColor(kBlack);
    //tex->DrawLatex(0.15, 0.85, "Gauss Fit Stack 0");
    //char buffer_M[100],
    //    buffer_S[100],
    //    buffer_Chi[100];
    //sprintf(buffer_M, "M_{Z} = %.3f #pm %.3f", g0->GetParameter(1), g0->GetParError(1));
    //sprintf(buffer_S, "#sigma = %.3f #pm %.3f", g0->GetParameter(2), g0->GetParError(2));
    //sprintf(buffer_Chi, "#chi^{2}_{red} = %.1f", g0->GetChisquare() / g0->GetNDF());
    //tex->DrawLatex(0.15, 0.80, buffer_M);
    //tex->DrawLatex(0.15, 0.75, buffer_S);
    //tex->DrawLatex(0.15, 0.70, buffer_Chi);
//
    //tex->DrawLatex(0.15, 0.60, "Gauss Fit Stack 4");
    //sprintf(buffer_M, "M_{Z} = %.3f #pm %.3f", g4->GetParameter(1), g4->GetParError(1));
    //sprintf(buffer_S, "#sigma = %.3f #pm %.3f", g4->GetParameter(2), g4->GetParError(2));
    //sprintf(buffer_Chi, "#chi^{2}_{red} = %.1f", g4->GetChisquare() / g4->GetNDF());
    //tex->DrawLatex(0.15, 0.55, buffer_M);
    //tex->DrawLatex(0.15, 0.50, buffer_S);
    //tex->DrawLatex(0.15, 0.45, buffer_Chi);
}