#ifndef ANALYSIS_DATES_H
#define ANALYSIS_DATES_H

#include <iostream>
#include <vector>
#include <TROOT.h>

class dates
{
public:
    std::vector<std::tuple<Int_t, Int_t, Int_t>> timestamps;
    std::vector<std::string> luminosity_labels;
    std::vector<Double_t> luminosity_points;

    dates(std::string date);
};

#endif