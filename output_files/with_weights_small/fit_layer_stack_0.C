{
    //..BABAR style from RooLogon.C in workdir
    TStyle *babarStyle = new TStyle("BABAR", "BaBar approved plots style");

    // use plain black on white colors
    babarStyle->SetFrameBorderMode(0);
    babarStyle->SetCanvasBorderMode(0);
    babarStyle->SetPadBorderMode(0);
    babarStyle->SetPadColor(0);
    babarStyle->SetCanvasColor(0);
    babarStyle->SetStatColor(0);
    babarStyle->SetFillColor(0);

    // set the paper & margin sizes
    babarStyle->SetPaperSize(20, 26);
    babarStyle->SetPadTopMargin(0.05);
    babarStyle->SetPadRightMargin(0.05);
    babarStyle->SetPadBottomMargin(0.16);
    babarStyle->SetPadLeftMargin(0.12);

    // use large Times-Roman fonts
    babarStyle->SetTextFont(132);
    babarStyle->SetTextSize(0.08);
    babarStyle->SetLabelFont(132, "x");
    babarStyle->SetLabelFont(132, "y");
    babarStyle->SetLabelFont(132, "z");
    babarStyle->SetLabelSize(0.05, "x");
    babarStyle->SetTitleSize(0.06, "x");
    babarStyle->SetLabelSize(0.05, "y");
    babarStyle->SetTitleSize(0.06, "y");
    babarStyle->SetLabelSize(0.05, "z");
    babarStyle->SetTitleSize(0.06, "z");

    // use bold lines and markers
    babarStyle->SetMarkerStyle(20);
    babarStyle->SetHistLineWidth(1);
    babarStyle->SetLineStyleString(2, "[12 12]"); // postscript dashes

    // get rid of X error bars and y error bar caps
    babarStyle->SetErrorX(0.001);

    // do not display any of the standard histogram decorations
    babarStyle->SetOptTitle(0);
    babarStyle->SetOptStat(0);
    babarStyle->SetOptFit(0);

    // put tick marks on top and RHS of plots
    babarStyle->SetPadTickX(1);
    babarStyle->SetPadTickY(1);

    gROOT->SetStyle("BABAR");

    ROOT::Math::MinimizerOptions::SetDefaultMinimizer("Minuit2");
    Int_t palette[6];
    palette[0] = 2;
    palette[1] = 3;
    palette[2] = 4;
    palette[3] = 6;
    palette[4] = 7;
    palette[5] = 1;
    gStyle->SetPalette(6, palette);
    std::vector<TString> root_File_Names;
    for (int i = 0; i < 18; i++)
    {
        root_File_Names.push_back(TString::Format("sm_%d.root", i));
    }
    const int numfiles = 18;

    auto C = new TCanvas();

    gStyle->SetOptTitle(kFALSE);
    gStyle->SetOptStat(0);
    gStyle->SetOptFit(0);

    Int_t  nbins = 30;
    auto h_0 = new TH1D("h_0", "Slope Fit Param Hists in Layer 0", nbins, 0.02, 0.2);
    auto h_1 = new TH1D("h_1", "Slope Fit Param Hists in Layer 1", nbins, 0.02, 0.2);
    auto h_2 = new TH1D("h_2", "Slope Fit Param Hists in Layer 2", nbins, 0.02, 0.2);
    auto h_3 = new TH1D("h_3", "Slope Fit Param Hists in Layer 3", nbins, 0.02, 0.2);
    auto h_4 = new TH1D("h_4", "Slope Fit Param Hists in Layer 4", nbins, 0.02, 0.2);
    auto h_5 = new TH1D("h_5", "Slope Fit Param Hists in Layer 5", nbins, 0.02, 0.2);

    Double_t para_a, para_b;
    Long64_t nentries;

    TBranch *para_aBranch = 0;
    TBranch *para_bBranch = 0;

    vector<Double_t>
        fit_a(540, 0.0),
        fit_b(540, 0.0);
    TTree *tree = 0;

    TFile *s[numfiles];
    for (Int_t i = 0; i < numfiles; i++)
    {
        s[i] = TFile::Open(root_File_Names[i], "READ");
        tree = (TTree *)s[i]->Get("fits/fit_params");
        tree->SetMakeClass(1);
        tree->SetBranchAddress("fit_a", &para_a, &para_aBranch);
        tree->SetBranchAddress("fit_b", &para_b, &para_bBranch);
        nentries = tree->GetEntries();

        // Then loop over all of them.
        for (Long64_t j = 0; j < nentries; j++)
        {
            tree->GetEntry(j);
            fit_a[30 * i + j] = para_a;
            fit_b[30 * i + j] = para_b;
        }
    }

    Int_t stacks[] = {0};

    for (Int_t sector = 0; sector < numfiles; sector++)
    {
        for (auto &&stack : stacks)
        {
            h_0->Fill(fit_b[30 * sector + stack * 6 + 0]);
            h_1->Fill(fit_b[30 * sector + stack * 6 + 1]);
            h_2->Fill(fit_b[30 * sector + stack * 6 + 2]);
            h_3->Fill(fit_b[30 * sector + stack * 6 + 3]);
            h_4->Fill(fit_b[30 * sector + stack * 6 + 4]);
            h_5->Fill(fit_b[30 * sector + stack * 6 + 5]);
        }
        
    }
    
    //Norm
    Double_t scale_0 = 1 / h_0->Integral();
    Double_t scale_1 = 1 / h_1->Integral();
    Double_t scale_2 = 1 / h_2->Integral();
    Double_t scale_3 = 1 / h_3->Integral();
    Double_t scale_4 = 1 / h_4->Integral();
    Double_t scale_5 = 1 / h_5->Integral();
    h_0->Scale(scale_0);
    h_1->Scale(scale_1);
    h_2->Scale(scale_2);
    h_3->Scale(scale_3);
    h_4->Scale(scale_4);
    h_5->Scale(scale_5);
    
    h_0->SetMarkerStyle(kFullCircle);
    h_1->SetMarkerStyle(kFullSquare);
    h_2->SetMarkerStyle(kFullTriangleUp);
    h_3->SetMarkerStyle(kFullTriangleDown);
    h_4->SetMarkerStyle(kOpenCircle);
    h_5->SetMarkerStyle(kFullFourTrianglesPlus);

    h_0->GetXaxis()->SetTitle("slope parameter [Hz/#mub]");
    h_0->GetYaxis()->SetTitle("nor. entries");
    // FIT
    Bool_t kfit = kTRUE;
    if (kfit)
    {
        TF1 *g0 = new TF1("g0", "gaus", 0.02, 0.2);
        TF1 *g1 = new TF1("g1", "gaus", 0.02, 0.2);
        TF1 *g2 = new TF1("g2", "gaus", 0.02, 0.2);
        TF1 *g3 = new TF1("g3", "gaus", 0.02, 0.2);
        TF1 *g4 = new TF1("g4", "gaus", 0.02, 0.2);
        TF1 *g5 = new TF1("g5", "gaus", 0.02, 0.2);
        h_0->Fit(g0, "R W");
        h_1->Fit(g1, "R W");
        h_2->Fit(g2, "R W");
        h_3->Fit(g3, "R W");
        h_4->Fit(g4, "R W");
        h_5->Fit(g5, "R W");
    }
    //
    h_0->GetFunction("g0")->SetLineColor(palette[0]);
    h_1->GetFunction("g1")->SetLineColor(palette[1]);
    h_2->GetFunction("g2")->SetLineColor(palette[2]);
    h_3->GetFunction("g3")->SetLineColor(palette[3]);
    h_4->GetFunction("g4")->SetLineColor(palette[4]);
    h_5->GetFunction("g5")->SetLineColor(palette[5]);

    h_0->GetYaxis()->SetRangeUser(0, 0.55);
    h_0->GetXaxis()->SetRangeUser(0.04, 0.12);


    h_0->Draw("9 PLC PMC");
    h_1->Draw("9 SAME PLC PMC");
    h_2->Draw("9 SAME PLC PMC");
    h_3->Draw("9 SAME PLC PMC");
    h_4->Draw("9 SAME PLC PMC");
    h_5->Draw("9 SAME PLC PMC");

    gPad->BuildLegend(0.75, 0.75, 0.9, 0.9);

    TLatex *tex = new TLatex();
    tex->SetNDC(kTRUE);
    tex->SetTextSize(0.02);
    tex->SetTextColor(kBlack);
    tex->DrawLatex(0.15, 0.88, "Gauss Fit Layer 0");
    char buffer_M[100],
        buffer_S[100];
    sprintf(buffer_M, "M_{Z} = %.3f #pm %.3f", g0->GetParameter(1), g0->GetParError(1));
    sprintf(buffer_S, "#sigma = %.3f #pm %.3f", g0->GetParameter(2), g0->GetParError(2));
    tex->DrawLatex(0.15, 0.85, buffer_M);
    tex->DrawLatex(0.15, 0.82, buffer_S);
    
    tex->DrawLatex(0.15, 0.78, "Gauss Fit Layer 1");
    sprintf(buffer_M, "M_{Z} = %.3f #pm %.3f", g1->GetParameter(1), g1->GetParError(1));
    sprintf(buffer_S, "#sigma = %.3f #pm %.3f", g1->GetParameter(2), g1->GetParError(2));
    tex->DrawLatex(0.15, 0.75, buffer_M);
    tex->DrawLatex(0.15, 0.72, buffer_S);
    
    tex->DrawLatex(0.15, 0.68, "Gauss Fit Layer 2");
    sprintf(buffer_M, "M_{Z} = %.3f #pm %.3f", g2->GetParameter(1), g2->GetParError(1));
    sprintf(buffer_S, "#sigma = %.3f #pm %.3f", g2->GetParameter(2), g2->GetParError(2));
    tex->DrawLatex(0.15, 0.65, buffer_M);
    tex->DrawLatex(0.15, 0.62, buffer_S);
    
    tex->DrawLatex(0.15, 0.58, "Gauss Fit Layer 3");
    sprintf(buffer_M, "M_{Z} = %.3f #pm %.3f", g3->GetParameter(1), g3->GetParError(1));
    sprintf(buffer_S, "#sigma = %.3f #pm %.3f", g3->GetParameter(2), g3->GetParError(2));
    tex->DrawLatex(0.15, 0.55, buffer_M);
    tex->DrawLatex(0.15, 0.52, buffer_S);
    
    tex->DrawLatex(0.15, 0.48, "Gauss Fit Layer 4");
    sprintf(buffer_M, "M_{Z} = %.3f #pm %.3f", g4->GetParameter(1), g4->GetParError(1));
    sprintf(buffer_S, "#sigma = %.3f #pm %.3f", g4->GetParameter(2), g4->GetParError(2));
    tex->DrawLatex(0.15, 0.45, buffer_M);
    tex->DrawLatex(0.15, 0.42, buffer_S);

    tex->DrawLatex(0.15, 0.38, "Gauss Fit Layer 5");
    sprintf(buffer_M, "M_{Z} = %.3f #pm %.3f", g5->GetParameter(1), g5->GetParError(1));
    sprintf(buffer_S, "#sigma = %.3f #pm %.3f", g5->GetParameter(2), g5->GetParError(2));
    tex->DrawLatex(0.15, 0.35, buffer_M);
    tex->DrawLatex(0.15, 0.32, buffer_S);   
}