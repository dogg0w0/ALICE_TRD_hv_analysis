void getBunchIntensities(string filename)
{
    gROOT->SetStyle("Pub");
    // Declaration of leaf types
    Double_t bunch1[3564];
    Double_t bunch2[3564];
    Double_t v0rate[3564];
    Double_t time;
    Double_t nsep;
    Int_t plane;
    Int_t aqflag;

    // List of branches
    TBranch *b_time; //!
    TBranch *b_bunch1;
    TBranch *b_bunch2;
    TBranch *b_nsep;
    TBranch *b_plane;
    TBranch *b_aqflag;
    TBranch *b_v0rate; //!

    TFile *f1 = TFile::Open(filename.c_str(), "READ");
    TTree *tree1 = new TTree();
    tree1 = (TTree *)f1->Get("VdM");
    tree1->SetMakeClass(1);
    tree1->SetBranchAddress("bunch1", &bunch1, &b_bunch1);
    tree1->SetBranchAddress("bunch2", &bunch2, &b_bunch2);
    tree1->SetBranchAddress("time", &time, &b_time);
    tree1->SetBranchAddress("nsep", &nsep, &b_nsep);
    tree1->SetBranchAddress("plane", &plane, &b_plane);
    tree1->SetBranchAddress("aqflag", &aqflag, &b_aqflag);
    tree1->SetBranchAddress("v0rate", &v0rate, &b_v0rate);

    Double_t N_1 = 0.0;
    Double_t N_2 = 0.0;
    Double_t v0rate_sum = 0.0;

    auto g = new TGraph();
    g->SetMarkerStyle(4);
    g->SetMarkerSize(1);
    g->SetMarkerColor(2);

    auto g1 = new TGraph();
    g1->SetMarkerStyle(4);
    g1->SetMarkerSize(1);
    g1->SetMarkerColor(3);

    auto mg1 = new TMultiGraph();

    auto g2 = new TGraph();
    g2->SetMarkerStyle(4);
    g2->SetMarkerSize(1);
    g2->SetMarkerColor(2);

    auto gun = new TGraph();
    gun->SetMarkerStyle(4);
    gun->SetMarkerSize(1);
    gun->SetMarkerColor(1);

    auto mg = new TMultiGraph();

    //----------------------------------------------------

    //__________________________________________________________16418
    Long64_t nentries = tree1->GetEntries();
    Long64_t gentry = 0;
    for (Long64_t jentry = 0; jentry < nentries; jentry++)
    {
        N_1 = 0.0;
        N_2 = 0.0;
        //cout << "This entry in tree1:\t" << jentry << endl;
        tree1->GetEntry(jentry);
        if (time >= 1501174150)
        {
            break;
        }
        N_1 = accumulate(bunch1, bunch1 + 3564, N_1);
        N_2 = accumulate(bunch2, bunch2 + 3564, N_2);
        g->SetPoint(gentry, time, N_1);
        g1->SetPoint(gentry, time, N_2);
        gentry++;
    }

    g->Fit("pol1", "Q");
    TF1 *pol1 = g->GetFunction("pol1");

    g1->Fit("pol1", "Q");
    TF1 *pol11 = g1->GetFunction("pol1");

    Double_t N_1_corr;
    Double_t N_2_corr;

    vector<double> n1;
    vector<double> n2;

    gentry = 0;
    for (Long64_t jentry = 0; jentry < nentries; jentry++)
    {
        N_1 = 0.0;
        N_2 = 0.0;
        //cout << "This entry in tree1:\t" << jentry << endl;
        tree1->GetEntry(jentry);
        if (time >= 1501174150)
        {
            break;
        }
        N_1 = accumulate(bunch1, bunch1 + 3564, N_1);
        N_2 = accumulate(bunch2, bunch2 + 3564, N_2);
        N_1_corr = N_1 + pol1->GetParameter(1) * (1501171000 - time);
        N_2_corr = N_2 + pol11->GetParameter(1) * (1501171000 - time);
        n1.push_back(N_1_corr);
        n2.push_back(N_2_corr);
        g2->SetPoint(gentry, time, N_1_corr);
        gun->SetPoint(gentry, time, N_1);
        gentry++;
    }
    Double_t sum = 0.0;
    cout << "N_1:\t" << accumulate(n1.begin(), n1.end(), sum) / n1.size() << endl;
    sum = 0.0;
    cout << "N_2:\t" << accumulate(n2.begin(), n2.end(), sum) / n2.size() << endl;

    mg->Add(g2);
    mg->Add(gun);
    mg->GetXaxis()->SetTimeOffset(0, "gmt");
    mg->GetXaxis()->SetTimeDisplay(1);
    mg->GetXaxis()->SetLabelOffset(0.02);
    mg->GetXaxis()->SetTimeFormat("#splitline{%H}{%M}");
    mg1->Add(g);
    mg1->Add(g1);
    mg1->GetXaxis()->SetTimeOffset(0, "gmt");
    mg1->GetXaxis()->SetTimeDisplay(1);
    mg1->GetXaxis()->SetLabelOffset(0.02);
    mg1->GetXaxis()->SetTimeFormat("#splitline{%H}{%M}");
    mg1->GetYaxis()->SetTitle("V0 rate");
    mg->GetYaxis()->SetTitle("V0 rate");
    auto c = new TCanvas();
    c->Divide(2, 1);
    c->cd(1);
    mg1->Draw("AP");
    c->cd(2);
    mg->Draw("AP");
}