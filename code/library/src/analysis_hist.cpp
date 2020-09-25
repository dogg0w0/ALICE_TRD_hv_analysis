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

histogramms::histogramms(Int_t sector_n, std::vector<std::string> &luminosity_labels, std::vector<Double_t> &luminosity_points)
{
    sector = sector_n;
    histogramms::ChannelNames();
    histogramms::Canvas();
    histogramms::HistSector();
    histogramms::HistLumi(luminosity_labels, luminosity_points);
    histogramms::HistPerc();
    histogramms::HistOffset();
}

histogramms::~histogramms()
{
    if (canvas)
        delete canvas;
    if (hist_sector)
        delete hist_sector;
    if (hist_perc)
        delete hist_perc;
    if (hist_lumi)
        delete hist_lumi;
    if (hist_offset)
        delete hist_offset;
    if (gr_lumi_fit)
        delete gr_lumi_fit;
}

void histogramms::Draw(Double_t overall_mean_current, Int_t luminosity_index, std::map<Int_t, std::map<Int_t, Double_t>> &mean_current_map,
                       std::map<Int_t, std::map<Int_t, Bool_t>> &mean_hv_map,
                       std::map<Int_t, std::map<Int_t, Double_t>> &mean_offset_map)
{
    canvas->SetName(Form("sector_%d_%d", sector, luminosity_index));
    canvas->SetTitle(Form("Sector %d Luminosity %d", sector, luminosity_index));
    hist_perc->SetTitle(Form("Anode Current to Baseline %f [#muA] in Sector 6", overall_mean_current));
    Double_t average_all_chambers = 0;
    Double_t number_active_chambers = 0;
    for (Int_t stack = 0; stack < 5; stack++)
    {
        for (Int_t layer = 0; layer < 6; layer++)
        {
            hist_sector->SetBinContent(stack + 1, layer + 1, (!mean_hv_map[stack][layer]) ? 0 : mean_current_map[stack][layer]);
            hist_offset->SetBinContent(stack + 1, layer + 1, (!mean_hv_map[stack][layer]) ? 0 : mean_offset_map[stack][layer]);
            hist_perc->SetBinContent(stack + 1, layer + 1,
                                     (!mean_hv_map[stack][layer]) ? 0 : mean_current_map[stack][layer] / overall_mean_current);
            if (mean_hv_map[stack][layer])
            {
                average_all_chambers += mean_current_map[stack][layer];
                number_active_chambers++;
            }
        }
    }
    // Calculate Mean
    average_all_chambers /= number_active_chambers;
    // Calculate RMS
    Double_t standardDeviation = 0,
             rms = 0;
    for (Int_t stack = 0; stack < 5; stack++)
    {
        for (Int_t layer = 0; layer < 6; layer++)
        {
            if (mean_hv_map[stack][layer])
            {
                standardDeviation += TMath::Power(mean_current_map[stack][layer] - average_all_chambers, 2);
            }
        }
    }
    rms = TMath::Sqrt(standardDeviation / (number_active_chambers));
    std::cout << "Average plus error: " << average_all_chambers << " +- " << rms << std::endl;
    mean_current_all_chambers_err.push_back(rms);
    mean_current_all_chambers.push_back(average_all_chambers);
}

void histogramms::Canvas()
{
    canvas = new TCanvas(Form("sector_%d", sector), Form("Sector %d", sector), 10, 10, 1800, 600);
    canvas->Divide(3, 1);
    canvas->SetLeftMargin(0.15);
    canvas->SetBottomMargin(0.15);
    canvas->GetPad(1)->SetRightMargin(0.12);
    canvas->GetPad(2)->SetRightMargin(0.12);
    canvas->GetPad(3)->SetRightMargin(0.12);
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

void histogramms::HistLumi(std::vector<std::string> &luminosity_labels, std::vector<Double_t> &luminosity_points)
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
    hist_lumi->GetZaxis()->SetTitle("Current [#muA]");
    hist_lumi->SetStats(0);
    for (auto &&element : luminosity_points)
    {
        luminosities.push_back(element);
    }
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
    TDirectory *plots = (gDirectory->FindObjectAny("plots")) ? (TDirectory *)gDirectory->FindObjectAny("plots") : out->mkdir("plots");
    plots->cd();
    gStyle->SetPaintTextFormat(".2f");
    canvas->cd(1);
    hist_sector->Draw("colz");
    canvas->cd(2);
    hist_perc->Draw("text");
    canvas->cd(3);
    hist_offset->Draw("text");
    canvas->Write();
    out->Write();
    out->Close();
}

void histogramms::WriteLumi(std::string time_stamp)
{
    TFile *out = new TFile(Form("sm_%d.root", sector), "UPDATE");
    TDirectory *plots = (gDirectory->FindObjectAny("plots")) ? (TDirectory *)gDirectory->FindObjectAny("plots") : out->mkdir("plots");
    plots->cd();

    gr_lumi_fit = new TGraphErrors(luminosities.size());
    gr_lumi_fit->SetName(Form("sector_%d_hist_lumi_fit", sector));
    gr_lumi_fit->SetTitle("Average Anode Current");
    gr_lumi_fit->GetXaxis()->SetTitle("Luminosity [Hz/#mub]");
    gr_lumi_fit->GetYaxis()->SetTitle("Current [#muA]");

    for (Int_t i = 0; i < luminosities.size(); i++)
    {
        gr_lumi_fit->SetPoint(i, luminosities[i], mean_current_all_chambers[i]);
        gr_lumi_fit->SetPointError(i, 0.0, mean_current_all_chambers_err[i]);
    }

    gr_lumi_fit->Fit("pol1");
    TF1 *fit = gr_lumi_fit->GetFunction("pol1");
    hist_lumi->SetTitle(("Anode Current during HL @ " + time_stamp).c_str());
    TCanvas *c0 = new TCanvas(Form("sector_lumi_%d", sector), Form("Sector %d", sector), 10, 10, 800, 600);
    c0->SetLeftMargin(0.15);
    c0->SetBottomMargin(0.15);
    c0->Divide(2, 1);
    c0->GetPad(1)->SetRightMargin(0.12);
    c0->GetPad(1)->SetLeftMargin(0.12);
    c0->GetPad(1)->SetGrid();
    c0->cd(1);
    hist_lumi->Draw("colz");
    c0->cd(2);
    gr_lumi_fit->Draw("AP");
    // Fit Results

    TLatex *tex = new TLatex();
    tex->SetNDC(kTRUE);
    tex->SetTextSize(0.035);
    tex->SetTextColor(kBlack);
    tex->DrawLatex(0.18, 0.75, "Pol1 Fit f(x) = a + b*x");
    char buffer_a[100],
        buffer_b[100],
        buffer_Chi[100];
    std::sprintf(buffer_a, "a = %.3f #pm %.3f", fit->GetParameter(0), fit->GetParError(0));
    std::sprintf(buffer_b, "b = %.3f #pm %.3f", fit->GetParameter(1), fit->GetParError(1));
    std::sprintf(buffer_Chi, "#chi^{2}_{red} = %.2f", fit->GetChisquare() / fit->GetNDF());
    tex->DrawLatex(0.20, 0.70, buffer_a);
    tex->DrawLatex(0.20, 0.65, buffer_b);
    tex->DrawLatex(0.20, 0.60, buffer_Chi);
    c0->Write();
    out->Write();
    out->Close();
    delete c0;
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