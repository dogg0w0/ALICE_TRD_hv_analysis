#ifndef ANALYSIS_HIST_H
#define ANALYSIS_HIST_H

#include <TROOT.h>
#include <TCanvas.h>
#include <TGraph.h>
#include <TH2F.h>
#include <TH1F.h>
#include <TFile.h>
#include <TAxis.h>
#include <iostream>

class histogramms
{
public:
    TCanvas *canvas;
    TH2F *hist_sector;
    TH2F *hist_perc;
    TH1F *hist_stack_perc;
    TH2F *hist_exclude;
    Int_t sector;

    histogramms(Int_t sector_n);
    ~histogramms();
    void Draw(Double_t overall_mean_current, std::map<Int_t, std::map<Int_t, Double_t>> &mean_current_map, std::map<Int_t, std::map<Int_t, Bool_t>> &mean_hv_map);
    void Write();
};

histogramms::histogramms(Int_t sector_n)
{
    sector = sector_n;
    canvas = new TCanvas(Form("sector_%d", sector), Form("Sector %d", sector), 10, 10, 800, 800);
    canvas->Divide(2, 2);
    canvas->SetGrid();
    canvas->SetLeftMargin(0.15);
    canvas->SetBottomMargin(0.15);
    canvas->GetPad(1)->SetGrid();
    canvas->GetPad(2)->SetGrid();
    canvas->GetPad(4)->SetGrid();

    hist_sector = new TH2F(Form("sector_%d_hist", sector), Form("Mean Anode Current in Sector %d", sector), 5, 0, 5, 6, 0, 6);
    hist_sector->GetXaxis()->SetTitle("Stack");
    hist_sector->GetYaxis()->SetTitle("Layer");
    hist_sector->GetZaxis()->SetTitle("Current [#muA]");
    hist_sector->GetXaxis()->SetNdivisions(8, 8, 0, kTRUE);
    for (Int_t stack = 1; stack < 6; stack++)
    {
        hist_sector->GetXaxis()->SetBinLabel(stack, Form("%d", stack - 1));
    }
    for (Int_t layer = 1; layer < 7; layer++)
    {
        hist_sector->GetYaxis()->SetBinLabel(layer, Form("%d", layer - 1));
    }
    hist_sector->SetStats(0);

    hist_perc = new TH2F(Form("sector_%d_hist_perc", sector), "", 5, 0, 5, 6, 0, 6);
    hist_perc->GetXaxis()->SetTitle("Stack");
    hist_perc->GetYaxis()->SetTitle("Layer");
    hist_perc->GetZaxis()->SetTitle("Percentage");
    hist_perc->GetXaxis()->SetNdivisions(8, 8, 0, kTRUE);
    for (Int_t stack = 1; stack < 6; stack++)
    {
        hist_perc->GetXaxis()->SetBinLabel(stack, Form("%d", stack - 1));
    }
    for (Int_t layer = 1; layer < 7; layer++)
    {
        hist_perc->GetYaxis()->SetBinLabel(layer, Form("%d", layer - 1));
    }
    hist_perc->SetStats(0);

    hist_stack_perc = new TH1F(Form("sector_%d_hist_stack_perc", sector), Form("Anode Current in Sector %d per Stack to Baseline", sector), 5, 0, 5);
    hist_stack_perc->GetXaxis()->SetTitle("Stack");
    hist_stack_perc->GetYaxis()->SetTitle("Percentage");
    hist_stack_perc->GetXaxis()->SetNdivisions(8, 8, 0, kTRUE);
    for (Int_t stack = 1; stack < 6; stack++)
    {
        hist_stack_perc->GetXaxis()->SetBinLabel(stack, Form("%d", stack - 1));
    }
    hist_stack_perc->SetStats(0);

    hist_exclude = new TH2F(Form("sector_%d_hist_exclude", sector), "Excluded Chambers", 5, 0, 5, 6, 0, 6);
    hist_exclude->GetXaxis()->SetTitle("Stack");
    hist_exclude->GetYaxis()->SetTitle("Layer");
    hist_exclude->GetXaxis()->SetNdivisions(8, 8, 0, kTRUE);
    for (Int_t stack = 1; stack < 6; stack++)
    {
        hist_exclude->GetXaxis()->SetBinLabel(stack, Form("%d", stack - 1));
    }
    for (Int_t layer = 1; layer < 7; layer++)
    {
        hist_exclude->GetYaxis()->SetBinLabel(layer, Form("%d", layer - 1));
    }
    hist_exclude->SetStats(0);
}

histogramms::~histogramms()
{
    delete canvas;
    delete hist_sector;
    delete hist_perc;
    delete hist_stack_perc;
    delete hist_exclude;
}

void histogramms::Draw(Double_t overall_mean_current, std::map<Int_t, std::map<Int_t, Double_t>> &mean_current_map, std::map<Int_t, std::map<Int_t, Bool_t>> &mean_hv_map)
{
    hist_perc->SetTitle(Form("Anode Current to Baseline %f [#muA] in Sector 6", overall_mean_current));
    for (Int_t stack = 0; stack < 5; stack++)
    {
        Double_t temp_layer_current = 0;
        for (Int_t layer = 0; layer < 6; layer++)
        {
            // Hist
            hist_sector->SetBinContent(stack + 1, layer + 1, (!mean_hv_map[stack][layer]) ? 0 : mean_current_map[stack][layer]);
            hist_exclude->SetBinContent(stack + 1, layer + 1, (mean_hv_map[stack][layer]) ? 0 : 1);
            hist_perc->SetBinContent(stack + 1, layer + 1, (!mean_hv_map[stack][layer]) ? 0 : mean_current_map[stack][layer] / overall_mean_current);
            temp_layer_current += mean_current_map[stack][layer] / overall_mean_current;
        }
        // Fill Hist per Stack
        hist_stack_perc->SetBinContent(stack + 1, temp_layer_current / 5);
    }
}

void histogramms::Write()
{
    std::cout << "Writing results into file:\t" << Form("sm_%d.root", sector) << std::endl;
    TFile *out = new TFile(Form("sm_%d.root", sector), "UPDATE");
    canvas->cd(1);
    hist_sector->Draw("colz");
    canvas->cd(2);
    hist_perc->Draw("text");
    canvas->cd(3);
    hist_stack_perc->Draw();
    canvas->cd(4);
    hist_exclude->Draw("colz");
    canvas->Write();
    out->Close();
}

#endif