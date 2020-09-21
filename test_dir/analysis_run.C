#define ANALYSIS_CXX
#include "analysis_run.h"
#include "analysis_hv.h"
#include "analysis_hist.h"
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
    Double_t start_time = 0, end_time = 0;
    Int_t mean_n = 0;
    Int_t sector_n = 6;
    Int_t luminosity_index = 0;

    // Map for Current per Chamber <Stack, <Layer,  Mean Current>>
    std::map<Int_t, std::map<Int_t, Double_t>> mean_current_map;
    // Map for offset per Chamber <Stack, <Layer,  Mean Offset>>
    std::map<Int_t, std::map<Int_t, Double_t>> mean_offset_map;
    // Map for if Current per Stack should be included in further Calculations <Stack, <Layer,  True/False>>
    std::map<Int_t, std::map<Int_t, Bool_t>> mean_hv_map;
    // Overall Mean Current in Sector;
    Double_t overall_mean_current = 0;

    // Map for offset per Chamber <Stack, <Layer,  Mean Offset>>
    std::map<Int_t, std::map<Int_t, Double_t>> mean_luminosity_current_map;
    std::vector<std::string> luminosity_labels = {"2.6 Hz/#mub", "9 Hz/#mub", "15 Hz/#mub", "30 Hz/#mub", "40 Hz/#mub", "52 Hz/#mub", "64 Hz/#mub", "70 Hz/#mub"};
    histogramms hists(6, luminosity_labels);

    // individual set timestamps
    std::vector<std::tuple<Int_t, Int_t, Int_t>> timestamps = {
        {1, 1504576500, 1504579200},
        {2, 1504579800, 1504583700},
        //{3, 1504584000, 1504587900},
        //{4, 1504589100, 1504592400},
        //{5, 1504605300, 1504607400},
        //{6, 1504608000, 1504610700},
        //{7, 1504611900, 1504615800},
        //{8, 1504617600, 1504626600}};
    };

    while (file >> crate_name >> file_index >> install_year >> measurement_type >> channel_name)
    {
        if (measurement_type[0] == 'I')
        {
            Int_t sector = std::stoi(channel_name.substr(0, 2));
            Int_t stack = std::stoi(channel_name.substr(3, 1));
            Int_t layer = std::stoi(channel_name.substr(5, 1));
            char plate = channel_name[6];
            if (sector == sector_n)
            {
                if (plate == 'A')
                {
                    analysis offset(Form("sorted_%d.csv.root", file_index), 1504399786, 1504401590);
                    offset.Offset();
                    mean_offset_map[stack][layer] = offset.offset;
                    for (const auto &timestamps_element : timestamps)
                    {
                        luminosity_index = std::get<0>(timestamps_element);
                        start_time = std::get<1>(timestamps_element);
                        end_time = std::get<2>(timestamps_element);

                        // Analysis for hv
                        analysis_hv hv(Form("sorted_%d.csv.root", file_index - 1),
                                       Form("sm_%d.root", sector),
                                       channel_name, luminosity_index,
                                       'V',
                                       start_time, end_time);
                        mean_hv = hv.Loop();
                        mean_hv_map[stack][layer] = (mean_hv > 1450) ? kTRUE : kFALSE;

                        // Anlysis for current
                        analysis current(Form("sorted_%d.csv.root", file_index),
                                         Form("sm_%d.root", sector),
                                         channel_name, luminosity_index,
                                         'I',
                                         start_time, end_time);
                        mean_current = current.Loop();
                        mean_current_map[stack][layer] = ((mean_current - offset.offset) < 0) ? 0 : mean_current - offset.offset;

                        // Calculate mean
                        if (mean_hv_map[stack][layer])
                        {
                            overall_mean_current += mean_current;
                            mean_n++;
                        }
                        // Print Information
                        std::cout << file_index - 1 << "\t\t" << mean_hv_map[stack][layer] << "\t\t"
                                  << stack << "\t\t" << layer << std::endl;
                        std::cout << file_index << "\t\t" << mean_current_map[stack][layer] << "\t\t"
                                  << mean_offset_map[stack][layer] << std::endl;
                        std::cout << "Luminosity index: " << luminosity_index << std::endl;
                        // TODO content
                        hists.hist_lumi->SetBinContent(luminosity_index, stack * 6 + layer + 1,
                                                       (!mean_hv_map[stack][layer]) ? 0 : mean_current_map[stack][layer]);
                        // deletes
                        //delete current;
                        //delete hv;
                    }
                }
            }
        }
    }
    // TODO move to correct place
    //overall_mean_current /= (mean_n != 0) ? mean_n : 30;
    //hists->Draw(overall_mean_current, mean_current_map, mean_hv_map, mean_offset_map);
    //hists->Write();
    hists.WriteLumi();
}

int main(int argc, char const *argv[])
{
    analysis_run();
    return 0;
}
