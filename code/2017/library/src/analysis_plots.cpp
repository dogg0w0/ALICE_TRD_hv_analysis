#include "analysis_plots.hpp"

plots::plots(const Int_t sector_n)
{
    sector = sector_n;
    plots::ChannelNames();
    plots::Canvas();
    plots::HistSector();
    plots::HistPerc();
    plots::HistOffset();
}

plots::plots(const Int_t sector_n, const std::vector<std::string> &luminosity_labels, const std::vector<Double_t> &luminosity_points, std::string gain_map)
{
    sector = sector_n;
    plots::ChamberWeightsInit();
    //plots::GainWeightsInit(gain_map);
    plots::RadialWeightsInit();
    plots::WeightsInit();
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

void plots::HistLumi(const std::vector<std::string> &luminosity_labels, const std::vector<Double_t> &luminosity_points)
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
    auto out = new TFile(Form("sm_%d.root", sector), "UPDATE");
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

void plots::WriteLumi(const std::string time_stamp)
{
    auto out = new TFile(Form("sm_%d.root", sector), "UPDATE");
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
    auto c0 = new TCanvas(Form("sector_lumi_%d", sector), Form("Sector %d", sector), 10, 10, 800, 600);
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

void plots::FitInit(const std::vector<std::string> &luminosity_labels)
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

void plots::FitUpdate(const Int_t luminosity_index, std::map<Int_t, std::map<Int_t, Double_t>> &mean_current_map,
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
    auto out = new TFile(Form("sm_%d.root", sector), "UPDATE");
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

            auto c0 = new TCanvas(Form("sector_lumi_fit_%02d_%d_%d", sector, stack, layer), Form("Sector %02d_%d_%d", sector, stack, layer), 10, 10, 800, 600);
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

    out->Write();
    out->Close();

    delete tex;
    plots::FitSlopeOffset(fit_a_v, fit_b_v);
}

void plots::FitSlopeOffset(const std::vector<Double_t> &fit_a_v, const std::vector<Double_t> &fit_b_v)
{
    auto out = new TFile(Form("sm_%d.root", sector), "UPDATE");
    TDirectory *fits = (gDirectory->FindObjectAny("fits")) ? (TDirectory *)gDirectory->FindObjectAny("fits") : out->mkdir("fits");
    fits->cd();

    // Write Tree for correlation with gain
    TTree tree_fit_a("fit_params", Form("Fit Parameters for Sector %d", sector));
    Double_t fit_a, fit_b;
    tree_fit_a.Branch("fit_a", &fit_a, "fit_a/D");
    tree_fit_a.Branch("fit_b", &fit_b, "fit_b/D");
    for (Int_t i = 0; i < 30; i++)
    {
        fit_a = fit_a_v[i];
        fit_b = fit_b_v[i];
        tree_fit_a.Fill();
    }
    tree_fit_a.Write();

    // Histogramms for slope and offset distribution
    Int_t nbins = 30;
    auto h_a = new TH1D("h_a", "Distribution of Offset", nbins, 0, 0.1);
    auto h_b = new TH1D("h_b", "Distribution of Slope", nbins, 0, 0.2);

    auto h_b_0 = new TH1D("h_b_0", "Distribution of Slope 0", nbins, 0, 0.2);
    auto h_b_1 = new TH1D("h_b_1", "Distribution of Slope 1", nbins, 0, 0.2);
    auto h_b_2 = new TH1D("h_b_2", "Distribution of Slope 2", nbins, 0, 0.2);
    auto h_b_3 = new TH1D("h_b_3", "Distribution of Slope 3", nbins, 0, 0.2);
    auto h_b_4 = new TH1D("h_b_4", "Distribution of Slope 4", nbins, 0, 0.2);

    auto h_a_0 = new TH1D("h_a_0", "Distribution of Offset 0", nbins, 0, 0.1);
    auto h_a_1 = new TH1D("h_a_1", "Distribution of Offset 1", nbins, 0, 0.1);
    auto h_a_2 = new TH1D("h_a_2", "Distribution of Offset 2", nbins, 0, 0.1);
    auto h_a_3 = new TH1D("h_a_3", "Distribution of Offset 3", nbins, 0, 0.1);
    auto h_a_4 = new TH1D("h_a_4", "Distribution of Offset 4", nbins, 0, 0.1);

    for (Int_t i = 0; i < 6; i++)
    {
        //h_b_0->Fill(fit_b_v[i], weights[i + 0 * 6 + sector*30]);
        //h_b_1->Fill(fit_b_v[i + 1 * 6], weights[i + 1 * 6 + sector*30]);
        //h_b_2->Fill(fit_b_v[i + 2 * 6], weights[i + 2 * 6 + sector*30]);
        //h_b_3->Fill(fit_b_v[i + 3 * 6], weights[i + 3 * 6 + sector*30]);
        //h_b_4->Fill(fit_b_v[i + 4 * 6], weights[i + 4 * 6 + sector*30]);
        //h_a_0->Fill(fit_a_v[i], weights[i + 0 * 6 + sector * 30]);
        //h_a_1->Fill(fit_a_v[i + 1 * 6], weights[i + 1 * 6 + sector*30]);
        //h_a_2->Fill(fit_a_v[i + 2 * 6], weights[i + 2 * 6 + sector*30]);
        //h_a_3->Fill(fit_a_v[i + 3 * 6], weights[i + 3 * 6 + sector*30]);
        //h_a_4->Fill(fit_a_v[i + 4 * 6], weights[i + 4 * 6 + sector*30]);

        h_b_0->Fill(fit_b_v[i]);
        h_b_1->Fill(fit_b_v[i + 1 * 6]);
        h_b_2->Fill(fit_b_v[i + 2 * 6]);
        h_b_3->Fill(fit_b_v[i + 3 * 6]);
        h_b_4->Fill(fit_b_v[i + 4 * 6]);
        h_a_0->Fill(fit_a_v[i]);
        h_a_1->Fill(fit_a_v[i + 1 * 6]);
        h_a_2->Fill(fit_a_v[i + 2 * 6]);
        h_a_3->Fill(fit_a_v[i + 3 * 6]);
        h_a_4->Fill(fit_a_v[i + 4 * 6]);
    }

    h_a->GetXaxis()->SetTitle("a");
    h_a->GetYaxis()->SetTitle("entries");
    h_b->GetXaxis()->SetTitle("b");
    h_b->GetYaxis()->SetTitle("entries");

    //for (Int_t i = n_not_working_chambers; i < fit_a_v.size(); i++)
    for (Int_t i = 0; i < 30; i++)
    {
        //h_a->Fill(fit_a_v[i], weights[i+sector*30]);
        //h_b->Fill(fit_b_v[i], weights[i+sector*30]);
        h_a->Fill(fit_a_v[i]);
        h_b->Fill(fit_b_v[i]);
    }
    // Fit
    h_b->Fit("gaus", "WW", "", 0.04, 0.2);
    h_b->SetStats(0);

    auto c1 = new TCanvas(Form("sector_lumi_fit_%d", sector), Form("Sector %02d", sector), 10, 10, 800, 600);
    c1->Divide(2, 1);
    c1->cd(1);
    h_a->Draw("");

    c1->cd(2);
    h_b->Draw("");

    // Fit Results
    TLatex *tex = new TLatex();
    tex->SetNDC(kTRUE);
    tex->SetTextSize(0.035);
    tex->SetTextColor(kBlack);
    TF1 *fgauss = h_b->GetFunction("gaus");
    tex->DrawLatex(0.60, 0.85, "Gauss Fit");
    char buffer_mu[100],
        buffer_S[100],
        buffer_Chi[100];
    sprintf(buffer_mu, "#mu = %.3f #pm %.3f", fgauss->GetParameter(1), fgauss->GetParError(1));
    sprintf(buffer_S, "#sigma = %.3f #pm %.3f", fgauss->GetParameter(2), fgauss->GetParError(2));
    sprintf(buffer_Chi, "#chi^{2}_{red} = %.1f", fgauss->GetChisquare() / fgauss->GetNDF());
    tex->DrawLatex(0.60, 0.80, buffer_mu);
    tex->DrawLatex(0.60, 0.75, buffer_S);
    tex->DrawLatex(0.60, 0.70, buffer_Chi);

    c1->Write();
    out->Write();
    out->Close();

    if (tex)
        delete tex;
    if (c1)
        delete c1;
}

void plots::ChamberWeightsInit()
{
    Double_t norm = 1060 * 1060;
    std::vector<Double_t> area = {
        1200 * 1200,
        1200 * 1200,
        1270 * 1270,
        1340 * 1340,
        1410 * 1410,
        1430 * 1430, // Stack 0
        1200 * 1200,
        1200 * 1200,
        1270 * 1270,
        1340 * 1340,
        1410 * 1410,
        1430 * 1430, // Stack 1
        1060 * 1060,
        1060 * 1060,
        1060 * 1060,
        1060 * 1060,
        1060 * 1060,
        1060 * 1060, // Stack 2
        1200 * 1200,
        1200 * 1200,
        1270 * 1270,
        1340 * 1340,
        1410 * 1410,
        1430 * 1430, // Stack 3
        1200 * 1200,
        1200 * 1200,
        1270 * 1270,
        1340 * 1340,
        1410 * 1410,
        1430 * 1430, // Stack 4
    };

    for (Int_t i = 0; i < 30; i++)
    {
        // Larger chambers produce more current than smaller ones,
        // thus one needs to correct them by considering their surface
        // area.
        chambers_weights[i] = (1 / (area[i] / norm));
        // Norm to absolute chamber size
        //chambers_weights.push_back(1 / area[i]);
    }
}

void plots::GainWeightsInit(std::string gain_map)
{
    // Open the ROOT file.
    TFile *f = TFile::Open(gain_map.c_str(), "READ");
    if (f == 0)
    {

        // If we cannot open the ROOT file, print an error message and return immediately.
        printf("Error: cannot open file");
        return;
    }

    Double_t run;
    Float_t gain;
    Double_t absolute_mean = 0.0;

    TBranch *gainBranch = 0;
    TBranch *runBranch = 0;

    std::vector<Double_t> meangain(540, 0.0);
    //vector<Double_t> gain_v(1620, 0.0);

    // Get a pointer to the tree.
    TTree *tree = (TTree *)f->Get("Results");

    // Use SetBranchAddress() with simple types (e.g. double, int) instead of objects (e.g. std::vector<Particle>).
    tree->SetMakeClass(1);

    // Connect the branch "fEventSize" with the variable eventSize that we want to contain the data.
    // While we are at it, ask the tree to save the branch in eventSizeBranch.
    tree->SetBranchAddress("gain", &gain, &gainBranch);
    //tree->SetBranchAddress("run", &run, &runBranch);
    // First, get the total number of entries.
    Long64_t nentries = tree->GetEntries();

    // Then loop over all of them.
    for (Long64_t i = 0; i < nentries; i++)
    {
        tree->GetEntry(i);
        meangain[i % 540] += gain;
        absolute_mean += gain;
        //gain_v[i] = gain;
    }
    // Mean
    for (Int_t i = 0; i < 540; i++)
    {
        meangain[i] /= 3;
    }
    absolute_mean /= nentries;
    for (Int_t i = 0; i < 540; i++)
    {
        gain_weights[i] = (meangain[i] / absolute_mean);
    }
}

void plots::RadialWeightsInit()
{
    for (Int_t i = 0; i < 6; i++)
    {
        radial_weights[i] = 0.13 * (i + 1) / 0.78;
    }
}

void plots::WeightsInit()
{
    Double_t _temp;
    for (Int_t sector = 0; sector < 18; sector++)
    {
        for (Int_t stack = 0; stack < 5; stack++)
        {
            for (Int_t layer = 0; layer < 6; layer++)
            {
                _temp = gain_weights[sector * 30 + stack * 6 + layer] * chambers_weights[layer + stack * 6] * radial_weights[layer];
                weights.push_back(_temp);
            }
        }
    }
}