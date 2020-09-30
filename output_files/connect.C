{
    TCanvas *c0 = new TCanvas("c0", "Offset Distribution", 10, 10, 1800, 1600);
    c0->Divide(6, 3);
    //c0->Draw("");

    //for (Int_t i = 0; i < 18; i++)
    //{
    //    c0->cd(i+1);
    //    TFile f(Form("sm_%d.root", i), "READ");
    //    TH1D *h_b;
    //    f.GetObject("fits/h_b;1", h_b);
//
    //    h_b->Draw("");
    //    delete h_b;
    //    f.Close();
//
    //}

    TH1D *h_b;
    Int_t i = 1;
    c0->cd(i);
    TFile f1(Form("sm_%d.root", i), "READ");
    f1.GetObject("fits/h_b;1", h_b);
    h_b->Draw("");
    i++;

    c0->cd(i);
    TFile f2(Form("sm_%d.root", i), "READ");
    f2.GetObject("fits/h_b;1", h_b);
    h_b->Draw("");
    i++;

    c0->cd(i);
    TFile f3(Form("sm_%d.root", i), "READ");
    f3.GetObject("fits/h_b;1", h_b);
    h_b->Draw("");
    i++;

    c0->cd(i);
    TFile f4(Form("sm_%d.root", i), "READ");
    f4.GetObject("fits/h_b;1", h_b);
    h_b->Draw("");
    i++;

    c0->cd(i);
    TFile f5(Form("sm_%d.root", i), "READ");
    f5.GetObject("fits/h_b;1", h_b);
    h_b->Draw("");
    i++;

    c0->cd(i);
    TFile f6(Form("sm_%d.root", i), "READ");
    f6.GetObject("fits/h_b;1", h_b);
    h_b->Draw("");
    i++;

    c0->cd(i);
    TFile f7(Form("sm_%d.root", i), "READ");
    f7.GetObject("fits/h_b;1", h_b);
    h_b->Draw("");
    i++;
}