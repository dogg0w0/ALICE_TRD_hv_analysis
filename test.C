void test()
{
    TCanvas *c0 = new TCanvas("sector_6", "Sector 6", 100, 100, 800, 800);
    TH2F *hist = new TH2F("sector_6_hist", "Mean Anode Current in Sector 6", 5, 0, 5, 6, 0, 6);
    hist->GetXaxis()->SetTitle("Stack");
    hist->GetYaxis()->SetTitle("Layer");
    hist->GetZaxis()->SetTitle("Current [#muA]");
    hist->SetStats(0);
    i);
    hist->Draw("colz");
    TAxis *a = hist->GetXaxis();
    a->SetNdivisions(8, 8, 0, kTRUE);
    c0->Draw();
}