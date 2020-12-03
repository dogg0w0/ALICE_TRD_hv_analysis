void all_pres_lumi()
{
    gROOT->SetStyle("ATLAS");
    gROOT->ForceStyle();

    Double_t mean_current1, std_current1, Luminosity1, pressure1;
    Double_t mean_current2, std_current2, Luminosity2, pressure2;
    TBranch *mean_current1Branch = 0;
    TBranch *std_current1Branch = 0;
    TBranch *Luminosity1Branch = 0;
    TBranch *pressure1Branch = 0;
    TBranch *mean_current2Branch = 0;
    TBranch *std_current2Branch = 0;
    TBranch *Luminosity2Branch = 0;
    TBranch *pressure2Branch = 0;
    Long64_t nentries1, nentries2;

    TTree *tree1 = 0;
    TTree *tree2 = 0;

    TFile *f1 = TFile::Open("/home/doggo/Downloads/ALICE_TRD/create_csv/output_all_withWights_pres/date_1/current_lumi.csv.root", "READ");
    TFile *f2 = TFile::Open("/home/doggo/Downloads/ALICE_TRD/create_csv/output_all_withWights_pres/date_2/current_lumi.csv.root", "READ");

    tree1 = (TTree *)f1->Get("TOF_T0_Tree");
    tree1->SetMakeClass(1);
    tree1->SetBranchAddress("mean_current", &mean_current1, &mean_current1Branch);
    tree1->SetBranchAddress("std_current", &std_current1, &std_current1Branch);
    tree1->SetBranchAddress("Luminosity", &Luminosity1, &Luminosity1Branch);
    tree1->SetBranchAddress("pressure", &pressure1, &pressure1Branch);
    nentries1 = tree1->GetEntries();

    tree2 = (TTree *)f2->Get("TOF_T0_Tree");
    tree2->SetMakeClass(1);
    tree2->SetBranchAddress("mean_current", &mean_current2, &mean_current2Branch);
    tree2->SetBranchAddress("std_current", &std_current2, &std_current2Branch);
    tree2->SetBranchAddress("Luminosity", &Luminosity2, &Luminosity2Branch);
    tree2->SetBranchAddress("pressure", &pressure2, &pressure2Branch);
    nentries2 = tree2->GetEntries();

    Int_t gentry = 0;
    Int_t gentry1 = 0;
    Int_t gentry2 = 0;
    auto g1 = new TGraph();
    auto g2 = new TGraph();
    auto g12 = new TMultiGraph();
    auto g3 = new TGraph();
    auto g4 = new TGraph();
    auto g34 = new TMultiGraph();
    auto g5 = new TGraph();
    auto g6 = new TGraph();
    auto g56 = new TMultiGraph();

    for (Long64_t i = 0; i < nentries1; i++)
    {
        tree1->GetEntry(i);
        for (Long64_t j = 0; j < nentries2; j++)
        {
            tree2->GetEntry(j);
            if (TMath::Abs(mean_current1 - mean_current2) < 0.0001)
            {
                if (pressure2 - pressure1 > 0.45)
                {
                    g1->SetPoint(gentry1, pressure2 - pressure1, Luminosity2 - Luminosity1);
                    g3->SetPoint(gentry1, gentry, Luminosity1);
                    g5->SetPoint(gentry1, gentry, pressure2 - pressure1);
                    gentry1++;
                }
                else
                {
                    g2->SetPoint(gentry2, pressure2 - pressure1, Luminosity2 - Luminosity1);
                    g4->SetPoint(gentry2, gentry, Luminosity1);
                    g6->SetPoint(gentry2, gentry, pressure2 - pressure1);
                    gentry2++;
                }

                gentry++;
            }
        }
    }
    cout << gentry << endl;
    auto c = new TCanvas();
    c->Divide(2, 1);
    c->cd(1);
    g1->SetMarkerColor(kRed);
    g2->SetMarkerColor(kBlue);
    g12->Add(g1);
    g12->Add(g2);
    g12->GetXaxis()->SetTitle("#Deltap (mbar)");
    g12->GetYaxis()->SetTitle("#DeltaLumi (Hz/#mub)");
    g12->Draw("AP");
    TVirtualPad *pad = c->GetPad(2);
    pad->Divide(1, 2);
    pad->cd(1);
    g3->SetMarkerColor(kRed);
    g4->SetMarkerColor(kBlue);
    g34->Add(g3);
    g34->Add(g4);
    g34->GetXaxis()->SetTitle("time (a.u.)");
    g34->GetYaxis()->SetTitle("Luminosity (Hz/#mub)");
    g34->Draw("AP");
    pad->cd(2);
    g5->SetMarkerColor(kRed);
    g6->SetMarkerColor(kBlue);
    g56->Add(g5);
    g56->Add(g6);
    g56->GetXaxis()->SetTitle("time (a.u.)");
    g56->GetYaxis()->SetTitle("#Deltap (mbar)");
    g56->Draw("AP");
}