#ifndef ANALYSIS_H
#define ANALYSIS_H

#include <TROOT.h>
#include <TChain.h>
#include <TCanvas.h>
#include <TGraph.h>
#include <TFile.h>
#include <TAxis.h>
#include <iostream>
#include <vector>

// Header file for the classes stored in the TTree if any.
#include "TTimeStamp.h"

class analysis
{
public:
    TTree *fChain;  //!pointer to the analyzed TTree or TChain
    Int_t fCurrent; //!current Tree number in a TChain

    // Fixed size dimensions of array or collections stored in the TTree if any.

    // Declaration of leaf types
    Double_t HV;
    //TTimeStamp      *time;
    Int_t fSec;
    Int_t fNanoSec;

    // List of branches
    TBranch *b_HV;            //!
    TBranch *b_time_fSec;     //!
    TBranch *b_time_fNanoSec; //!

    // Names and File-info
    std::string outfile_name;
    std::string channel_name;
    char measurement_type;
    Int_t start_time = 0; // start fSec since EPOCH
    Int_t end_time = 0;   // end fSec since EPOCH
    Int_t offset_start_time = 0;
    Int_t offset_end_time = 0;
    Int_t luminosity_index = 0;
    Double_t offset = 0;

    analysis(std::string filename, Int_t offset_start, Int_t offset_end);
    analysis(std::string filename, std::string outfile, std::string channel_name,
             Int_t lumi_index, char measurement, Int_t start, Int_t end);
    virtual ~analysis();
    virtual Int_t Cut(Long64_t entry);
    virtual void Offset();
    virtual Int_t GetEntry(Long64_t entry);
    virtual Long64_t LoadTree(Long64_t entry);
    virtual void Init(TTree *tree);
    virtual Double_t Loop();
    virtual Bool_t Notify();
    virtual Double_t mean(std::vector<Double_t> *v);
    virtual void Show(Long64_t entry = -1);
};

Double_t analysis::Loop()
{
    if (fChain == 0)
        return -1;

    Long64_t nentries = fChain->GetEntriesFast();
    TFile *myfile = new TFile(outfile_name.c_str(), "UPDATE");
    TTimeStamp ttime(0, 0);
    TCanvas *c0 = new TCanvas((channel_name + "_" + measurement_type + "_" + Form("%d", luminosity_index)).c_str(),
                              (channel_name + "_" + measurement_type + "_" + Form("%d", luminosity_index)).c_str(),
                              10, 10, 800, 600);
    c0->cd();
    TGraph *g = new TGraph();
    Long64_t gentry = 0;
    Long64_t nbytes = 0, nb = 0;
    std::vector<Double_t> hv_v = {};
    for (Long64_t jentry = 0; jentry < nentries; jentry++)
    {
        Long64_t ientry = LoadTree(jentry);
        if (ientry < 0)
            break;
        nb = fChain->GetEntry(jentry);
        nbytes += nb;
        // if (Cut(ientry) < 0) continue;
        if (HV < 0)
        {
            continue;
        }
        if (fSec < start_time)
        {
            continue;
        }
        if (fSec > end_time)
        {
            break;
        }

        ttime.SetSec(fSec);
        ttime.SetNanoSec(fNanoSec);
        hv_v.push_back(HV);
        g->SetPoint(gentry, ttime.AsDouble(), HV);
        gentry++;
    }

    g->SetTitle(channel_name.c_str());
    g->GetXaxis()->SetTimeOffset(0, "gmt");
    g->GetXaxis()->SetTimeDisplay(1);
    g->GetXaxis()->SetLabelOffset(0.02);
    g->GetXaxis()->SetTimeFormat("#splitline{%Y}{#splitline{%d\/%m}{%H\:%M\:%S}}");
    g->GetXaxis()->SetTitle("Time");
    g->GetYaxis()->SetTitle("Current [#muA]");
    g->SetMarkerStyle(8);
    g->SetMarkerSize(0.5);
    g->SetMarkerColorAlpha(kRed, 0.35);
    g->Draw("ALP");
    c0->Write();
    myfile->Close();
    if (g)
        g->Delete();
    if (c0)
        delete c0;
    return analysis::mean(&hv_v);
}

analysis::analysis(std::string filename, Int_t offset_start, Int_t offset_end)
{
    offset_start_time = offset_start;
    offset_end_time = offset_end;
    TTree *tree = 0;
    TFile *f = new TFile(filename.c_str(), "READ");
    if (!f || !f->IsOpen())
    {
        std::cerr << "cannot open file:\t" << filename << std::endl;
    }
    f->GetObject("Tree_TRD_HV", tree);

    Init(tree);
}

analysis::analysis(std::string filename, std::string outfile, std::string channel,
                   Int_t lumi_index, char measurement, Int_t start, Int_t end) : fChain(0)
{
    outfile_name = outfile;
    channel_name = channel;
    measurement_type = measurement;
    luminosity_index = lumi_index;
    start_time = start;
    end_time = end;
    TTree *tree = 0;
    TFile *f = new TFile(filename.c_str(), "READ");
    if (!f || !f->IsOpen())
    {
        std::cerr << "cannot open file:\t" << filename << std::endl;
    }
    f->GetObject("Tree_TRD_HV", tree);

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
    // Called inside Loop
    Long64_t nentries = fChain->GetEntriesFast();
    Long64_t nbytes = 0, nb = 0;
    std::vector<Double_t> offset_v = {};
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
        offset_v.push_back(HV);
    }
    offset = analysis::mean(&offset_v);
}

Int_t analysis::GetEntry(Long64_t entry)
{
    // Read contents of entry.
    if (!fChain)
        return 0;
    return fChain->GetEntry(entry);
}
Long64_t analysis::LoadTree(Long64_t entry)
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
Double_t analysis::mean(std::vector<Double_t> *v)
{
    Double_t sum = 0, n = 0;
    for (auto &&element : *v)
    {
        sum += element;
        n++;
    }
    return sum / n;
}
void analysis::Show(Long64_t entry)
{
    // Print contents of entry.
    // If entry is not specified, print current entry
    if (!fChain)
        return;
    fChain->Show(entry);
}
Int_t analysis::Cut(Long64_t entry)
{
    // This function may be called from Loop.
    // returns  1 if entry is accepted.
    // returns -1 otherwise.
    return 1;
}

#endif // ANALYSIS_H