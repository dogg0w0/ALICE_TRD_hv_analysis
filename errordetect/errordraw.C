void errordraw(){
    gROOT->SetStyle("Pub");
    // Declaration of leaf types
    Double_t HV;
    //TTimeStamp      *time;
    Int_t fSec;
    Int_t fNanoSec;

    // List of branches
    TBranch *b_HV;            //!
    TBranch *b_time_fSec;     //!
    TBranch *b_time_fNanoSec; //!

    TFile *f1 = TFile::Open("/media/felix/flush/ALICE_TRD/hv_ALICE_Data/sorted_486.csv.root", "READ");
    TTree *tree1 = new TTree();
    tree1 = (TTree *)f1->Get("Tree_TRD_HV");
    tree1->SetMakeClass(1);
    tree1->SetBranchAddress("HV", &HV, &b_HV);
    tree1->SetBranchAddress("fSec", &fSec, &b_time_fSec);
    tree1->SetBranchAddress("fNanoSec", &fNanoSec, &b_time_fNanoSec);

    auto g = new TGraph();

    Long64_t nentries = tree1->GetEntries();
    //auto myfile = new TFile("output.root", "UPDATE");
    TTimeStamp *ttime = new TTimeStamp();
    Long64_t gentry = 0;

    int _temp;

    for (Long64_t jentry = 0; jentry < nentries; jentry++)
    {
        tree1->GetEntry(jentry);
        if (fSec < 1521057610)
        {
            continue;
        }
        if (fSec > 1521086341)
            break;
        ttime->SetSec(fSec);
        ttime->SetNanoSec(fNanoSec);
        g->SetPoint(gentry, ttime->AsDouble(), HV);
        gentry++;
    }

    TFile *f2 = TFile::Open("/media/felix/flush/ALICE_TRD/hv_ALICE_Data/sorted_485.csv.root", "READ");
    TTree *tree2 = new TTree();
    tree2 = (TTree *)f2->Get("Tree_TRD_HV");
    tree2->SetMakeClass(1);
    tree2->SetBranchAddress("HV", &HV, &b_HV);
    tree2->SetBranchAddress("fSec", &fSec, &b_time_fSec);
    tree2->SetBranchAddress("fNanoSec", &fNanoSec, &b_time_fNanoSec);

    auto s = new TGraph();

     nentries = tree2->GetEntries();
    gentry = 0;
    for (Long64_t jentry = 0; jentry < nentries; jentry++)
    {
        tree2->GetEntry(jentry);
        if (fSec < 1521057610)
        {
            continue;
        }
        if (fSec > 1521086341)
            break;
        ttime->SetSec(fSec);
        ttime->SetNanoSec(fNanoSec);
        s->SetPoint(gentry, ttime->AsDouble(), HV);
        gentry++;
    }

    TCanvas *c = new TCanvas("c", "multigraphs on same pad", 200, 10, 700, 500);
    TPad *pad1 = new TPad("pad1", "", 0, 0, 1, 1);
    TPad *pad2 = new TPad("pad2", "", 0, 0, 1, 1);

    s->SetLineColor(kBlue);
    g->SetLineColor(kRed);

    // Draw the 1st TMultiGraph
    pad1->Draw();
    pad1->cd();

    TMultiGraph *mg1 = new TMultiGraph();
    mg1->Add(g);
    mg1->GetYaxis()->SetLabelColor(kRed);
    mg1->Draw("AL");

    mg1->GetXaxis()->SetTimeOffset(0, "gmt");
    mg1->GetXaxis()->SetTimeDisplay(1);
    mg1->GetXaxis()->SetLabelOffset(0.02);
    mg1->GetXaxis()->SetTimeFormat("#splitline{%Y}{#splitline{%d\/%m}{%H\:%M}}");
    mg1->GetXaxis()->SetTitle("");
    mg1->GetYaxis()->SetTitle("Current (#muA)");

    // Draw the 2nd TMultiGraph
    pad2->SetFrameFillStyle(0);
    pad2->SetFillStyle(4000);
    pad2->Draw();
    pad2->cd();

    TMultiGraph *mg2 = new TMultiGraph();
    mg2->Add(s);
    mg2->Draw("AL");

    // Draw the axis of the 2nd TMultiGraph
    TAxis *mg2Xaxis = mg2->GetHistogram()->GetXaxis();
    TAxis *mg2Yaxis = mg2->GetHistogram()->GetYaxis();
    Double_t xmin = mg2Xaxis->GetXmin();
    Double_t xmax = mg2Xaxis->GetXmax();
    Double_t ymin = mg2Yaxis->GetXmin();
    Double_t ymax = mg2Yaxis->GetXmax();
    mg2Xaxis->SetLabelSize(0);
    mg2Xaxis->SetTickLength(0);
    mg2Yaxis->SetLabelSize(0);
    mg2Yaxis->SetTickLength(0);
    TGaxis *yaxis = new TGaxis(xmax, ymin, xmax, ymax, ymin, ymax, 510, "+L");
    yaxis->SetTitle("HV (V)");
    TGaxis *xaxis = new TGaxis(xmin, ymax, xmax, ymax, xmin, xmax, 510, "-L");
    xaxis->SetLabelColor(kBlue);
    //xaxis->Draw();
    yaxis->SetLabelColor(kBlue);
    yaxis->Draw();
}