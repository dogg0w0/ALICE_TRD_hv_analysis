#include "analysis_hv.hpp"

void analysis_hv::Loop()
{
    Long64_t nentries = fChain->GetEntriesFast();
    auto myfile = new TFile(outfile_name.c_str(), "UPDATE");
    TDirectory *subdir = (gDirectory->FindObjectAny(Form("luminosity_%d", luminosity_index))) ? (TDirectory *)gDirectory->FindObjectAny(Form("luminosity_%d", luminosity_index)) : myfile->mkdir(Form("luminosity_%d", luminosity_index));
    subdir->cd();
    TTimeStamp ttime(0, 0);
    auto c0 = new TCanvas((channel_name + "_" + measurement_type).c_str(),
                              (channel_name + "_" + measurement_type).c_str(),
                              10, 10, 800, 600);
    c0->cd();
    auto g = new TGraph();
    Long64_t gentry = 0;
    Long64_t nbytes = 0, nb = 0;
    std::vector<Double_t> hv_v = {};
    for (Long64_t jentry = 0; jentry < nentries; jentry++)
    {
        Long64_t ientry = LoadTree(jentry);
        if (ientry < 0)
            break;
        nb = fChain->GetEntry(jentry);
        nbytes += nb;
        // if (Cut(ientry) < 0) continue;
        if (HV < 0)
        {
            continue;
        }
        if (fSec < start_time)
        {
            continue;
        }
        if (fSec > end_time)
        {
            break;
        }

        ttime.SetSec(fSec);
        ttime.SetNanoSec(fNanoSec);
        hv_v.push_back(HV);
        g->SetPoint(gentry, ttime.AsDouble(), HV);
        gentry++;
    }

    g->SetTitle(channel_name.c_str());
    g->GetXaxis()->SetTimeOffset(0, "gmt");
    g->GetXaxis()->SetTimeDisplay(1);
    g->GetXaxis()->SetLabelOffset(0.02);
    g->GetXaxis()->SetTimeFormat("#splitline{%Y}{#splitline{%d\/%m}{%H\:%M\:%S}}");
    g->GetXaxis()->SetTitle("Time");
    g->GetYaxis()->SetTitle("HV [V]");
    g->SetMarkerStyle(8);
    g->SetMarkerSize(0.5);
    g->SetMarkerColorAlpha(kRed, 0.35);
    g->Draw("ALP");
    c0->Write();
    myfile->Write();
    myfile->Close();
    if (g)
        g->Delete();
    if (c0)
        delete c0;
    Double_t mean = analysis::mean(&hv_v);
    Double_t mean_std = analysis::mean_std(&hv_v, mean);
    return std::pair<Double_t, Double_t>(mean, mean_std);
}