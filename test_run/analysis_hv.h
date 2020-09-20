#ifndef ANALYSIS_HV_H
#define ANALYSIS_HV_H

#include "analysis_run.h"
#include <TROOT.h>
#include <TChain.h>
#include <TCanvas.h>
#include <TGraph.h>
#include <TFile.h>
#include <TAxis.h>
#include <iostream>
#include <vector>

class analysis_hv : public analysis
{
public:
    using analysis::analysis;
    virtual Double_t Loop();
};

Double_t analysis_hv::Loop()
{
    if (fChain == 0)
        return -1;

    Long64_t nentries = fChain->GetEntriesFast();
    TFile *myfile = new TFile(outfile_name.c_str(), "UPDATE");
    TTimeStamp *ttime = new TTimeStamp();
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
        //if (fSec < start_time || fSec > end_time)
        if (fSec < start_time)
        {
            continue;
        }
        if (fSec > end_time)
        {
            break;
        }

        ttime->SetSec(fSec);
        ttime->SetNanoSec(fNanoSec);
        hv_v.push_back(HV);
        g->SetPoint(gentry, ttime->AsDouble(), HV);
        gentry++;
    }

    g->SetTitle(channel_name.c_str());
    g->GetXaxis()->SetTimeOffset(0, "gmt");
    g->GetXaxis()->SetTimeDisplay(1);
    g->GetXaxis()->SetLabelOffset(0.02);
    g->GetXaxis()->SetTimeFormat("#splitline{%Y}{#splitline{%d\/%m}{%H\:%M\:%S}}");
    g->GetXaxis()->SetTitle("Time");
    g->GetYaxis()->SetTitle("HV [V]");
    g->SetMarkerStyle(8);
    g->SetMarkerSize(0.5);
    g->SetMarkerColorAlpha(kRed, 0.35);
    g->Draw("ALP");
    c0->Write();
    myfile->Close();
    return analysis_hv::mean(&hv_v);
}

#endif