void getAnodeCurrent(string filename)
{
    // Declaration of leaf types
    Double_t HV;
    //TTimeStamp      *time;
    Int_t fSec;
    Int_t fNanoSec;

    // List of branches
    TBranch *b_HV;            //!
    TBranch *b_time_fSec;     //!
    TBranch *b_time_fNanoSec; //!

    TTimeStamp *ttime = new TTimeStamp();

    TFile *f2 = TFile::Open(filename.c_str(), "READ");
    TTree *tree2 = new TTree();
    tree2 = (TTree *)f2->Get("Tree_TRD_HV");
    tree2->SetMakeClass(1);
    tree2->SetBranchAddress("HV", &HV, &b_HV);
    tree2->SetBranchAddress("fSec", &fSec, &b_time_fSec);
    tree2->SetBranchAddress("fNanoSec", &fNanoSec, &b_time_fNanoSec);

    auto g = new TGraph();
    g->SetMarkerStyle(4);
    g->SetMarkerSize(1);
    g->SetMarkerColor(2);

    Int_t time1 = 1501170800, time2 = 1501171400,
          time3 = 1501172624, time4 = 1501172753,
          time5 = 1501173900, time6 = 1501173960;

    Long64_t nentries2 = tree2->GetEntries();
    Long64_t gentry = 0;

    for (Long64_t ientry = 0; ientry < nentries2; ientry++)
    {
        tree2->GetEntry(ientry);
        ttime->SetSec(fSec);
        ttime->SetNanoSec(fNanoSec);
        Double_t time = ttime->AsDouble();
        if ((time >= time1 and time <= time2) or (time >= time3 and time <= time4) or (time >= time5 and time <= time6))
        {
            g->SetPoint(gentry, ttime->AsDouble(), HV);
            gentry++;
        }
    }

    g->Fit("pol1", "Q");
    TF1 *pol1 = g->GetFunction("pol1");

    Int_t fSec1, fNanoSec1;
    Double_t HV1;

    auto gcor = new TGraph();
    auto f = new TFile("cor.root", "RECREATE");
    TTree *tree = new TTree("Tree_TRD_HV_cor", "corrected data");
    tree->Branch("fSec", &fSec1, "fSec/I");
    tree->Branch("fNanoSec", &fNanoSec1, "fNanoSec/I");
    tree->Branch("HV", &HV1, "HV/D");
    Double_t HV_cor;
    gentry = 0;
    for (Long64_t ientry = 0; ientry < nentries2; ientry++)
    {
        HV_cor = 0.0;
        tree2->GetEntry(ientry);
        if (fSec <= time1)
        {
            continue;
        }
        if (fSec >= 1501174150)
        {
            break;
        }
        ttime->SetSec(fSec);
        ttime->SetNanoSec(fNanoSec);
        fSec1 = fSec;
        fNanoSec1 = fNanoSec;
        HV_cor = HV + pol1->GetParameter(1) * (1501171000 - ttime->AsDouble());
        HV1 = HV;
        tree->Fill();
        gcor->SetPoint(gentry, ttime->AsDouble(), HV_cor);
        gentry++;
    }
    f->Write();
    f->Close();
    auto c = new TCanvas();
    c->Divide(2, 1);
    c->cd(1);
    g->Draw("AP");
    c->cd(2);
    gcor->Draw("AP");
}