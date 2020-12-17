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

    //auto c0 = new TCanvas("c0", "Slope Distribution", 10, 10, 1800, 1600);
    vector<Int_t> bins;
    vector<Double_t> chi2;
    // for (int i = 5; i < 100; i++)
    // {
    //     TH1D hist("hist", "Add of all Slope Hists", i, 0.02, 0.15);
    //     TTree *tree = 0;
    //     TBranch *para_bBranch = 0;
    //     Double_t para_a, para_b;
    //     Long64_t nentries;

    //     TFile *s[numfiles];
    //     for (Int_t i = 0; i < numfiles; i++)
    //     {
    //         s[i] = TFile::Open(root_File_Names[i], "READ");
    //         tree = (TTree *)s[i]->Get("fits/fit_params");
    //         tree->SetMakeClass(1);
    //         tree->SetBranchAddress("fit_b", &para_b, &para_bBranch);
    //         nentries = tree->GetEntries();

    //         // Then loop over all of them.
    //         for (Long64_t j = 0; j < nentries; j++)
    //         {
    //             tree->GetEntry(j);
    //             hist.Fill(para_b);
    //         }

    //         //auto h1 = (TH1D *)s[i]->Get("fits/h_b");
    //         //hist->Add(h1);
    //     }

    //     Double_t par[6];
    //     TF1 g1("g1", "gaus", 0.02, 0.15);
    //     hist.Fit(&g1, "R");
    //     //
    //     //hist->GetXaxis()->SetTitle("Slope Parameter");
    //     //hist->GetYaxis()->SetTitle("entries");
    //     //hist->Draw("");

    //     // Fit results
    //     // TLatex *tex = new TLatex();
    //     // tex->SetNDC(kTRUE);
    //     // tex->SetTextSize(0.035);
    //     // tex->SetTextColor(kBlack);
    //     // char buffer_M[100],
    //     //     buffer_S[100],
    //     //     buffer_Chi[100];
    //     // tex->DrawLatex(0.6, 0.85, "Gauss Fit");
    //     // sprintf(buffer_M, "#mu = %.3f #pm %.3f", g1->GetParameter(1), g1->GetParError(1));
    //     // sprintf(buffer_S, "#sigma = %.3f #pm %.3f", g1->GetParameter(2), g1->GetParError(2));
    //     // sprintf(buffer_Chi, "#chi^{2}_{red} = %.1f", g1->GetChisquare() / g1->GetNDF());
    //     // tex->DrawLatex(0.6, 0.80, buffer_M);
    //     // tex->DrawLatex(0.6, 0.75, buffer_S);
    //     // tex->DrawLatex(0.6, 0.70, buffer_Chi);
    //     bins.push_back(i);
    //     chi2.push_back(g1.GetChisquare() / g1.GetNDF());
    // }
    // auto c = new TCanvas();
    // auto h = new TGraph();
    // for (int i = 0; i < bins.size(); i++)
    // {
    //     h->SetPoint(i, bins[i], chi2[i]);
    // }
    // h->SetMarkerSize(3);
    // h->SetMarkerColor(2);
    // h->Draw("ALP");

    TH1D hist("hist", "Add of all Slope Hists", 53, 0.02, 0.15);
    TTree *tree = 0;
    TBranch *para_bBranch = 0;
    Double_t para_a, para_b;
    Long64_t nentries;

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
            hist.Fill(para_b);
        }

        //auto h1 = (TH1D *)s[i]->Get("fits/h_b");
        //hist->Add(h1);
    }

    Double_t par[6];
    TF1* g1 = new TF1("g1", "gaus", 0.02, 0.15);
    hist.Fit(g1, "R");
    
    hist.GetXaxis()->SetTitle("Slope Parameter (#muA/(Hz/#mub))");
    hist.GetYaxis()->SetTitle("entries");
    hist.Draw("");

    TLatex *tex = new TLatex();
    tex->SetNDC(kTRUE);
    tex->SetTextSize(0.035);
    tex->SetTextColor(kBlack);
    char buffer_M[100],
        buffer_S[100],
        buffer_Chi[100];
    tex->DrawLatex(0.6, 0.85, "Gauss Fit");
    sprintf(buffer_M, "#mu = %.3f #pm %.3f", g1->GetParameter(1), g1->GetParError(1));
    sprintf(buffer_S, "#sigma = %.3f #pm %.3f", g1->GetParameter(2), g1->GetParError(2));
    sprintf(buffer_Chi, "#chi^{2}_{red} = %.1f", g1->GetChisquare() / g1->GetNDF());
    tex->DrawLatex(0.6, 0.80, buffer_M);
    tex->DrawLatex(0.6, 0.75, buffer_S);
    tex->DrawLatex(0.6, 0.70, buffer_Chi);
}