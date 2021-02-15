#include <iostream>
#include <fstream>
#include "analysis_base.hpp"
#include "analysis_plots.hpp"
#include "analysis_hv.hpp"
#include "analysis_dates.hpp"
#include "analysis_working.hpp"

void analysis_run(Int_t sector_number, std::string input_data_dir, std::string input_outfile, std::string date, std::string gain_map)
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
    std::map<Int_t, std::map<Int_t, Double_t>> mean_current_map, mean_std_current_map;
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

    dates dates_temp(date);
    std::vector<std::tuple<Int_t, Int_t, Int_t>> timestamps = dates_temp.timestamps;
    std::vector<std::string> luminosity_labels = dates_temp.luminosity_labels;
    std::vector<Double_t> luminosity_points = dates_temp.luminosity_points;
    plots plotter(sector_n, luminosity_labels, luminosity_points, gain_map);
    analysis_working working;

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
                        mean_hv_map[stack][layer] = (mean_hv > 1000) ? kTRUE : kFALSE;
                        working.Update(stack, layer, mean_hv_map[stack][layer]);

                        // Anlysis for current
                        analysis current(Form(file_dir.c_str(), file_index),
                                         Form("sm_%d.root", sector),
                                         channel_name, luminosity_index,
                                         'I',
                                         start_time, end_time);
                        mean_and_std_pair = current.Loop();
                        mean_current = mean_and_std_pair.first;
                        mean_std_current = mean_and_std_pair.second;

                        mean_current_map[stack][layer] = ((mean_current - offset.offset) < 0) ? 0 : (mean_current - offset.offset) * plotter.weights[layer + stack * 6 + sector * 30];

                        mean_std_current_map[stack][layer] = mean_std_current * plotter.weights[layer + stack * 6 + sector * 30];

                        // Calculate mean
                        if (mean_hv_map[stack][layer])
                        {
                            overall_mean_current += mean_current_map[stack][layer];
                            mean_n++;
                        }
                        // Print Information
                        std::cout << file_index - 1 << "\t\t\t" << mean_hv_map[stack][layer] << "\t\t\t"
                                  << stack << "\t\t\t" << layer << std::endl;
                        std::cout << file_index << "\t\t\t" << mean_current_map[stack][layer] << "\t\t\t"
                                  << mean_offset_map[stack][layer] << std::endl;
                        std::cout << "Luminosity index: " << luminosity_index << std::endl;

                        plotter.hist_lumi->SetBinContent(luminosity_index, stack * 6 + layer + 1,
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
        plotter.Draw(overall_mean_current, luminosity_index, mean_current_map, mean_hv_map, mean_offset_map);
        plotter.FitUpdate(luminosity_index, mean_current_map, mean_std_current_map, mean_hv_map);
        plotter.Write();
        file.close();
    }
    working.Write(sector_n);
    plotter.FitDraw();
    plotter.WriteLumi(((std::string)time_stamp.AsString()).substr(5, 11));
}

void invokeStyle();

int main(int argc, char const *argv[])
{
    //invokeStyle();
    gROOT->SetStyle("Pub");
    gROOT->ForceStyle();
    TH1::SetDefaultSumw2(kTRUE);
    TH2::SetDefaultSumw2(kTRUE);
    if (argc == 6)
    {
        analysis_run((std::stoi(argv[1])), (std::string)argv[2], (std::string)argv[3], (std::string)argv[4], (std::string)argv[5]);
    }
    else
    {
        std::cerr << "Usage:\t" << argv[0] << "\t sector number"
                  << "\t input data dir "
                  << "\t input_file.txt "
                  << "\t Date "
                  << "\t gain_map.root" << std::endl;
    }
    return 0;
}