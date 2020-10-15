#include "analysis_base.hpp"

void analysis::Loop()
{

    if (fChain == 0)
        return;

    Long64_t nentries = fChain->GetEntriesFast();
    auto myfile = new TFile(outfile_name.c_str(), "UPDATE");
    TDirectory *subdir = (gDirectory->FindObjectAny("lumi_cur")) ? (TDirectory *)gDirectory->FindObjectAny("lumi_cur") : myfile->mkdir("lumi_cur");
    subdir->cd();

    auto c = new TCanvas((channel_name + "_" + measurement_type).c_str(),
                         (channel_name + "_" + measurement_type).c_str(),
                         10, 10, 1600, 900);
    c->Divide(2, 1);
    auto *grs = new TMultiGraph();
    auto *gr45 = new TMultiGraph();
    auto g0 = new TGraph();
    auto g1 = new TGraph();
    auto g2 = new TGraph();
    auto g3 = new TGraph();
    auto g4 = new TGraph();
    auto g5 = new TGraph();
    Long64_t gentry = 0;
    Double_t current_cor = 0;
    TTimeStamp *ttime = new TTimeStamp();
    Long64_t nbytes = 0, nb = 0;
    for (Long64_t jentry = 0; jentry < nentries; jentry++)
    {

        Long64_t ientry = LoadTree(jentry);
        if (ientry < 0)
        {
            break;
        }
        nb = fChain->GetEntry(jentry);
        nbytes += nb;
        // if (Cut(ientry) < 0) continue;
        ttime->SetSec(fSec);
        ttime->SetNanoSec(fNanoSec);
        current_cor = (current - offset < 0) ? 0 : current - offset;
        g0->SetPoint(gentry, ttime->AsDouble(), HV);
        g1->SetPoint(gentry, ttime->AsDouble(), current_cor);
        g2->SetPoint(gentry, ttime->AsDouble(), T0_Luminosity);
        g3->SetPoint(gentry, ttime->AsDouble(), Luminosity);
        g4->SetPoint(gentry, Luminosity, current_cor);
        g5->SetPoint(gentry, T0_Luminosity, current_cor);
        gentry++;
    }
    // Find average HV
    g0->Fit("pol0", "Q");
    mean_hv = g0->GetFunction("pol0")->GetParameter(0);

    c->cd(1);
    TPad *pad1 = new TPad("pad1", "", 0, 0, 1, 1);
    TPad *pad2 = new TPad("pad2", "", 0, 0, 1, 1);
    pad1->Draw();
    pad1->cd();
    g1->SetMarkerStyle(20);
    g1->SetMarkerSize(0.5);
    g1->SetMarkerColorAlpha(1, 1);

    g1->GetXaxis()->SetTimeOffset(0, "gmt");
    g1->GetXaxis()->SetTimeDisplay(1);
    g1->GetXaxis()->SetLabelOffset(0.02);
    g1->GetXaxis()->SetTimeFormat("#splitline{%Y}{#splitline{%d\/%m}{%H\:%M}}");
    g1->GetXaxis()->SetTitle("");
    g1->GetYaxis()->SetTitle("Current (A)");
    g1->Draw("AP");

    pad2->SetFrameFillStyle(0);
    pad2->SetFillStyle(4000);
    pad2->Draw();
    pad2->cd();
    g2->SetMarkerStyle(20);
    g2->SetMarkerSize(0.5);
    g2->SetMarkerColorAlpha(4, 0.4);
    g3->SetMarkerStyle(7);
    g3->SetMarkerSize(0.5);
    g3->SetMarkerColorAlpha(3, 0.35);
    grs->Add(g2);
    grs->Add(g3);
    grs->SetTitle(channel_name.c_str());
    grs->GetXaxis()->SetTimeOffset(0, "gmt");
    grs->GetXaxis()->SetTimeDisplay(1);
    grs->GetXaxis()->SetLabelOffset(0.02);
    grs->GetXaxis()->SetTimeFormat("#splitline{%Y}{#splitline{%d\/%m}{%H\:%M}}");
    grs->GetXaxis()->SetTitle("");
    grs->GetYaxis()->SetTitle("");
    grs->Draw("AP");

    // Draw the axis of the 2nd TMultiGraph
    TAxis *mg2Xaxis = grs->GetHistogram()->GetXaxis();
    TAxis *mg2Yaxis = grs->GetHistogram()->GetYaxis();
    Double_t xmin = mg2Xaxis->GetXmin();
    Double_t xmax = mg2Xaxis->GetXmax();
    Double_t ymin = mg2Yaxis->GetXmin();
    Double_t ymax = mg2Yaxis->GetXmax();
    mg2Xaxis->SetLabelSize(0);
    mg2Xaxis->SetTickLength(0);
    mg2Yaxis->SetLabelSize(0);
    mg2Yaxis->SetTickLength(0);
    TGaxis *yaxis = new TGaxis(xmax, ymin, xmax, ymax, ymin, ymax, 510, "+L");
    TGaxis *xaxis = new TGaxis(xmin, ymax, xmax, ymax, xmin, xmax, 510, "-L");
    xaxis->SetLabelColor(kRed);
    yaxis->SetTextColor(kRed);
    xaxis->SetTimeOffset(0, "gmt");
    xaxis->SetLabelOffset(0.005);
    xaxis->SetTimeFormat("%H\:%M");
    xaxis->SetTitle("");
    yaxis->SetTitle("Luminostiy (HZ/#mub)");
    xaxis->Draw();
    yaxis->SetLabelColor(kRed);
    yaxis->Draw();

    //Legend
    auto legend = new TLegend(0.55, 0.15, 0.8, 0.3);
    legend->SetHeader("small offset in times", "C"); // option "C" allows to center the header
    legend->AddEntry(g1, "current", "lp");
    legend->AddEntry(g2, "T0 Luminosity", "lp");
    legend->AddEntry(g3, "ECal Luminosity", "lp");
    legend->Draw();

    c->cd(2);
    g4->SetMarkerColorAlpha(3, 0.2);
    g4->SetMarkerStyle(20);

    g4->SetName("T0 Luminosity");
    g5->SetName("ECal Luminosity");

    g5->SetMarkerColorAlpha(4, 0.2);
    g5->SetMarkerStyle(22);

    t0_lumi_gr = new TGraph(*g4);
    tof_lumi_gr = new TGraph(*g5);

    gr45->Add(g4);
    gr45->Add(g5);
    gr45->GetXaxis()->SetTitle("Luminostiy (HZ/#mub)");
    gr45->GetYaxis()->SetTitle("Current (A)");
    gr45->SetTitle("Luminosity Current");
    gr45->Draw("AP");
    auto legend1 = new TLegend(0.55, 0.15, 0.8, 0.3);
    legend1->AddEntry(g4, "T0 Luminosity", "lp");
    legend1->AddEntry(g5, "ECal Luminosity", "lp");
    legend1->Draw();

    c->Write();
    //g4->Write();
    //g5->Write();

    myfile->Write();
    myfile->Close();
    if (g1)
        delete g1;
    if (g2)
        delete g2;
    if (g3)
        delete g3;
    if (g4)
        delete g4;
    if (g5)
        delete g5;
    if (c)
        delete c;
    if (legend)
        delete legend;
    if (legend1)
        delete legend1;
}

