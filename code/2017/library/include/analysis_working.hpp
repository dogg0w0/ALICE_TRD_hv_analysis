#ifndef ANALYSIS_WORKING
#define ANALYSIS_WORKING

#include <TROOT.h>
#include <TFile.h>
#include <TTree.h>
#include <iostream>
#include <vector>

class analysis_working
{
public:
    std::map<Int_t, std::map<Int_t, Bool_t>> working_map;

    analysis_working();
    ~analysis_working();
    void Update(Int_t stack, Int_t layer, Bool_t b);
    void Write(Int_t sector);
};

#endif