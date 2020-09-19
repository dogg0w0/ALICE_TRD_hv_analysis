#define ANALYSIS_CXX
#include "analysis_run.h"
#include "analysis_hv.h"
#include "analysis_hist.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TROOT.h>
#include <iostream>
#include <fstream>
#include <vector>

void analysis_run()
{
    std::ifstream file("../outfile.txt");
    if (!file.is_open())
    {
        std::cout << "outfile not open" << std::endl;
        return;
    }
    std::string line, channel_name, crate_name, measurement_type;
    Int_t file_index, install_year;
    Double_t mean_current, mean_hv;
    Int_t mean_n = 0;

    // Map for Current per Stack <Stack, <Layer,  Mean Current>>
    std::map<Int_t, std::map<Int_t, Double_t>> mean_current_map;
    // Map for if Current per Stack should be included in mean <Stack, <Layer,  True/False>>
    std::map<Int_t, std::map<Int_t, Bool_t>> mean_hv_map;
    // Overall Mean Current in Sector;
    Double_t overall_mean_current = 0;

    histogramms *hists = new histogramms(6);

    while (file >> crate_name >> file_index >> install_year >> measurement_type >> channel_name)
    {
        if (measurement_type[0] == 'I')
        {
            Int_t sector = std::stoi(channel_name.substr(0, 2));
            Int_t stack = std::stoi(channel_name.substr(3, 1));
            Int_t layer = std::stoi(channel_name.substr(5, 1));
            char plate = channel_name[6];
            if (sector == 6)
            {
                if (plate == 'A')
                {
                    // Analysis for hv
                    analysis_hv *hv = new analysis_hv(Form("sorted_%d.csv.root", file_index - 1),
                                                      Form("sm_%d.root", sector),
                                                      channel_name,
                                                      'V',
                                                      1536323400, 1536330600);
                    mean_hv = hv->Loop();
                    mean_hv_map[stack][layer] = (mean_hv > 1450) ? kTRUE : kFALSE;

                    // Anlysis for current
                    analysis *current = new analysis(Form("sorted_%d.csv.root", file_index),
                                                     Form("sm_%d.root", sector),
                                                     channel_name,
                                                     'I',
                                                     1536323400, 1536330600,
                                                     1504399786, 1504401590);
                    mean_current = current->Loop();
                    mean_current_map[stack][layer] = mean_current;

                    // Calculate mean
                    if (mean_hv_map[stack][layer])
                    {
                        overall_mean_current += mean_current;
                        mean_n++;
                    }
                    // Print Information
                    std::cout << file_index - 1 << "\t\t" << mean_hv_map[stack][layer] << "\t\t" << stack << "\t\t" << layer << std::endl;
                    std::cout << file_index << "\t\t" << mean_current_map[stack][layer] << "\t\t" << stack << "\t\t" << layer <<  std::endl;
                    std::cout << ""<< std::endl;
                }
            }
        }
    }
    overall_mean_current /= (mean_n != 0) ? mean_n : 30;
    hists->Draw(overall_mean_current, mean_current_map, mean_hv_map);
    hists->Write();
}

int main(int argc, char const *argv[])
{
    analysis_run();
    return 0;
}
