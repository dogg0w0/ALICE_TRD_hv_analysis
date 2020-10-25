{
    auto g = new TGraph();
    for (int i = 0; i < 200; i++)
    {
        g->SetPoint(i, i, i);
    }
    auto h = new TH1D("h", "title", 10, 0, 200);
    for (int i = 0; i < g->GetN(); i++)
    {
        h->Fill(g->GetPointX(i), g->GetPointY(i));
    }
    h->Draw();
}