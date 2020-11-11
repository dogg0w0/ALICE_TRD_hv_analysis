void sector_average()
{
    gROOT->SetStyle("ATLAS");
    gROOT->ForceStyle();

    std::vector<TString> root_File_Names;
    for (int i = 0; i < 18; i++)
    {
        root_File_Names.push_back(TString::Format("sm_%d.root", i));
    }
    const int numfiles = 18;

    Double_t para_a, para_b;
    Long64_t nentries;
    Bool_t work;

    TBranch *para_aBranch = 0;
    TBranch *para_bBranch = 0;
    TBranch *para_wBranch = 0;

    vector<Double_t>
        fit_a(540, 0.0),
        fit_b(540, 0.0);
    vector<Bool_t> working_chamber(540, 0);
    TTree *tree = 0;
    TTree *tree_w = 0;

    TFile *s[numfiles];
    for (Int_t i = 0; i < numfiles; i++)
    {
        s[i] = TFile::Open(root_File_Names[i], "READ");
        tree = (TTree *)s[i]->Get("fits/fit_params");
        tree->SetMakeClass(1);
        tree->SetBranchAddress("fit_a", &para_a, &para_aBranch);
        tree->SetBranchAddress("fit_b", &para_b, &para_bBranch);
        nentries = tree->GetEntries();
        //cout << "HER---------------------------------------" << endl;
        tree_w = (TTree *)s[i]->Get("working_chambers");
        tree_w->SetMakeClass(1);
        tree_w->SetBranchAddress("working", &work, &para_wBranch);

        // Then loop over all of them.
        for (Long64_t j = 0; j < nentries; j++)
        {
            tree->GetEntry(j);
            tree_w->GetEntry(j);
            fit_a[30 * i + j] = para_a;
            fit_b[30 * i + j] = para_b;
            working_chamber[30 * i + j] = work;
        }
    }

    Int_t x_bins_layer = 6,
          x_bins_stack = 5,
          y_bins = 18;

    vector<Double_t> v_sector(y_bins, 0.0),
                    v_n_sec(y_bins, 0);

    for (Int_t sector = 0; sector < y_bins; sector++)
    {
        Double_t _mean = 0.0;
        Int_t _counter = 0;
        vector<Double_t> _std;
        for (Int_t stack = 0; stack < x_bins_stack; stack++)
        {
            for (Int_t layer = 0; layer < x_bins_layer; layer++)
            {
                if (working_chamber[sector * 30 + layer + stack * 6])
                {
                    _mean += fit_b[sector * 30 + stack * 6 + layer];
                    _counter++;
                }
            }
        }
        v_n_sec[sector] = _counter;
        _mean /= (_counter > 0) ? (Double_t)_counter : -0.001;
        v_sector[sector] = _mean;
        _std.clear();
    }

    auto C1 = new TCanvas("C1", "Layer", 10, 10, 800, 600);
    auto h_sector = new TH1D("h_layer", "Mean Slope Para per Sector;Sector;Slope", y_bins, 0, y_bins);
    h_sector->GetXaxis()->SetNdivisions(20, 0, 0, kTRUE);

    for (Int_t sector = 0; sector < 18; sector++)
    {
        h_sector->GetXaxis()->SetBinLabel(sector + 1, Form("%d", sector));
        h_sector->SetBinContent(sector+1, v_sector[sector]);
    }


    C1->cd();
    h_sector->Draw();
}