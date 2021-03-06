Double_t mean_std(const std::vector<Double_t> *v, const Double_t mean)
{
    Double_t standardDeviation = 0;
    for (auto &&element : *v)
    {
        standardDeviation += TMath::Power(element - mean, 2);
    }
    return TMath::Sqrt(standardDeviation / ((Double_t)(v->size() - 1)));
}

Double_t mean_se(const std::vector<Double_t> *v, const Double_t mean)
{
    Double_t standardDeviation = 0;
    for (auto &&element : *v)
    {
        standardDeviation += TMath::Power(element - mean, 2);
    }
    standardDeviation = TMath::Sqrt(standardDeviation / ((Double_t)(v->size() - 1)));
    return standardDeviation / TMath::Sqrt(v->size());
}

void eta_phi()
{
    gROOT->SetStyle("Pub");
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
            cout << working_chamber[30 * i + j] << endl;
        }
    }

    Int_t x_bins_layer = 6,
          x_bins_stack = 5,
          y_bins = 18;

    vector<Double_t> v_layer(y_bins * x_bins_layer, 0.0),
        v_layer_err(y_bins * x_bins_layer, 0.0),
        v_stack(y_bins * x_bins_stack, 0.0),
        v_stack_err(y_bins * x_bins_stack, 0.0);

    for (Int_t sector = 0; sector < y_bins; sector++)
    {
        for (Int_t layer = 0; layer < x_bins_layer; layer++)
        {
            Double_t _mean = 0.0;
            Int_t _counter = 0;
            vector<Double_t> _std;
            for (Int_t stack = 0; stack < x_bins_stack; stack++)
            {
                if (working_chamber[sector * 30 + layer + stack * 6])
                {
                    _mean += fit_b[sector * 30 + layer + stack * 6];
                    _std.push_back(fit_b[sector * 30 + layer + stack * 6]);
                    _counter++;
                }
            }
            _mean /= (_counter > 0) ? (Double_t) _counter : -0.01;
            v_layer[sector * x_bins_layer + layer] = _mean;
            v_layer_err[sector * x_bins_layer + layer] = mean_se(&_std, _mean);
            _std.clear();
        }
        for (Int_t stack = 0; stack < x_bins_stack; stack++)
        {
            Double_t _mean = 0.0;
            Int_t _counter = 0;
            vector<Double_t> _std;
            for (Int_t layer = 0; layer < x_bins_layer; layer++)
            {
                if (working_chamber[sector * 30 + layer + stack * 6])
                {
                    _mean += fit_b[sector * 30 + stack * 6 + layer];
                    _std.push_back(fit_b[sector * 30 + stack * 6 + layer]);
                    _counter++;
                }
            }
            _mean /= (_counter > 0) ? (Double_t) _counter : -0.001;
            v_stack[sector * x_bins_stack + stack] = _mean;
            v_stack_err[sector * x_bins_stack + stack] = mean_se(&_std, _mean);
            _std.clear();
        }
    }

    auto C1 = new TCanvas("C1", "Layer", 10, 10, 800, 600);
    auto C2 = new TCanvas("C2", "Stack", 10, 10, 800, 600);
    auto h_layer = new TH2D("h_layer", "Mean Slope per Layer;Layer;Sector;Slope Parameter (#muA/(Hz/#mub))", x_bins_layer, 0, x_bins_layer, y_bins, 0, y_bins);
    auto h_layer_err = new TH2D("h_layer_err", "SE Slope per Layer;Layer;Sector;Slope Standard Error", x_bins_layer, 0, x_bins_layer, y_bins, 0, y_bins);
    auto h_stack = new TH2D("h_stack", "Mean Slope per Stack;Stack;Sector;Slope Parameter (#muA/(Hz/#mub))", x_bins_stack, 0, x_bins_stack, y_bins, 0, y_bins);
    auto h_stack_err = new TH2D("h_stack_err", "SE Slope per Stack;Stack;Sector;Slope Standard Error", x_bins_stack, 0, x_bins_stack, y_bins, 0, y_bins);
    h_layer->GetXaxis()->SetNdivisions(8, 0, 0, kTRUE);
    h_layer->GetYaxis()->SetNdivisions(20, 0, 0, kTRUE);
    h_layer_err->GetXaxis()->SetNdivisions(8, 0, 0, kTRUE);
    h_layer_err->GetYaxis()->SetNdivisions(20, 0, 0, kTRUE);
    h_stack->GetXaxis()->SetNdivisions(8, 0, 0, kTRUE);
    h_stack->GetYaxis()->SetNdivisions(20, 0, 0, kTRUE);
    h_stack_err->GetXaxis()->SetNdivisions(8, 0, 0, kTRUE);
    h_stack_err->GetYaxis()->SetNdivisions(20, 0, 0, kTRUE);

    for (Int_t layer = 0; layer < 6; layer++)
    {
        h_layer->GetXaxis()->SetBinLabel(layer + 1, Form("%d", layer));
        h_layer_err->GetXaxis()->SetBinLabel(layer + 1, Form("%d", layer));
    }

    for (Int_t stack = 1; stack < 6; stack++)
    {
        h_stack->GetXaxis()->SetBinLabel(stack, Form("%d", stack - 1));
        h_stack_err->GetXaxis()->SetBinLabel(stack, Form("%d", stack - 1));
    }
    for (Int_t sector = 0; sector < 18; sector++)
    {
        h_stack->GetYaxis()->SetBinLabel(sector + 1, Form("%d", sector));
        h_layer->GetYaxis()->SetBinLabel(sector + 1, Form("%d", sector));
        h_stack_err->GetYaxis()->SetBinLabel(sector + 1, Form("%d", sector));
        h_layer_err->GetYaxis()->SetBinLabel(sector + 1, Form("%d", sector));
    }

    for (Int_t sector = 0; sector < y_bins; sector++)
    {
        for (Int_t layer = 0; layer < x_bins_layer; layer++)
        {
            h_layer->SetBinContent(layer + 1, sector + 1, v_layer[sector * x_bins_layer + layer]);
            h_layer_err->SetBinContent(layer + 1, sector + 1, v_layer_err[sector * x_bins_layer + layer]);
        }
        for (Int_t stack = 0; stack < x_bins_stack; stack++)
        {
            h_stack->SetBinContent(stack + 1, sector + 1, v_stack[sector * x_bins_stack + stack]);
            h_stack_err->SetBinContent(stack + 1, sector + 1, v_stack_err[sector * x_bins_stack + stack]);
        }
    }

    C1->Divide(2, 1);
    C1->cd(1);
    h_layer->Draw("colz");
    C1->cd(2);
    h_layer_err->Draw("colz");
    C1->Draw();

    C2->Divide(2, 1);
    C2->cd(1);
    h_stack->Draw("colz");
    C2->cd(2);
    h_stack_err->Draw("colz");
    C2->Draw();
}