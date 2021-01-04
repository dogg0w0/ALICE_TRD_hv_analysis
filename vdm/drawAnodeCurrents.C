void drawAnodeCurrents(string filename)
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
    g->SetMarkerStyle(8);
    g->SetMarkerSize(.5);
    g->SetMarkerColor(1);

    Long64_t nentries2 = tree2->GetEntries();
    Long64_t gentry = 0;

    for (Long64_t ientry = 0; ientry < nentries2; ientry++)
    {
        tree2->GetEntry(ientry);
        ttime->SetSec(fSec);
        ttime->SetNanoSec(fNanoSec);
        Double_t time = ttime->AsDouble();
        if (time < 1479915000)
        {
            continue;
        }
        if (time > 1479923000)
        {
            break;
        }
        
        g->SetPoint(gentry, ttime->AsDouble(), HV);
        gentry++;
    }

    auto c = new TCanvas();
    c->cd();
    g->GetXaxis()->SetTitle("");
    g->GetYaxis()->SetTitle("current (#muA)");
    g->GetXaxis()->SetTimeOffset(0, "gmt");
    g->GetXaxis()->SetTimeDisplay(1);
    g->GetXaxis()->SetLabelOffset(0.02);
    g->GetXaxis()->SetTimeFormat("#splitline{%Y}{#splitline{%d\/%m}{%H\:%M}}");
    g->Draw("AP");
}