#include "analysis_plots.hpp"

plots::plots(const Int_t sector_n)
{
    sector = sector_n;
    plots::ChamberWeightsInit();
    //plots::GainWeightsInit(gain_map);
    //plots::RadialWeightsInit();
    plots::WeightsInit();
    plots::ChannelNames();
    plots::Canvas();
    plots::HistOffset();
}

plots::plots(const Int_t sector_n, std::string gain_map, const Int_t gain_index)
{
    sector = sector_n;
    plots::ChamberWeightsInit();
    //plots::GainWeightsInit(gain_map, gain_index);
    //plots::RadialWeightsInit();
    plots::WeightsInit();
    plots::ChannelNames();
    plots::Canvas();
    plots::HistOffset();
}

plots::~plots()
{
    if (canvas)
        delete canvas;
    if (hist_offset)
        delete hist_offset;
    for (auto &&ptr : gr_lumi_fit_single_t0)
    {
        delete ptr;
    }
    for (auto &&ptr : gr_lumi_fit_single_tof)
    {
        delete ptr;
    }
}

void plots::Draw(std::map<Int_t, std::map<Int_t, Bool_t>> &mean_hv_map,
                 std::map<Int_t, std::map<Int_t, Double_t>> &mean_offset_map)
{
    canvas->SetName(Form("sector_%d_offset", sector));
    canvas->SetTitle(Form("Sector %d Offset Map", sector));
    Double_t average_all_chambers = 0;
    Double_t number_active_chambers = 0;
    for (Int_t stack = 0; stack < 5; stack++)
    {
        for (Int_t layer = 0; layer < 6; layer++)
        {
            hist_offset->SetBinContent(stack + 1, layer + 1, (!mean_hv_map[stack][layer]) ? 0 : mean_offset_map[stack][layer]);
        }
    }
}

void plots::Canvas()
{
    canvas = new TCanvas(Form("sector_%d", sector), Form("Sector %d", sector), 10, 10, 1600, 900);
    canvas->GetPad(0)->SetGrid();
}

void plots::HistOffset()
{
    hist_offset = new TH2D(Form("sector_%d_hist_offset", sector), "Offset (#muA)", 5, 0, 5, 6, 0, 6);
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
    canvas->cd();
    hist_offset->Draw("text");
    canvas->Write();
    out->Write();
    out->Close();
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

void plots::FitInit()
{
    for (Int_t stack = 0; stack < 5; stack++)
    {
        for (Int_t layer = 0; layer < 6; layer++)
        {
            gr_lumi_fit_single_t0[layer + stack * 6]->SetTitle(Form("%02d_%d_%d Fit T0", sector, stack, layer));
            gr_lumi_fit_single_t0[layer + stack * 6]->SetName(Form("%02d_%d_%d_fit_T0", sector, stack, layer));
            gr_lumi_fit_single_t0[layer + stack * 6]->GetXaxis()->SetTitle("Luminosity (Hz/#mub)");
            gr_lumi_fit_single_t0[layer + stack * 6]->GetYaxis()->SetTitle("Current (#muA)");
            gr_lumi_fit_single_tof[layer + stack * 6]->SetTitle(Form("%02d_%d_%d Fit EMCAL", sector, stack, layer));
            gr_lumi_fit_single_tof[layer + stack * 6]->SetName(Form("%02d_%d_%d_fit_EMCAL", sector, stack, layer));
            gr_lumi_fit_single_tof[layer + stack * 6]->GetXaxis()->SetTitle("Luminosity (Hz/#mub)");
            gr_lumi_fit_single_tof[layer + stack * 6]->GetYaxis()->SetTitle("Current (#muA)");
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
            // FOR NOW JUST TOF
            gr_lumi_fit_single_tof[layer + stack * 6]->Fit("pol1", "Q");
            TF1 *fit = gr_lumi_fit_single_tof[layer + stack * 6]->GetFunction("pol1");
            fit_a_v.push_back(fit->GetParameter(0));
            fit_b_v.push_back(fit->GetParameter(1));

            auto c0 = new TCanvas(Form("sector_lumi_fit_%02d_%d_%d", sector, stack, layer), Form("Sector %02d_%d_%d", sector, stack, layer), 10, 10, 800, 600);
            c0->GetPad(0)->SetGrid();
            gr_lumi_fit_single_tof[layer + stack * 6]->Draw("AP");

            // Fit Results
            tex->DrawLatex(0.18, 0.75, "Pol1 Fit f(x) = a + b*x");
            std::sprintf(buffer_a, "a = %.3f #pm %.3f (#muA)", fit->GetParameter(0), fit->GetParError(0));
            std::sprintf(buffer_b, "b = %.3f #pm %.3f (#muA/(Hz/#mub))", fit->GetParameter(1), fit->GetParError(1));
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
    Double_t xbins = 0.15;
    auto h_a = new TH1D("h_a", "Distribution of Offset", nbins, 0, 0.1);
    auto h_b = new TH1D("h_b", "Distribution of Slope", nbins, 0, xbins);

    auto h_b_0 = new TH1D("h_b_0", "Distribution of Slope 0", nbins, 0, xbins);
    auto h_b_1 = new TH1D("h_b_1", "Distribution of Slope 1", nbins, 0, xbins);
    auto h_b_2 = new TH1D("h_b_2", "Distribution of Slope 2", nbins, 0, xbins);
    auto h_b_3 = new TH1D("h_b_3", "Distribution of Slope 3", nbins, 0, xbins);
    auto h_b_4 = new TH1D("h_b_4", "Distribution of Slope 4", nbins, 0, xbins);

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
    h_b->Fit("gaus", "WWQ", "", 0.04, xbins);
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

void plots::GainWeightsInit(const std::string gain_map, const Int_t gain_index)
{
    if (gain_index < 0)
        return;
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
    Long64_t nentries = tree->GetEntriesFast();
    Long64_t start_index = 0 + 540 * gain_index;
    Long64_t end_index = 0 + 540 * (gain_index + 1);

    // Then loop over all of them.
    for (Long64_t i = start_index; i < end_index; i++)
    {
        tree->GetEntry(i);
        absolute_mean += gain;
    }
    absolute_mean /= nentries;
    for (Long64_t i = start_index; i < end_index; i++)
    {
        tree->GetEntry(i);
        gain_weights[i%540] = (gain / absolute_mean);
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