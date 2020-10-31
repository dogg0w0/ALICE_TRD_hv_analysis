#include <iostream>
#include <fstream>
#include "TTree.h"
#include "TFile.h"

void convertCSV(std::string inputfile, std::string outputfile)
{
    std::ifstream in;
    in.open(inputfile);
    Int_t fSec, fNanoSec;
    Double_t TOF_average_current, TOF_rms_current, luminosity, T0_luminosity, current, HV, mean_current, std_current;

    auto f = new TFile(outputfile.c_str(), "RECREATE");
    TTree *tree = new TTree("TOF_T0_Tree", "data from matched csv");

    tree->Branch("fSec", &fSec, "fSec/I");
    tree->Branch("fNanoSec", &fNanoSec, "fNanoSec/I");
    tree->Branch("TOF_average_current", &TOF_average_current, "TOF_average_current/D");
    tree->Branch("TOF_rms_current", &TOF_rms_current, "TOF_rms_current/D");
    tree->Branch("Luminosity", &luminosity, "Luminosity/D");
    tree->Branch("T0_Luminosity", &T0_luminosity, "T0_Luminosity/D");
    tree->Branch("HV", &HV, "HV/D");
    tree->Branch("current", &current, "current/D");
    tree->Branch("mean_current", &mean_current, "mean_current/D");
    tree->Branch("std_current", &std_current, "std_current/D");

    while (in >> fSec >> fNanoSec >> TOF_average_current >> TOF_rms_current >> luminosity >> T0_luminosity >> current >> HV >> mean_current >> std_current)
    {
        tree->Fill();
    }
    in.close();
    f->Write();
}

int main(int argc, char const *argv[])
{
    convertCSV((std::string)argv[1], (std::string)argv[2]);
    return 0;
}
