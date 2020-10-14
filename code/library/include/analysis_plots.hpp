#ifndef ANALYSIS_PLOTS_H
#define ANALYSIS_PLOTS_H

#include <TROOT.h>
#include <TStyle.h>
#include <TLatex.h>
#include <TF1.h>
#include <TCanvas.h>
#include <TGraphErrors.h>
#include <TGraph.h>
#include <TH2D.h>
#include <TH1D.h>
#include <TFile.h>
#include <TAxis.h>
#include <TTree.h>
#include <iostream>
#include <vector>

class plots
{
public:
    TCanvas *canvas;
    TH2D *hist_sector;
    TH2D *hist_perc;
    TH2D *hist_lumi;
    TH2D *hist_offset;
    TGraphErrors *gr_lumi_fit;
    std::vector<TGraphErrors *> gr_lumi_fit_single;
    Int_t sector;
    Int_t n_not_working_chambers = 0;
    std::vector<std::string> channel_labels;
    std::vector<Double_t> luminosities = {0.0};
    std::vector<Double_t> mean_current_all_chambers = {0.0};
    std::vector<Double_t> mean_current_all_chambers_err = {0.0};
    std::vector<Double_t> chambers_weights = std::vector<Double_t>(30, 1.0);
    std::vector<Double_t> gain_weights = std::vector<Double_t>(540, 1.0);
    std::vector<Double_t> radial_weights = std::vector<Double_t>(6, 1.0);
    std::vector<Double_t> weights;
    //std::vector<std::string> luminosity_labels;

    plots(const Int_t sector_n);
    plots(const Int_t sector_n, const std::vector<std::string> &luminosity_labels, const std::vector<Double_t> &luminosity_points, std::string gain_map);
    ~plots();
    void Draw(const Double_t overall_mean_current, const Int_t luminosity_index, std::map<Int_t, std::map<Int_t, Double_t>> &mean_current_map,
              std::map<Int_t, std::map<Int_t, Bool_t>> &mean_hv_map, std::map<Int_t, std::map<Int_t, Double_t>> &mean_offset_map);
    void Canvas();
    void HistSector();
    void HistLumi(const std::vector<std::string> &luminosity_labels, const std::vector<Double_t> &luminosity_points);
    void HistPerc();
    void HistOffset();
    void Write();
    void WriteLumi(const std::string time_stamp);
    void ChannelNames();
    void ChamberWeightsInit();
    void GainWeightsInit(std::string gain_map);
    void RadialWeightsInit();
    void WeightsInit();
    void FitInit(const std::vector<std::string> &luminosity_labels);
    void FitUpdate(const Int_t luminosity_index, std::map<Int_t, std::map<Int_t, Double_t>> &mean_current_map, std::map<Int_t, std::map<Int_t, Double_t>> &mean_std_current_map,
                   std::map<Int_t, std::map<Int_t, Bool_t>> &mean_hv_map);
    void FitDraw();
    void FitSlopeOffset(const std::vector<Double_t> &fit_a_v, const std::vector<Double_t> &fit_b_v);
};

#endif