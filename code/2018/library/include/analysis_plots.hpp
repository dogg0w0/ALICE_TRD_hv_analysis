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
    TH2D *hist_offset;
    std::vector<TGraph *> gr_lumi_fit_single_t0;
    std::vector<TGraph *> gr_lumi_fit_single_tof;
    Int_t sector;
    Int_t n_not_working_chambers = 0;
    std::vector<std::string> channel_labels;
    std::vector<Double_t> chambers_weights = std::vector<Double_t>(30, 1.0);
    std::vector<Double_t> angle_weights = std::vector<Double_t>(30, 1.0);
    std::vector<Double_t> gain_weights = std::vector<Double_t>(540, 1.0);
    std::vector<Double_t> radial_weights = std::vector<Double_t>(6, 1.0);
    std::vector<Double_t> weights;

    plots(const Int_t sector_n);
    plots(const Int_t sector_n, const std::string gain_map, const Int_t gain_index);
    ~plots();
    void Draw(std::map<Int_t, std::map<Int_t, Bool_t>> &mean_hv_map, std::map<Int_t, std::map<Int_t, Double_t>> &mean_offset_map);
    void Canvas();
    void HistOffset();
    void Write();
    void ChannelNames();
    void ChamberWeightsInit();
    void GainWeightsInit(const std::string gain_map, const Int_t gain_index);
    void RadialWeightsInit();
    void SolidAngleWeight();
    void WeightsInit();
    void FitInit();
    void FitDraw();
    void FitSlopeOffset(const std::vector<Double_t> &fit_a_v, const std::vector<Double_t> &fit_b_v);
};

#endif