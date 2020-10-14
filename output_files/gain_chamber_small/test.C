{
    auto h = new TH1D("h", "Title;x;entries", 20, -3, 3);
    h->Sumw2();
    h->FillRandom("gaus", 10000);
    Double_t scale = 1/h->Integral();
    h->Scale(scale);
    
    auto g = new TGraphErrors(h);
    g->Draw();
}