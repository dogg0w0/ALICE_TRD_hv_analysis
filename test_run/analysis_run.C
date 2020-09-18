#define ANALYSIS_CXX
#include "analysis_run.h"
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
    Double_t mean_hv;

    // Map for Current per Stack <Stack, <Layer,  Mean Current>>
    std::map<Int_t, std::map<Int_t, Double_t>> mean_current_map;
    // Overall Mean Current in Sector;
    Double_t overall_mean_hv = 0;

    TCanvas *c0 = new TCanvas("sector_6", "Sector 6", 10, 10, 800, 800);
    c0->cd();
    c0->SetGrid();
    c0->SetLeftMargin(0.15);
    c0->SetBottomMargin(0.15);
    TH2F *hist = new TH2F("sector_6_hist", "Mean Anode Current in Sector 6", 5, 0, 5, 6, 0, 6);
    hist->GetXaxis()->SetTitle("Stack");
    hist->GetYaxis()->SetTitle("Layer");
    hist->GetZaxis()->SetTitle("Current [#muA]");
    hist->SetStats(0);

    TH2F *hist_perc = new TH2F("sector_6_hist_perc", "", 5, 0, 5, 6, 0, 6);
    hist_perc->GetXaxis()->SetTitle("Stack");
    hist_perc->GetYaxis()->SetTitle("Layer");
    hist_perc->GetZaxis()->SetTitle("Percentage");
    hist_perc->SetStats(0);

    TH1F *hist_stack_perc = new TH1F("sector_6_hist_stack_perc", "Anode Current in Sector 6 per Stack to Baseline", 5, 0, 5);
    hist_stack_perc->GetXaxis()->SetTitle("Stack");
    hist_stack_perc->GetYaxis()->SetTitle("Percentage");
    hist_stack_perc->SetStats(0);

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
                    char filename[30];
                    std::sprintf(filename, "sorted_%d.csv.root", file_index);
                    analysis *t = new analysis(filename, "outfile.root", channel_name, crate_name, 1536323400, 1536330600);
                    mean_hv = t->Loop();
                    mean_current_map[stack][layer] = mean_hv;
                    overall_mean_hv += mean_hv;
                    std::cout << filename << "\t" << mean_current_map[stack][layer] << "\t" << stack << "\t" << layer << std::endl;
                    hist->SetBinContent(stack + 1, layer + 1, mean_hv);
                }
            }
        }
    }
    overall_mean_hv /= 30;
    char hist_perc_title[70];
    std::sprintf(hist_perc_title, "Anode Current to Baseline %f [#muA] in Sector 6", overall_mean_hv);
    hist_perc->SetTitle(hist_perc_title);
    for (Int_t stack = 0; stack < 5; stack++)
    {
        Double_t temp_layer_current = 0;
        for (Int_t layer = 0; layer < 6; layer++)
        {
            hist_perc->SetBinContent(stack + 1, layer + 1, mean_current_map[stack][layer] / overall_mean_hv);
            temp_layer_current += mean_current_map[stack][layer] / overall_mean_hv;
        }
        // Fill Hist per Stack
        hist_stack_perc->SetBinContent(stack + 1, temp_layer_current / 5);
    }

    TFile *out = new TFile("outfile.root", "UPDATE");
    hist->Draw("colz");
    hist_perc->Draw("text");
    hist_stack_perc->Draw();
    hist_stack_perc->Write();
    hist_perc->Write();
    hist->Write();
    c0->Write();
    out->Close();
}

int main(int argc, char const *argv[])
{
    analysis_run();
    return 0;
}
