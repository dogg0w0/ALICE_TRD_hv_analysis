#include "analysis_plots.hpp"

plots::plots(Int_t sector_n)
{
    sector = sector_n;
    plots::ChannelNames();
    plots::Canvas();
    plots::HistSector();
    plots::HistPerc();
    plots::HistOffset();
}

plots::plots(Int_t sector_n, std::vector<std::string> &luminosity_labels, std::vector<Double_t> &luminosity_points)
{
    sector = sector_n;
    plots::ChannelNames();
    plots::Canvas();
    plots::HistSector();
    plots::HistLumi(luminosity_labels, luminosity_points);
    plots::HistPerc();
    plots::HistOffset();
    plots::FitInit(luminosity_labels);
}

plots::~plots()
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
    for (auto &&ptr : gr_lumi_fit_single)
    {
        delete ptr;
    }
}

void plots::Draw(Double_t overall_mean_current, Int_t luminosity_index, std::map<Int_t, std::map<Int_t, Double_t>> &mean_current_map,
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

void plots::Canvas()
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

void plots::HistSector()
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

void plots::HistLumi(std::vector<std::string> &luminosity_labels, std::vector<Double_t> &luminosity_points)
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

void plots::HistPerc()
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

void plots::HistOffset()
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

void plots::Write()
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

void plots::WriteLumi(std::string time_stamp)
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

void plots::ChannelNames()
{
    for (Int_t stack = 0; stack < 5; stack++)
    {
        for (Int_t layer = 0; layer < 6; layer++)
        {
            channel_labels.push_back(Form("%02d_%d_%d", sector, stack, layer));
        }
    }
}

void plots::FitInit(std::vector<std::string> &luminosity_labels)
{
    for (Int_t stack = 0; stack < 5; stack++)
    {
        for (Int_t layer = 0; layer < 6; layer++)
        {
            gr_lumi_fit_single.push_back(new TGraphErrors(luminosity_labels.size() + 1));
            gr_lumi_fit_single[layer + stack * 6]->SetTitle(Form("%02d_%d_%d Fit Current", sector, stack, layer));
            gr_lumi_fit_single[layer + stack * 6]->SetName(Form("%02d_%d_%d_fit", sector, stack, layer));
            gr_lumi_fit_single[layer + stack * 6]->GetXaxis()->SetTitle("Luminosity [Hz/#mub]");
            gr_lumi_fit_single[layer + stack * 6]->GetYaxis()->SetTitle("Current [#muA]");
            gr_lumi_fit_single[layer + stack * 6]->SetPoint(0, 0.0, 0.0);
            gr_lumi_fit_single[layer + stack * 6]->SetPointError(0, 0.0, 0.0);
        }
    }
}

void plots::FitUpdate(Int_t luminosity_index, std::map<Int_t, std::map<Int_t, Double_t>> &mean_current_map,
                      std::map<Int_t, std::map<Int_t, Double_t>> &mean_std_current_map,
                      std::map<Int_t, std::map<Int_t, Bool_t>> &mean_hv_map)
{
    n_not_working_chambers = 0;
    for (Int_t stack = 0; stack < 5; stack++)
    {
        for (Int_t layer = 0; layer < 6; layer++)
        {
            if (mean_hv_map[stack][layer])
            {
                gr_lumi_fit_single[layer + stack * 6]->SetPoint(luminosity_index, luminosities[luminosity_index], mean_current_map[stack][layer]);
                gr_lumi_fit_single[layer + stack * 6]->SetPointError(luminosity_index, 0.0, mean_std_current_map[stack][layer]);
            }
            else
            {
                n_not_working_chambers++;
                gr_lumi_fit_single[layer + stack * 6]->SetPoint(luminosity_index, luminosities[luminosity_index], 0.0);
                gr_lumi_fit_single[layer + stack * 6]->SetPointError(luminosity_index, 0.0, 0.0);
            }
        }
    }
}

void plots::FitDraw()
{
    TFile *out = new TFile(Form("sm_%d.root", sector), "UPDATE");
    TDirectory *fits = (gDirectory->FindObjectAny("fits")) ? (TDirectory *)gDirectory->FindObjectAny("fits") : out->mkdir("fits");
    fits->cd();

    // Latex
    TLatex *tex = new TLatex();
    tex->SetNDC(kTRUE);
    tex->SetTextSize(0.035);
    tex->SetTextColor(kBlack);
    char buffer_a[100],
        buffer_b[100],
        buffer_Chi[100];

    // Plot for fit params
    std::vector<Double_t> fit_a_v, fit_b_v;
    for (Int_t stack = 0; stack < 5; stack++)
    {
        for (Int_t layer = 0; layer < 6; layer++)
        {
            gr_lumi_fit_single[layer + stack * 6]->Fit("pol1");
            TF1 *fit = gr_lumi_fit_single[layer + stack * 6]->GetFunction("pol1");
            fit_a_v.push_back(fit->GetParameter(0));
            fit_b_v.push_back(fit->GetParameter(1));

            TCanvas *c0 = new TCanvas(Form("sector_lumi_fit_%02d_%d_%d", sector, stack, layer), Form("Sector %02d_%d_%d", sector, stack, layer), 10, 10, 800, 600);
            c0->SetTopMargin(0.15);
            c0->SetBottomMargin(0.15);
            c0->GetPad(0)->SetRightMargin(0.12);
            c0->GetPad(0)->SetLeftMargin(0.12);
            c0->GetPad(0)->SetGrid();
            gr_lumi_fit_single[layer + stack * 6]->Draw("AP");

            // Fit Results
            tex->DrawLatex(0.18, 0.75, "Pol1 Fit f(x) = a + b*x");
            std::sprintf(buffer_a, "a = %.3f #pm %.3f", fit->GetParameter(0), fit->GetParError(0));
            std::sprintf(buffer_b, "b = %.3f #pm %.3f", fit->GetParameter(1), fit->GetParError(1));
            std::sprintf(buffer_Chi, "#chi^{2}_{red} = %.2f", fit->GetChisquare() / fit->GetNDF());
            tex->DrawLatex(0.20, 0.70, buffer_a);
            tex->DrawLatex(0.20, 0.65, buffer_b);
            tex->DrawLatex(0.20, 0.60, buffer_Chi);
            c0->Write();
            delete c0;
        }
    }
    std::sort(fit_a_v.begin(), fit_a_v.end());
    std::sort(fit_b_v.begin(), fit_b_v.end());

    TCanvas *c0 = new TCanvas(Form("sector_lumi_fit_%d", sector), Form("Sector %02d", sector), 10, 10, 800, 600);
    c0->Divide(2, 1);
    // Histogramms for slope and offset distribution, bins with sturge's rule
    Int_t nbins = TMath::Nint(1 + TMath::Log2(30 - n_not_working_chambers));
    TH1D *h_a = new TH1D("h_a", "Distribution of Offset", nbins, fit_a_v[n_not_working_chambers], fit_a_v.back());
    TH1D *h_b = new TH1D("h_b", "Distribution of Slope", nbins, fit_b_v[n_not_working_chambers], fit_b_v.back());

    for (Int_t i = n_not_working_chambers; i < fit_a_v.size(); i++)
    {
        h_a->Fill(fit_a_v[i]);
        h_b->Fill(fit_b_v[i]);
    }
    // Fit
    h_b->Fit("gaus", "WW");
    h_b->SetStats(0);
    TF1* fgauss = h_b->GetFunction("gaus");
    tex->DrawLatex(0.65, 0.85, "Gauss Fit");
    char buffer_M[100],
        buffer_S[100];
    sprintf(buffer_M, "M_{Z} = %.3f #pm %.3f", fgauss->GetParameter(1), fgauss->GetParError(1));
    sprintf(buffer_S, "#sigma = %.3f #pm %.3f", fgauss->GetParameter(2), fgauss->GetParError(2));
    sprintf(buffer_Chi, "#chi^{2}_{red} = %.1f", fgauss->GetChisquare() / fgauss->GetNDF());
    tex->DrawLatex(0.65, 0.80, buffer_M);
    tex->DrawLatex(0.65, 0.75, buffer_S);
    tex->DrawLatex(0.65, 0.70, buffer_Chi);

    c0->cd(1);
    h_a->Draw("");
    c0->cd(2);
    h_b->Draw("");
    c0->Write();
    out->Write();
    out->Close();

    delete tex;
    delete c0;
}