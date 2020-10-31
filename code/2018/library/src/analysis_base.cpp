#include "analysis_base.hpp"

void analysis::Loop(Double_t weight_channel)
{

    if (fChain == 0)
        return;

    Long64_t nentries = fChain->GetEntriesFast();
    auto myfile = new TFile(outfile_name.c_str(), "UPDATE");
    myfile->mkdir("lumi_cur", "", true);
    myfile->mkdir("quality_assurance", "", true);
    myfile->cd("lumi_cur");

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
    auto g6 = new TGraph();
    auto gT0Errors = new TGraph();
    auto gECALErrors = new TGraph();
    auto dummyhist = new TH1D();
    auto dummygraphT0 = new TGraph();
    auto dummygraphECAL = new TGraph();
    auto current_tof_cur = new TGraph();

    Double_t delta = 1;
    Long64_t gentry = 0, gerrorsentryT0 = 0, gerrorsentryECAL = 0, g4entry = 0, g5entry = 0;
    Double_t current_cor = 0;
    TTimeStamp *ttime = new TTimeStamp();
    Long64_t nbytes = 0, nb = 0;
    Double_t maxLumi = 0;

    for (Long64_t jentry = 0; jentry < nentries; jentry++)
    {
        Long64_t ientry = LoadTree(jentry);
        if (ientry < 0)
            break;
        fChain->GetEntry(jentry);
        if (HV < 0)
            continue;
        // if (Cut(ientry) < 0) continue;
        dummygraphT0->SetPoint(gentry, Luminosity, current);
        dummygraphECAL->SetPoint(gentry, T0_Luminosity, current);
        gentry++;
    }

    // Mark Outliers
    dummygraphT0->Fit("pol1", "Q");
    dummygraphECAL->Fit("pol1", "Q");
    auto fT0 = dummygraphT0->GetFunction("pol1");
    auto fECAL = dummygraphECAL->GetFunction("pol1");

    gentry = 0;
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
        if (HV < 0)
            continue;

        current_cor = (current - offset < 0) ? 0 : (current - offset) * weight_channel;
        ttime->SetSec(fSec);
        ttime->SetNanoSec(fNanoSec);
        if (TMath::Abs(fECAL->Eval(Luminosity) - current) < delta)
        {
            g4->SetPoint(g4entry, Luminosity, current_cor);
            g4entry++;
        }
        else
        {
            gECALErrors->SetPoint(gerrorsentryECAL, Luminosity, current_cor);
            gerrorsentryECAL++;
        }

        if (TMath::Abs(fT0->Eval(T0_Luminosity) - current) < delta)
        {
            g5->SetPoint(g5entry, T0_Luminosity, current_cor);
            g5entry++;
        }
        else
        {
            gT0Errors->SetPoint(gerrorsentryT0, T0_Luminosity, current_cor);
            gerrorsentryT0++;
        }
        if (TMath::Abs(fT0->Eval(T0_Luminosity) - current) < delta && TMath::Abs(fECAL->Eval(Luminosity) - current) < delta)
        {
            g0->SetPoint(gentry, ttime->AsDouble(), HV);
            g1->SetPoint(gentry, ttime->AsDouble(), current_cor);
            g2->SetPoint(gentry, ttime->AsDouble(), T0_Luminosity);
            g3->SetPoint(gentry, ttime->AsDouble(), Luminosity);
            g6->SetPoint(gentry, T0_Luminosity, Luminosity);
            current_tof_cur->SetPoint(gentry, current_cor, TOF_average_current);
            gentry++;
            if (maxLumi < Luminosity)
                maxLumi = Luminosity;
        }
    }
    g4->SetPoint(gentry, 0, 0);
    g5->SetPoint(gentry, 0, 0);
    // Find average HV
    g0->Fit("pol0", "Q");
    mean_hv = g0->GetFunction("pol0")->GetParameter(0);

    c->cd(1);
    dummyhist->Draw();
    g2->SetMarkerStyle(20);
    g2->SetMarkerSize(0.7);
    g2->SetMarkerColor(8);
    g3->SetMarkerStyle(21);
    g3->SetMarkerSize(0.7);
    g3->SetMarkerColor(9);
    grs->Add(g2);
    grs->Add(g3);
    grs->SetTitle(channel_name.c_str());
    grs->GetXaxis()->SetTimeOffset(0, "gmt");
    grs->GetXaxis()->SetTimeDisplay(1);
    grs->GetXaxis()->SetLabelOffset(0.02);
    grs->GetXaxis()->SetTimeFormat("#splitline{%H}{%M}");
    grs->GetXaxis()->SetTitle("Time #splitline{H}{M}");
    grs->GetYaxis()->SetTitle("Luminostiy (HZ/#mub)  #color[2]{Current (A)}");
    grs->Draw("AP SAME");

    g1->SetMarkerStyle(2);
    g1->SetMarkerSize(0.7);
    g1->SetMarkerColor(kRed);

    g1->GetXaxis()->SetTimeOffset(0, "gmt");
    g1->GetXaxis()->SetTimeDisplay(1);
    g1->GetXaxis()->SetLabelOffset(0.03);
    g1->GetXaxis()->SetTimeFormat("#splitline{%H}{%M}");
    //g1->GetXaxis()->SetTitle("");
    //g1->GetYaxis()->SetTitle("Current (A)");
    g1->Draw("P SAME");

    //Legend
    auto legend = new TLegend(0.55, 0.3, 0.8, 0.45);
    //legend->SetHeader("small offset in times","C"); // option "C" allows to center the header
    legend->AddEntry(g1, "current", "p");
    legend->AddEntry(g2, "T0 Luminosity", "p");
    legend->AddEntry(g3, "ECal Luminosity", "p");
    legend->Draw();

    c->cd(2);
    g4->SetMarkerColor(8);
    g4->SetMarkerSize(0.5);
    g4->SetMarkerStyle(20);

    g5->SetMarkerColor(9);
    g5->SetMarkerSize(0.5);
    g5->SetMarkerStyle(21);

    t0_lumi_gr = new TGraph(*g4);
    tof_lumi_gr = new TGraph(*g5);

    g4->Fit("pol1", "Q");
    g5->Fit("pol1", "Q");
    g4->GetFunction("pol1")->SetLineColor(8);
    g5->GetFunction("pol1")->SetLineColor(9);

    gT0Errors->SetMarkerColor(kRed);
    gT0Errors->SetMarkerSize(0.3);
    gT0Errors->SetMarkerStyle(3);

    gECALErrors->SetMarkerColor(kRed);
    gECALErrors->SetMarkerSize(0.3);
    gECALErrors->SetMarkerStyle(4);

    gr45->Add(g4);
    gr45->Add(g5);
    gr45->Add(gT0Errors);
    gr45->Add(gECALErrors);
    gr45->SetTitle("Luminosity Current Correlation");
    gr45->GetXaxis()->SetTitle("Luminostiy (HZ/#mub)");
    gr45->GetYaxis()->SetTitle("Current (A)");
    gr45->Draw("AP");
    auto legend1 = new TLegend(0.55, 0.3, 0.8, 0.45);
    legend1->AddEntry(g4, "T0 Luminosity", "p");
    legend1->AddEntry(g5, "ECal Luminosity", "p");
    legend1->AddEntry(gT0Errors, "T0 Outliers", "p");
    legend1->AddEntry(gECALErrors, "ECal Outliers", "p");
    legend1->Draw();

    c->Write();
    myfile->Write();
    myfile->cd("..");
    myfile->cd("quality_assurance");

    auto c1 = new TCanvas((channel_name + "_" + measurement_type + "assurance").c_str(),
                          (channel_name + "_" + measurement_type).c_str(),
                          10, 10, 1600, 900);
    c1->Divide(2, 1);

    c1->cd(1);
    auto fDiag = new TF1("fDiag", "x", 0, maxLumi);
    g6->GetXaxis()->SetTitle("T0 Luminosity (Hz/#mub)");
    g6->GetYaxis()->SetTitle("ECal Luminosity (Hz/#mub)");
    g6->SetTitle("Luminosities Correlation");
    g6->SetMarkerStyle(28);
    g6->SetMarkerColor(8);
    g6->SetMarkerSize(0.6);
    g6->Fit("pol1", "Q");
    g6->Draw("AP");
    fDiag->SetLineColor(kMagenta + 1);
    fDiag->Draw("SAME");

    auto legend2 = new TLegend(0.55, 0.3, 0.8, 0.45);
    //legend->SetHeader("small offset in times","C"); // option "C" allows to center the header
    legend2->AddEntry(fDiag, "Perfect Correlation", "l");
    legend2->AddEntry(g6, "Correlation", "lp");
    legend2->Draw();

    c1->cd(2);
    current_tof_cur->GetXaxis()->SetTitle("Anode Current (A)");
    current_tof_cur->GetYaxis()->SetTitle("TOF Current (A)");
    current_tof_cur->SetTitle("Correlation TOF-TRD Current");
    current_tof_cur->SetMarkerStyle(28);
    current_tof_cur->SetMarkerColor(8);
    current_tof_cur->SetMarkerSize(0.6);
    current_tof_cur->Draw("AP");
    c1->Write();
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
    if (g6)
        delete g6;
    if (gT0Errors)
        delete gT0Errors;
    if (gECALErrors)
        delete gECALErrors;
    if (dummyhist)
        delete dummyhist;
    if (dummygraphT0)
        delete dummygraphT0;
    if (dummygraphECAL)
        delete dummygraphECAL;
    if (c)
        delete c;
    if (c1)
        delete c1;
    if (legend)
        delete legend;
    if (legend1)
        delete legend1;
    if (legend2)
        delete legend2;
    if (fDiag)
        delete fDiag;
    if (current_tof_cur)
    {
        delete current_tof_cur;
    }
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
    auto c = new TCanvas(channel_name.c_str(), ("Offset @ " + channel_name).c_str(), 1600, 900);
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
    g0->GetXaxis()->SetTimeFormat("#splitline{%H}{%M}");
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