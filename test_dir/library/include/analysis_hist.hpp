#ifndef ANALYSIS_HIST_H
#define ANALYSIS_HIST_H

#include <TROOT.h>
#include <TCanvas.h>
#include <TGraph.h>
#include <TH2D.h>
#include <TH1D.h>
#include <TFile.h>
#include <TAxis.h>
#include <iostream>
#include <vector>

class histogramms
{
public:
    TCanvas *canvas;
    TH2D *hist_sector;
    TH2D *hist_perc;
    TH2D *hist_lumi;
    TH2D *hist_offset;
    Int_t sector;
    std::vector<std::string> channel_labels;
    //std::vector<std::string> luminosity_labels;

    histogramms(Int_t sector_n);
    histogramms(Int_t sector_n, std::vector<std::string> &luminosity_labels);
    ~histogramms();
    void Draw(Double_t overall_mean_current, std::map<Int_t, std::map<Int_t, Double_t>> &mean_current_map,
              std::map<Int_t, std::map<Int_t, Bool_t>> &mean_hv_map, std::map<Int_t, std::map<Int_t, Double_t>> &mean_offset_map);
    void Canvas();
    void HistSector();
    void HistLumi(std::vector<std::string> &luminosity_labels);
    void HistPerc();
    void HistOffset();
    void Write();
    void WriteLumi();
    void ChannelNames();
};

#endif