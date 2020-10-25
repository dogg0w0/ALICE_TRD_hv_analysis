{
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
    babarStyle->SetHistLineWidth(1.85);
    babarStyle->SetLineStyleString(2, "[12 12]"); // postscript dashes

    // get rid of X error bars and y error bar caps
    babarStyle->SetErrorX(0.001);

    // do not display any of the standard histogram decorations
    babarStyle->SetOptTitle(0);
    babarStyle->SetOptStat(111111);
    babarStyle->SetOptFit(0);

    // put tick marks on top and RHS of plots
    babarStyle->SetPadTickX(1);
    babarStyle->SetPadTickY(1);

    gROOT->SetStyle("BABAR");
    ROOT::Math::MinimizerOptions::SetDefaultMinimizer("Minuit2");
    gStyle->SetOptFit(0);
    gStyle->SetOptStat(0);
    std::vector<TString> root_File_Names;
    for (int i = 0; i < 18; i++)
    {
        root_File_Names.push_back(TString::Format("sm_%d.root", i));
    }
    const int numfiles = 18;

    auto c0 = new TCanvas("c0", "Slope Distribution", 10, 10, 1800, 1600);
    auto hist = new TH1D("hist", "Add of all Slope Hists", 30, 0, 0.2);

    TFile *s[numfiles];
    for (Int_t i = 0; i < numfiles; i++)
    {
        s[i] = TFile::Open(root_File_Names[i], "READ");

        auto h1 = (TH1D *)s[i]->Get("fits/h_b");
        hist->Add(h1);
    }

    Double_t par[6];
    TF1 *g1 = new TF1("g1", "gaus", 0.0, 0.2);
    hist->Fit(g1, "R");


    hist->GetXaxis()->SetTitle("Slope Parameter");
    hist->GetYaxis()->SetTitle("entries");
    hist->Draw("");

    // Fit results
    TLatex *tex = new TLatex();
    tex->SetNDC(kTRUE);
    tex->SetTextSize(0.035);
    tex->SetTextColor(kBlack);
    tex->DrawLatex(0.15, 0.85, "Gauss Fit");
    char buffer_M[100],
        buffer_S[100],
        buffer_Chi[100];
    sprintf(buffer_M, "#mu = %.3f #pm %.3f", g1->GetParameter(1), g1->GetParError(1));
    sprintf(buffer_S, "#sigma = %.3f #pm %.3f", g1->GetParameter(2), g1->GetParError(2));
    sprintf(buffer_Chi, "#chi^{2}_{red} = %.1f", g1->GetChisquare() / g1->GetNDF());
    tex->DrawLatex(0.15, 0.80, buffer_M);
    tex->DrawLatex(0.15, 0.75, buffer_S);
    tex->DrawLatex(0.15, 0.70, buffer_Chi);
}
