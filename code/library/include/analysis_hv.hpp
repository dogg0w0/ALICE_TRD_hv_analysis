#ifndef ANALYSIS_HV_H
#define ANALYSIS_HV_H

#include "analysis_base.hpp"
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

#endif