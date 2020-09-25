#include <iostream>
#include <fstream>
#include "analysis_base.hpp"
#include "analysis_hist.hpp"
#include "analysis_hv.hpp"

void analysis_run(Int_t sector_number, std::string input_data_dir, std::string input_outfile)
{
    std::string file_dir = input_data_dir + "sorted_%d.csv.root";
    std::string line, channel_name, crate_name, measurement_type;
    Int_t file_index, install_year;
    Double_t mean_current, mean_std_current, mean_hv, mean_std_hv;
    Double_t start_time = 0, end_time = 0;
    Int_t mean_n = 0;
    Int_t sector_n = sector_number;
    Int_t luminosity_index = 0;

    // Map for Current per Chamber <Stack, <Layer,  Mean Current>>
    std::map<Int_t, std::map<Int_t, Double_t>> mean_current_map;
    // Map for offset per Chamber <Stack, <Layer,  Mean Offset>>
    std::map<Int_t, std::map<Int_t, Double_t>> mean_offset_map;
    // Map for if Current per Stack should be included in further Calculations <Stack, <Layer,  True/False>>
    std::map<Int_t, std::map<Int_t, Bool_t>> mean_hv_map;
    // Overall Mean Current in Sector;
    Double_t overall_mean_current = 0;
    // std::pair<Double_t, Double_t> for analysis::Loop
    std::pair<Double_t, Double_t> mean_and_std_pair;

    // Map for offset per Chamber <Stack, <Layer,  Mean Offset>>
    std::map<Int_t, std::map<Int_t, Double_t>> mean_luminosity_current_map;
    std::vector<std::string> luminosity_labels = {"2.6 Hz/#mub", "9 Hz/#mub", "15 Hz/#mub", "30 Hz/#mub", "40 Hz/#mub", "52 Hz/#mub", "64 Hz/#mub", "70 Hz/#mub"};
    std::vector<Double_t> luminosity_points = {2.6, 9.0, 15.0, 30.0, 40.0, 52.0, 64.0, 70.0};
    histogramms hists(sector_n, luminosity_labels, luminosity_points);

    // individual set timestamps
    std::vector<std::tuple<Int_t, Int_t, Int_t>> timestamps = {
        {1, 1504576500, 1504579200},
        {2, 1504579800, 1504583700},
        {3, 1504584000, 1504587900},
        {4, 1504589100, 1504592400},
        {5, 1504605300, 1504607400},
        {6, 1504608000, 1504610700},
        {7, 1504611900, 1504615800},
        {8, 1504617600, 1504626600}};

    Int_t date_time = std::get<1>(timestamps[0]);
    TTimeStamp time_stamp;
    time_stamp.SetSec(date_time);

    for (const auto &timestamps_element : timestamps)
    {
        Int_t counter = 0;
        luminosity_index = std::get<0>(timestamps_element);
        start_time = std::get<1>(timestamps_element);
        end_time = std::get<2>(timestamps_element);

        std::ifstream file(input_outfile);
        if (!file.is_open())
        {
            std::cout << "outfile not open" << std::endl;
            return;
        }

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
                        // Calculate Offset
                        analysis offset(Form(file_dir.c_str(), file_index), 1504399786, 1504401590);
                        offset.Offset();
                        mean_offset_map[stack][layer] = offset.offset;

                        // Analysis for hv
                        analysis_hv hv(Form(file_dir.c_str(), file_index - 1),
                                       Form("sm_%d.root", sector),
                                       channel_name, luminosity_index,
                                       'V',
                                       start_time, end_time);
                        mean_and_std_pair = hv.Loop();
                        mean_hv = mean_and_std_pair.first;
                        mean_std_hv = mean_and_std_pair.second;
                        mean_hv_map[stack][layer] = (mean_hv > 1450) ? kTRUE : kFALSE;

                        // Anlysis for current
                        analysis current(Form(file_dir.c_str(), file_index),
                                         Form("sm_%d.root", sector),
                                         channel_name, luminosity_index,
                                         'I',
                                         start_time, end_time);
                        mean_and_std_pair = current.Loop();
                        mean_current = mean_and_std_pair.first;
                        mean_std_current = mean_and_std_pair.second;
                        mean_current_map[stack][layer] = ((mean_current - offset.offset) < 0) ? 0 : mean_current - offset.offset;

                        // Calculate mean
                        if (mean_hv_map[stack][layer])
                        {
                            overall_mean_current += mean_current;
                            mean_n++;
                        }
                        // Print Information
                        std::cout << file_index - 1 << std::setw(12) << mean_hv_map[stack][layer] << std::setw(12)
                                  << stack << std::setw(12) << layer << std::endl;
                        std::cout << file_index << std::setw(12) << mean_current_map[stack][layer] << std::setw(12)
                                  << mean_offset_map[stack][layer] << std::endl;
                        std::cout << "Luminosity index: " << luminosity_index << std::endl;

                        hists.hist_lumi->SetBinContent(luminosity_index, stack * 6 + layer + 1,
                                                       (!mean_hv_map[stack][layer]) ? 0 : mean_current_map[stack][layer]);
                        counter++;
                    }
                }
            }
            if (counter == 60)
            {
                break;
            }
        }
        overall_mean_current /= (mean_n != 0) ? mean_n : 30;
        hists.Draw(overall_mean_current, luminosity_index, mean_current_map, mean_hv_map, mean_offset_map);
        hists.Write();
        file.close();
    }
    hists.WriteLumi(((std::string)time_stamp.AsString()).substr(5, 11));
}

int main(int argc, char const *argv[])
{
    if (argc == 4)
    {
        analysis_run((std::stoi(argv[1])), (std::string)argv[2], (std::string)argv[3]);
    }
    else
    {
        std::cerr << "Usage:\t" << argv[0] << "\t sector number"
                  << "\t input data dir "
                  << "\t input_file.txt " << std::endl;
    }
    return 0;
}