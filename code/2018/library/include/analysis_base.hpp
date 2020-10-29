#ifndef ANALYSIS_H
#define ANALYSIS_H

#include <TROOT.h>
#include <TMath.h>
#include <TChain.h>
#include <TMultiGraph.h>
#include <TGaxis.h>
#include <TLegend.h>
#include <TCanvas.h>
#include <TGraph.h>
#include <TFile.h>
#include <TF1.h>
#include <TH1F.h>
#include <TAxis.h>
#include <iostream>
#include <vector>
#include <TColor.h>

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
    Double_t TOF_average_current;
    Double_t TOF_rms_current;
    Double_t Luminosity;
    Double_t T0_Luminosity;
    Double_t current;

    //TTimeStamp      *time;
    Int_t fSec;
    Int_t fNanoSec;

    // List of branches
    TBranch *b_HV;                  //!
    TBranch *b_time_fSec;           //!
    TBranch *b_time_fNanoSec;       //!
    TBranch *b_TOF_average_current; //!
    TBranch *b_TOF_rms_current;     //!
    TBranch *b_Luminosity;          //!
    TBranch *b_T0_Luminosity;       //!
    TBranch *b_current;             //!

    // Names and File-info
    std::string outfile_name;
    std::string channel_name;
    char measurement_type;
    Int_t offset_start_time = 0;
    Int_t offset_end_time = 0;
    Double_t offset = 0;
    Double_t mean_hv = 0;

    TGraph *t0_lumi_gr;
    TGraph *tof_lumi_gr;

    analysis(const std::string filename, const std::string outfile, const std::string channel, const Int_t offset_start, const Int_t offset_end);
    analysis(const std::string filename, const std::string outfile, const std::string channel, const char measurement, const Double_t _offset);
    virtual ~analysis();
    virtual Int_t Cut(const Long64_t entry);
    virtual void Offset();
    virtual Int_t GetEntry(const Long64_t entry);
    virtual Long64_t LoadTree(const Long64_t entry);
    virtual void Init(TTree *tree);
    virtual void InitOffset(TTree *tree);
    virtual void Loop(Double_t weight_channel);
    virtual Bool_t Notify();
    virtual void Show(const Long64_t entry = -1);
    Double_t mean(const std::vector<Double_t> *v);
};

#endif