Double_t G2(Double_t *x, Double_t *par)
{
    Double_t PDF;
    Double_t g1;
    Double_t g2;

    Double_t arg1 = (par[2] != 0.0) ? (x[0] - par[1]) / (par[2]) : 0.0;
    Double_t arg2 = (par[4] != 0.0) ? (x[0] - par[3]) / (par[4]) : 0.0;

    g1 = exp(-0.5 * arg1 * arg1) / (par[2] * sqrt(2.0 * TMath::Pi()));
    g2 = exp(-0.5 * arg2 * arg2) / (par[4] * sqrt(2.0 * TMath::Pi()));

    PDF = par[0] * (par[5]*g1 + (1-par[5])*g2);

    return PDF;
}

void connectGraph()
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

    auto c0 = new TCanvas("c0", "Slope Distribution", 10, 10, 1800, 1600);
    auto hist = new TH1D("hist", "Add of all Slope Hists", 40, 0.02, .22);

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

    for(int i=0;i<fit_b.size();i++){
        if(working_chamber[i]){
            hist->Fill(fit_b[i]);
        }
    }

    
    Double_t par[9];
    TF1 *g1 = new TF1("g1", "gaus", 0.02, 0.1);
    hist->Fit(g1, "R");
    TF1 *g2 = new TF1("g2", "gaus", 0.08, 0.15);
    hist->Fit(g2, "R");
    TF1 *g3 = new TF1("g3", "gaus", 0.13, 0.22);
    hist->Fit(g3, "R");
    g1->GetParameters(&par[0]);
    g2->GetParameters(&par[3]);
    g3->GetParameters(&par[6]);
    TF1 *g4 = new TF1("g4", "gaus(0)+gaus(3)+gaus(6)", 0.02, 0.22);
    g4->SetParameters(par);
    hist->Fit(g4, "R");

    hist->GetXaxis()->SetTitle("Slope Parameter (#muA/(Hz/#mub))");
    hist->GetYaxis()->SetTitle("entries");
    //hist->GetXaxis()->SetRangeUser(0.02, 0.15);
    hist->Draw("");

    // Fit results
    TLatex *tex = new TLatex();
    tex->SetNDC(kTRUE);
    tex->SetTextSize(0.035);
    tex->SetTextColor(kBlack);
    char buffer_M[100],
        buffer_S[100],
        buffer_M1[100],
        buffer_S1[100],
        buffer_M2[100],
        buffer_S2[100],
        buffer_Chi[100];
    sprintf(buffer_M, "#mu_1 = %.3f #pm %.3f", g4->GetParameter(1), g4->GetParError(1));
    sprintf(buffer_S, "#sigma_1 = %.3f #pm %.3f", g4->GetParameter(2), g4->GetParError(2));
    sprintf(buffer_M1, "#mu_2 = %.3f #pm %.3f", g4->GetParameter(4), g4->GetParError(4));
    sprintf(buffer_S1, "#sigma_2 = %.3f #pm %.3f", g4->GetParameter(5), g4->GetParError(5));
    sprintf(buffer_M2, "#mu_3 = %.3f #pm %.3f", g4->GetParameter(6), g4->GetParError(6));
    sprintf(buffer_S2, "#sigma_3 = %.3f #pm %.3f", g4->GetParameter(7), g4->GetParError(7));
    sprintf(buffer_Chi, "#chi^{2}_{red} = %.1f", g4->GetChisquare() / g4->GetNDF());
    tex->DrawLatex(0.6, 0.85, "Gaussian Fit");
    tex->DrawLatex(0.6, 0.80, buffer_M);
    tex->DrawLatex(0.6, 0.75, buffer_S);
    tex->DrawLatex(0.6, 0.70, buffer_M1);
    tex->DrawLatex(0.6, 0.65, buffer_S1);
    tex->DrawLatex(0.6, 0.60, buffer_M2);
    tex->DrawLatex(0.6, 0.55, buffer_S2);
    tex->DrawLatex(0.6, 0.50, buffer_Chi);
}
