#ifndef ANALYSIS_H
#define ANALYSIS_H

#include <TROOT.h>
#include <TMath.h>
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

    analysis(const std::string filename, const Int_t offset_start, const Int_t offset_end);
    analysis(const std::string filename, const std::string outfile, const std::string channel_name,
             const Int_t lumi_index, const char measurement, const Int_t start, const Int_t end);
    virtual ~analysis();
    virtual Int_t Cut(const Long64_t entry);
    virtual void Offset();
    virtual Int_t GetEntry(const Long64_t entry);
    virtual Long64_t LoadTree(const Long64_t entry);
    virtual void Init(TTree *tree);
    virtual std::pair<Double_t, Double_t> Loop();
    virtual Bool_t Notify();
    virtual Double_t mean(const std::vector<Double_t> *v);
    virtual Double_t mean_std(const std::vector<Double_t> *v, const Double_t mean);
    virtual void Show(const Long64_t entry = -1);
};

#endif