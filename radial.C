{
    gROOT->SetStyle("Pub");
    gROOT->ForceStyle();
    auto g = new TGraphErrors();
    Double_t m[6] = {0.065, 0.058, 0.052, 0.047, 0.041, 0.037};
    Double_t me[6] = {0.003, 0.003, 0.002, 0.002, 0.002, 0.002};
    float x0[6] = {300.2f, 312.8f, 325.4f, 338.0f, 350.6f, 363.2f};
    for (int i = 0; i < 6; i++)
    {
        g->SetPoint(i, x0[i], m[i]);
        g->SetPointError(i, 0.0, me[i]);
    }
    g->Fit("pol1", "Q");
    TF1* p = g->GetFunction("pol1");
    g->GetXaxis()->SetTitle("radial position of pad-plane (cm)");
    g->GetYaxis()->SetTitle("slope parameter (#muA/(Hz/#mub))");
    g->Draw("AP");

    TLatex *tex = new TLatex();
    tex->SetNDC(kTRUE);
    tex->SetTextSize(0.035);
    tex->SetTextColor(kBlack);
    char buffer_a[100],
        buffer_b[100],
        buffer_Chi[100];
    sprintf(buffer_a, "a = %.3f #pm %.3f", p->GetParameter(0), p->GetParError(0));
    sprintf(buffer_b, "b = %.6f #pm %.6f", p->GetParameter(1), p->GetParError(1));
    sprintf(buffer_Chi, "#chi^{2}_{red} = %.1f", p->GetChisquare() / p->GetNDF());
    tex->DrawLatex(0.6, 0.85, "Pol1 fit: a + b*x");
    tex->DrawLatex(0.6, 0.80, buffer_a);
    tex->DrawLatex(0.6, 0.75, buffer_b);
    tex->DrawLatex(0.6, 0.70, buffer_Chi);
}