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

    TCanvas *c0 = new TCanvas("c0", "Slope Distribution", 10, 10, 1800, 1600);
    TH1D *h_0 = new TH1D("h_0", "Add of all Slope Hists in Stack 0", 30, 0, 0.15);
    TH1D *h_4 = new TH1D("h_4", "Add of all Slope Hists in Stack 4", 30, 0, 0.15);

    TFile *s[numfiles];
    for (Int_t i = 0; i < numfiles; i++)
    {
        s[i] = TFile::Open(root_File_Names[i], "READ");

        TH1D *h0 = (TH1D *)s[i]->Get("fits/h_b_0");
        TH1D *h4 = (TH1D *)s[i]->Get("fits/h_b_4");

        h_0->Add(h0);
        h_4->Add(h4);
    }
    // Norm
    Double_t scale_0 = 1 / h_0->Integral();
    Double_t scale_4 = 1 / h_4->Integral();
    h_0->Scale(scale_0);
    h_4->Scale(scale_4);

    h_0->Draw();
    h_4->Draw("same");
}