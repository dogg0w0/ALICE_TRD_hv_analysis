void all_pres()
{
    gROOT->SetStyle("Pub");

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
    auto g1 = new TGraph();
    auto g2 = new TGraph();
    auto g21 = new TGraph();
    auto g22 = new TGraph();

    auto g12 = new TMultiGraph();
    auto g3 = new TGraph();
    auto g4 = new TGraph();
    auto g5 = new TGraph();
    auto g6 = new TGraph();
    auto g7 = new TGraph();
    auto g67 = new TMultiGraph();

    auto ggreen = new TGraph();
    auto gred = new TGraph();


    auto hist1 = new TH1D("hist1", "T;#DeltaA;entries", 100, -0.15, 0.15);
    auto hist2 = new TH1D("hist2", "T;#DeltaA;entries", 100, -0.15, 0.15);
    auto stack = new THStack();

    for (Long64_t i = 0; i < nentries1; i++)
    {
        tree1->GetEntry(i);
        for (Long64_t j = 0; j < nentries2; j++)
        {
            tree2->GetEntry(j);
            if (TMath::Abs(Luminosity1 - Luminosity2) < 0.005)
            {
                g1->SetPoint(gentry, gentry, pressure1);
                g3->SetPoint(gentry, gentry, mean_current1 - mean_current2);
                g4->SetPoint(gentry, gentry, Luminosity1);
                if (gentry < 650)
                {
                    g6->SetPoint(gentry, pressure2 - pressure1, mean_current2 - mean_current1);
                    g21->SetPoint(gentry, gentry, pressure2);
                    hist1->Fill(mean_current1 - mean_current2);
                    ggreen->SetPoint(gentry,gentry, pressure2-pressure1);
                }
                else
                {
                    g7->SetPoint(gentry - 650, pressure2 - pressure1, mean_current2 - mean_current1);
                    g22->SetPoint(gentry - 650, gentry, pressure2);
                    hist2->Fill(mean_current1 - mean_current2);
                    gred->SetPoint(gentry,gentry, pressure2 - pressure1);
                }

                gentry++;
            }
        }
    }
    // cout << gentry << endl;
    // g1->SetMarkerColor(kRed);
    // g21->SetMarkerColor(kGreen);
    // g22->SetMarkerColor(kBlue);
    // g12->Add(g1);
    // g12->Add(g21);
    // g12->Add(g22);
    // g12->GetXaxis()->SetTitle("entry");
    // g12->GetYaxis()->SetTitle("pressure (mbar)");
    // g4->GetXaxis()->SetTitle("entry");
    // g4->GetYaxis()->SetTitle("Luminosity (Hz/#mub)");

//     auto c1 = new TCanvas("c1", "Canvas", 10, 10, 800, 600);
//     c1->Divide(2,2);
//     c1->cd(1);
//     g3->GetXaxis()->SetTitle("entry");
//     g3->GetYaxis()->SetTitle("difference in current at same Luminosity (#muA)");
//     g3->Draw("AP");
//     c1->cd(2);
//     g12->Draw("AP");
//     c1->cd(3);
//     hist1->SetFillColor(kRed);
//     hist2->SetFillColor(kGreen);
//     stack->Add(hist2);

//     stack->Add(hist1);
//     stack->Draw();
//     //g4->Draw("AP");
//     //g12->Draw("AP");
//     //g1->Draw("AP");
//     //g2->Draw("P SAME");
//     c1->cd(4);
//     //g6->GetYaxis()->SetTitle("difference in current at same Luminosity (#muA)");
//     //g6->GetXaxis()->SetTitle("difference in pressure at same Luminosity (#muA)");
//     //g6->Draw("AP");
//     g6->SetMarkerColor(kGreen);
//     g7->SetMarkerColor(kBlue);
//     g67->Add(g6);
//     g67->Add(g7);
//     g67->GetYaxis()->SetTitle("#DeltaA at same Luminosity (#muA)");
//     g67->GetXaxis()->SetTitle("#Deltap at same Luminosity (#muA)");
//     g67->Draw("AP");
//     c1->Draw();
    gred->SetMarkerColor(kRed);
    gred->SetMarkerSize(100);
    gred->SetDrawOption("P");
    ggreen->SetMarkerColor(kGreen);
    ggreen->SetMarkerSize(10);
    g12->Add(gred);
    g12->Add(ggreen);
    g12->GetXaxis()->SetTitle("Time (a.u.)");
    g12->GetYaxis()->SetTitle("#Deltap (mbar)");
    auto c1 = new TCanvas("c1", "Canvas", 10, 10, 1000, 3000);
    c1->Divide(1, 3);
    c1->cd(1);
    g12->Draw("AP");
    c1->cd(2);
    hist1->SetFillColor(kRed);
    hist2->SetFillColor(kGreen);

    stack->Add(hist2);

    stack->Add(hist1);
    stack->Draw();
    stack->GetXaxis()->SetTitle("#DeltaA (#muA)");
    stack->GetYaxis()->SetTitle("entries");

    //g4->Draw("AP");
    //g12->Draw("AP");
    //g1->Draw("AP");
    //g2->Draw("P SAME");
    c1->cd(3);
    //g6->GetYaxis()->SetTitle("difference in current at same Luminosity (#muA)");
    //g6->GetXaxis()->SetTitle("difference in pressure at same Luminosity (#muA)");
    //g6->Draw("AP");
    g6->SetMarkerColor(kGreen);
    g7->SetMarkerColor(kBlue);
    g67->Add(g6);
    g67->Add(g7);
    g67->GetYaxis()->SetTitle("#DeltaA at same Luminosity (#muA)");
    g67->GetXaxis()->SetTitle("#Deltap at same Luminosity (mbar)");
    g67->Draw("AP");
    c1->Draw();
}