analysis::analysis(const std::string filename, const std::string outfile, const std::string channel, const Int_t offset_start, const Int_t offset_end)
{
    outfile_name = outfile;
    channel_name = channel;
    offset_start_time = offset_start;
    offset_end_time = offset_end;
    TTree *tree = 0;
    auto f = new TFile(filename.c_str(), "READ");
    if (!f || !f->IsOpen())
    {
        std::cerr << "cannot open file:\t" << filename << std::endl;
    }
    f->GetObject("Tree_TRD_HV", tree);

    InitOffset(tree);
}

analysis::analysis(const std::string filename, const std::string outfile, const std::string channel, const char measurement, const Double_t _offset) : fChain(0)
{
    outfile_name = outfile;
    channel_name = channel;
    measurement_type = measurement;
    offset = _offset;
    TTree *tree = 0;
    auto f = new TFile(filename.c_str(), "READ");
    if (!f || !f->IsOpen())
    {
        std::cerr << "cannot open file:\t" << filename << std::endl;
    }
    f->GetObject("TOF_T0_Tree", tree);

    Init(tree);
}

analysis::~analysis()
{
    if (!fChain)
        return;
    delete fChain->GetCurrentFile();
}

void analysis::Offset()
{
    auto myfile = new TFile(outfile_name.c_str(), "UPDATE");
    TDirectory *subdir = (gDirectory->FindObjectAny("offset")) ? (TDirectory *)gDirectory->FindObjectAny("offset") : myfile->mkdir("offset");
    subdir->cd();
    auto c = new TCanvas(channel_name.c_str(), ("Offset @" + channel_name).c_str(), 1600, 900);
    // Called inside Loop
    auto g0 = new TGraph();
    TTimeStamp *ttime = new TTimeStamp();

    Long64_t nentries = fChain->GetEntriesFast();
    Long64_t nbytes = 0, nb = 0;
    Long64_t gentry = 0;
    for (Long64_t jentry = 0; jentry < nentries; jentry++)
    {
        Long64_t ientry = LoadTree(jentry);
        if (ientry < 0)
            break;
        nb = fChain->GetEntry(jentry);
        nbytes += nb;
        if (fSec < offset_start_time)
        {
            continue;
        }
        if (fSec > offset_end_time)
        {
            break;
        }
        ttime->SetSec(fSec);
        ttime->SetNanoSec(fNanoSec);
        g0->SetPoint(gentry, ttime->AsDouble(), HV);
        gentry++;
    }
    Int_t date_time = offset_start_time;
    TTimeStamp time_stamp;
    time_stamp.SetSec(date_time);
    std::string title = "Offset current" + ((std::string)time_stamp.AsString()).substr(5, 11);
    g0->GetXaxis()->SetTimeOffset(0, "gmt");
    g0->GetXaxis()->SetTimeDisplay(1);
    g0->GetXaxis()->SetLabelOffset(0.02);
    g0->GetXaxis()->SetTimeFormat("{%H\:%M}}");
    g0->GetYaxis()->SetTitle("offset current (A)");
    g0->SetTitle(title.c_str());
    g0->Fit("pol0", "Q");
    offset = g0->GetFunction("pol0")->GetParameter(0);
    g0->Draw("AP");
    c->Write();
    myfile->Write();
    myfile->Close();
}

