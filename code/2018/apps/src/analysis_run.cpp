#include <iostream>
#include <fstream>
#include "analysis_base.hpp"
#include "analysis_plots.hpp"
#include "analysis_working.hpp"

void analysis_run(Int_t sector_number, std::string input_data_dir, std::string input_outfile, std::string file_offset_dir, Int_t offset_start, Int_t offset_end, Int_t gain_index)
{
    std::string file_dir = input_data_dir + "sorted_%d.csv.root";
    std::string file_dir_offset = file_offset_dir + "sorted_%d.csv.root";
    std::string line, channel_name, crate_name, measurement_type;
    Int_t file_index, install_year;
    Double_t mean_current, mean_std_current, mean_hv, mean_std_hv;
    Double_t start_time = 0, end_time = 0;
    Int_t mean_n = 0;
    Int_t sector_n = sector_number;
    Int_t luminosity_index = 0;

    // Map for offset per Chamber <Stack, <Layer,  Mean Offset>>
    std::map<Int_t, std::map<Int_t, Double_t>> mean_offset_map;
    // Map for if Current per Stack should be included in further Calculations <Stack, <Layer,  True/False>>
    std::map<Int_t, std::map<Int_t, Bool_t>> mean_hv_map;

    plots plotter(sector_n, "/home/doggo/Downloads/ALICE_TRD/gain/trendingOCDB_2018.root", gain_index);
    analysis_working working;

    //Int_t date_time = std::get<1>(timestamps[0]);
    //TTimeStamp time_stamp;
    //time_stamp.SetSec(date_time);

    Int_t counter = 0;

    std::ifstream file(input_outfile);
    if (!file.is_open())
    {
        std::cout << "outfile not open" << std::endl;
        return;
    }

    while (file >> crate_name >> file_index >> install_year >> measurement_type >> channel_name)
    {
        if (file_index % 2 == 0)
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
                        analysis offset(Form(file_dir_offset.c_str(), file_index), Form("sm_%d.root", sector),
                                        channel_name, offset_start, offset_end);
                        offset.Offset();
                        mean_offset_map[stack][layer] = offset.offset;

                        // Anlysis Call
                        analysis a(Form(file_dir.c_str(), file_index),
                                   Form("sm_%d.root", sector),
                                   channel_name, 'I', offset.offset);
                        a.Loop(plotter.weights[30 * sector + 6 * stack + layer]);
                        mean_hv_map[stack][layer] = (a.mean_hv > 1450) ? kTRUE : kFALSE;
                        working.Update(stack, layer, mean_hv_map[stack][layer]);

                        // Print Information
                        std::cout << file_index << "\t\t\t" << mean_hv_map[stack][layer] << "\t\t\t"
                                  << stack << "\t\t\t" << layer << std::endl;

                        plotter.gr_lumi_fit_single_t0.push_back(a.t0_lumi_gr);
                        plotter.gr_lumi_fit_single_tof.push_back(a.tof_lumi_gr);
                        counter++;
                    }
                }
            }
        }
        if (counter == 30)
        {
            break;
        }
    }
    plotter.FitInit();
    plotter.Draw(mean_hv_map, mean_offset_map);
    plotter.Write();
    file.close();
    working.Write(sector_n);
    plotter.FitDraw();
}

void invokeStyle();

int main(int argc, char const *argv[])
{
    //invokeStyle();
    //auto style = gROOT->GetStyle("Pub");
    //style->SetOptTitle(1);
    gROOT->SetStyle("Pub");
    gROOT->ForceStyle();
    TH1::SetDefaultSumw2(kTRUE);
    TH2::SetDefaultSumw2(kTRUE);
    if (argc == 8)
    {
        analysis_run((std::stoi(argv[1])), (std::string)argv[2], (std::string)argv[3], (std::string)argv[4], (std::stoi(argv[5])), (std::stoi(argv[6])), std::stoi(argv[7]));
    }
    else
    {
        std::cerr << "Usage:\t" << argv[0] << "\t sector number"
                  << "\t input data dir "
                  << "\t input_file.txt "
                  << "\t input_offset_dir"
                  << "\t offset_start"
                  << "\t offset_end"
                  << "\t Gain Index negative if to be ignored" << std::endl;
    }
    return 0;
}

void invokeStyle()
{
    //..BABAR style from RooLogon.C in workdir
    TStyle *babarStyle = new TStyle("BABAR", "BaBar approved plots style");

    // use plain black on white colors
    babarStyle->SetFrameBorderMode(0);
    babarStyle->SetCanvasBorderMode(0);
    babarStyle->SetPadBorderMode(0);
    babarStyle->SetPadColor(0);
    babarStyle->SetCanvasColor(0);
    babarStyle->SetStatColor(0);

    // set the paper & margin sizes
    babarStyle->SetPaperSize(20, 26);
    babarStyle->SetPadTopMargin(0.1);
    babarStyle->SetPadRightMargin(0.14);
    babarStyle->SetPadBottomMargin(0.1);
    babarStyle->SetPadLeftMargin(0.14);

    // use large Times-Roman fonts
    babarStyle->SetTextFont(132);
    babarStyle->SetTextSize(0.08);
    babarStyle->SetLabelFont(132, "x");
    babarStyle->SetLabelFont(132, "y");
    babarStyle->SetLabelFont(132, "z");
    babarStyle->SetLabelSize(0.05, "x");
    babarStyle->SetTitleSize(0.06, "x");
    babarStyle->SetLabelSize(0.05, "y");
    babarStyle->SetTitleSize(0.06, "y");
    babarStyle->SetLabelSize(0.05, "z");
    babarStyle->SetTitleSize(0.06, "z");

    // use bold lines and markers
    babarStyle->SetMarkerStyle(20);
    babarStyle->SetHistLineWidth(1.85);
    babarStyle->SetLineStyleString(2, "[12 12]"); // postscript dashes

    // get rid of X error bars and y error bar caps
    babarStyle->SetErrorX(0.001);

    // do not display any of the standard histogram decorations
    //babarStyle->SetOptTitle(0);
    babarStyle->SetOptStat(0);
    babarStyle->SetOptFit(0);

    // put tick marks on top and RHS of plots
    babarStyle->SetPadTickX(1);
    babarStyle->SetPadTickY(1);

    gROOT->SetStyle("Pub");

    gROOT->ForceStyle();
    TH1::SetDefaultSumw2(kTRUE);
    TH2::SetDefaultSumw2(kTRUE);
}