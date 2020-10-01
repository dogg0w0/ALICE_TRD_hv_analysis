{
    gStyle->SetOptFit(0);
    gStyle->SetOptStat(0);
    std::vector<TString> root_File_Names;
    for (int i = 0; i < 18; i++)
    {
        root_File_Names.push_back(TString::Format("sm_%d.root", i));
    }
    const int numfiles = 18;

    TCanvas *c0 = new TCanvas("c0", "Offset Distribution", 10, 10, 1800, 1600);
    c0->Divide(6, 3);

    TFile *s[numfiles];
    for (Int_t i = 0; i < numfiles; i++)
    {
        c0->cd(i + 1);
        s[i] = TFile::Open(root_File_Names[i], "READ");

        TH1D *h1 = (TH1D *)s[i]->Get("fits/h_a");
        h1->SetTitle(Form("Offset Sector %d", i));
        h1->Fit("gaus");
        h1->Draw("");
    }
    c0->Draw();
}