Int_t analysis::GetEntry(const Long64_t entry)
{
    // Read contents of entry.
    if (!fChain)
        return 0;
    return fChain->GetEntry(entry);
}
Long64_t analysis::LoadTree(const Long64_t entry)
{
    // Set the environment to read one entry
    if (!fChain)
        return -5;
    Long64_t centry = fChain->LoadTree(entry);
    if (centry < 0)
        return centry;
    if (fChain->GetTreeNumber() != fCurrent)
    {
        fCurrent = fChain->GetTreeNumber();
        Notify();
    }
    return centry;
}

void analysis::Init(TTree *tree)
{
    // The Init() function is called when the selector needs to initialize
    // a new tree or chain. Typically here the branch addresses and branch
    // pointers of the tree will be set.
    // It is normally not necessary to make changes to the generated
    // code, but the routine can be extended by the user if needed.
    // Init() will be called many times when running on PROOF
    // (once per file to be processed).

    // Set branch addresses and branch pointers
    if (!tree)
        return;
    fChain = tree;
    fCurrent = -1;
    fChain->SetMakeClass(1);

    fChain->SetBranchAddress("HV", &HV, &b_HV);
    fChain->SetBranchAddress("fSec", &fSec, &b_time_fSec);
    fChain->SetBranchAddress("fNanoSec", &fNanoSec, &b_time_fNanoSec);
    fChain->SetBranchAddress("TOF_average_current", &TOF_average_current, &b_TOF_average_current);
    fChain->SetBranchAddress("TOF_rms_current", &TOF_rms_current, &b_TOF_rms_current);
    fChain->SetBranchAddress("Luminosity", &Luminosity, &b_Luminosity);
    fChain->SetBranchAddress("T0_Luminosity", &T0_Luminosity, &b_T0_Luminosity);
    fChain->SetBranchAddress("current", &current, &b_current);
    Notify();
}

void analysis::InitOffset(TTree *tree)
{
    if (!tree)
        return;
    fChain = tree;
    fCurrent = -1;
    fChain->SetMakeClass(1);

    fChain->SetBranchAddress("HV", &HV, &b_HV);
    fChain->SetBranchAddress("fSec", &fSec, &b_time_fSec);
    fChain->SetBranchAddress("fNanoSec", &fNanoSec, &b_time_fNanoSec);
    Notify();
}

Bool_t analysis::Notify()
{
    // The Notify() function is called when a new file is opened. This
    // can be either for a new TTree in a TChain or when when a new TTree
    // is started when using PROOF. It is normally not necessary to make changes
    // to the generated code, but the routine can be extended by the
    // user if needed. The return value is currently not used.

    return kTRUE;
}

void analysis::Show(const Long64_t entry)
{
    // Print contents of entry.
    // If entry is not specified, print current entry
    if (!fChain)
        return;
    fChain->Show(entry);
}
Int_t analysis::Cut(const Long64_t entry)
{
    // This function may be called from Loop.
    // returns  1 if entry is accepted.
    // returns -1 otherwise.
    return 1;
}

Double_t analysis::mean(const std::vector<Double_t> *v)
{
    Double_t sum = 0, n = 0;
    for (auto &&element : *v)
    {
        sum += element;
        n++;
    }
    return sum / n;
}