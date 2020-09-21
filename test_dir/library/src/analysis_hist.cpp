#include "analysis_hist.hpp"

histogramms::histogramms(Int_t sector_n)
{
    sector = sector_n;
    histogramms::ChannelNames();
    histogramms::Canvas();
    histogramms::HistSector();
    histogramms::HistPerc();
    histogramms::HistOffset();
}

histogramms::histogramms(Int_t sector_n, std::vector<std::string> &luminosity_labels)
{
    sector = sector_n;
    histogramms::ChannelNames();
    histogramms::Canvas();
    histogramms::HistSector();
    histogramms::HistLumi(luminosity_labels);
    histogramms::HistPerc();
    histogramms::HistOffset();
}

histogramms::~histogramms()
{
    if (canvas)
        delete canvas;
    if (hist_sector)
        hist_sector->Delete();
    if (hist_perc)
        hist_perc->Delete();
    if (hist_lumi)
        hist_lumi->Delete();
    if (hist_offset)
        hist_offset->Delete();
}

void histogramms::Draw(Double_t overall_mean_current, std::map<Int_t, std::map<Int_t, Double_t>> &mean_current_map,
                       std::map<Int_t, std::map<Int_t, Bool_t>> &mean_hv_map,
                       std::map<Int_t, std::map<Int_t, Double_t>> &mean_offset_map)
{
    hist_perc->SetTitle(Form("Anode Current to Baseline %f [#muA] in Sector 6", overall_mean_current));
    for (Int_t stack = 0; stack < 5; stack++)
    {
        for (Int_t layer = 0; layer < 6; layer++)
        {
            hist_sector->SetBinContent(stack + 1, layer + 1, (!mean_hv_map[stack][layer]) ? 0 : mean_current_map[stack][layer]);
            hist_offset->SetBinContent(stack + 1, layer + 1, (!mean_hv_map[stack][layer]) ? 0 : mean_offset_map[stack][layer]);
            hist_perc->SetBinContent(stack + 1, layer + 1,
                                     (!mean_hv_map[stack][layer]) ? 0 : mean_current_map[stack][layer] / overall_mean_current);
        }
    }
}

void histogramms::Canvas()
{
    canvas = new TCanvas(Form("sector_%d", sector), Form("Sector %d", sector), 10, 10, 1800, 600);
    canvas->Divide(3, 1);
    canvas->SetLeftMargin(0.15);
    canvas->SetBottomMargin(0.15);
    canvas->GetPad(1)->SetGrid();
    canvas->GetPad(2)->SetGrid();
    canvas->GetPad(3)->SetGrid();
}

void histogramms::HistSector()
{
    hist_sector = new TH2D(Form("sector_%d_hist", sector), Form("Mean Anode Current in Sector %d", sector), 5, 0, 5, 6, 0, 6);
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
}

void histogramms::HistLumi(std::vector<std::string> &luminosity_labels)
{
    hist_lumi = new TH2D(Form("sector_%d_hist_lumi", sector), "Anode Current during high luminosity", luminosity_labels.size(), 0, luminosity_labels.size(), 30, 0, 30);
    for (Int_t luminosity = 0; luminosity < (Int_t)luminosity_labels.size(); luminosity++)
    {
        hist_lumi->GetXaxis()->SetBinLabel(luminosity + 1, luminosity_labels[luminosity].c_str());
    }
    for (Int_t channels = 0; channels < 30; channels++)
    {
        hist_lumi->GetYaxis()->SetBinLabel(channels + 1, channel_labels[channels].c_str());
    }
    hist_lumi->GetXaxis()->SetTitle("Luminosity");
    hist_lumi->GetYaxis()->SetTitle("Channel");
    hist_lumi->GetZaxis()->SetTitle("Percentage");
    hist_lumi->SetStats(0);
}

void histogramms::HistPerc()
{
    hist_perc = new TH2D(Form("sector_%d_hist_perc", sector), "", 5, 0, 5, 6, 0, 6);
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
}

void histogramms::HistOffset()
{
    hist_offset = new TH2D(Form("sector_%d_hist_offset", sector), "Offset [#muA]", 5, 0, 5, 6, 0, 6);
    hist_offset->GetXaxis()->SetTitle("Stack");
    hist_offset->GetYaxis()->SetTitle("Layer");
    hist_offset->GetXaxis()->SetNdivisions(8, 8, 0, kTRUE);
    for (Int_t stack = 1; stack < 6; stack++)
    {
        hist_offset->GetXaxis()->SetBinLabel(stack, Form("%d", stack - 1));
    }
    for (Int_t layer = 1; layer < 7; layer++)
    {
        hist_offset->GetYaxis()->SetBinLabel(layer, Form("%d", layer - 1));
    }
    hist_offset->SetStats(0);
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
    hist_offset->Draw("text");
    canvas->Write();
    out->Close();
}

void histogramms::WriteLumi()
{
    TFile *out = new TFile(Form("sm_%d.root", sector), "UPDATE");
    TCanvas *c0 = new TCanvas(Form("sector_lumi_%d", sector), Form("Sector %d", sector), 10, 10, 800, 600);
    c0->SetLeftMargin(0.15);
    c0->SetBottomMargin(0.15);
    c0->SetGrid();
    //c0->GetPad(1)->SetGrid();
    hist_lumi->Draw("colz");
    c0->Write();
    out->Close();
}

void histogramms::ChannelNames()
{
    for (Int_t stack = 0; stack < 5; stack++)
    {
        for (Int_t layer = 0; layer < 6; layer++)
        {
            channel_labels.push_back(Form("%02d_%d_%d", sector, stack, layer));
        }
    }
}