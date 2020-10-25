{
    TStyle *babarStyle = new TStyle("BABAR", "BaBar approved plots style");

    // use plain black on white colors
    babarStyle->SetFrameBorderMode(0);
    babarStyle->SetCanvasBorderMode(0);
    babarStyle->SetPadBorderMode(0);
    babarStyle->SetPadColor(0);
    babarStyle->SetCanvasColor(0);
    babarStyle->SetStatColor(0);
    //babarStyle->SetFillColor(0);

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
    babarStyle->SetOptStat(0);
    babarStyle->SetOptFit(0);

    // put tick marks on top and RHS of plots
    babarStyle->SetPadTickX(1);
    babarStyle->SetPadTickY(1);

    gROOT->SetStyle("BABAR");


    Double_t run, para_a, para_b;
    Float_t gain;

    TBranch *gainBranch = 0;
    TBranch *runBranch = 0;
    TBranch *para_aBranch = 0;
    TBranch *para_bBranch = 0;

    vector<Double_t> meangain(540, 0.0),
        gain_v(1620, 0.0),
        fit_a(540, 0.0),
        fit_b(540, 0.0);

    // Open the ROOT file.
    TFile *f = TFile::Open("trendingOCDB_2017.root", "READ");
    if (f == 0)
    {

        // If we cannot open the ROOT file, print an error message and return immediately.
        printf("Error: cannot open file");
        return;
    }

    // Get a pointer to the tree.
    TTree *tree = (TTree *)f->Get("Results");

    // Use SetBranchAddress() with simple types (e.g. double, int) instead of objects (e.g. std::vector<Particle>).
    tree->SetMakeClass(1);

    // Connect the branch "fEventSize" with the variable eventSize that we want to contain the data.
    // While we are at it, ask the tree to save the branch in eventSizeBranch.
    tree->SetBranchAddress("gain", &gain, &gainBranch);
    tree->SetBranchAddress("run", &run, &runBranch);
    // First, get the total number of entries.
    Long64_t nentries = tree->GetEntries();

    // Then loop over all of them.
    for (Long64_t i = 0; i < nentries; i++)
    {
        tree->GetEntry(i);
        meangain[i % 540] += gain;
        gain_v[i] = gain;
    }
    for (Int_t i = 0; i < 540; i++)
    {
        meangain[i] /= 3;
    }

    std::vector<TString> root_File_Names;
    for (int i = 0; i < 18; i++)
    {
        root_File_Names.push_back(TString::Format("sm_%d.root", i));
    }
    const int numfiles = 18;

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

    auto hist = new TH2D("hist", "Gain vs. Slope", 50, 0.2, 0.7, 30, 0.04, 0.2);
    for (Int_t i = 0; i < 540; i++)
    {
        //hist->Fill(meangain[i], fit_b[i]);
        hist->Fill(gain_v[i], fit_b[i]);
        hist->Fill(gain_v[i + 540], fit_b[i]);
        hist->Fill(gain_v[i + 880], fit_b[i]);
    }
    hist->GetXaxis()->SetTitle("Gain");
    hist->GetYaxis()->SetTitle("slope parameter (A/(Hz/#mub))");

    TH1 *hpro1 = hist->ProjectionY("", 1, 25 , "");
    hpro1->SetTitle("Cut [gain<0.45]");
    hpro1->SetName("hpro1");

    TH1 *hpro2 = hist->ProjectionY("", 32, 50, "");
    hpro2->SetTitle("Cut [gain>0.52]");
    hpro2->SetName("hpro2");

    auto c = new TCanvas("c", "Gain vs Slope", 10, 10, 1000, 1000);
    c->Divide(1,2);
    c->GetPad(2)->Divide(2, 1);
    c->cd(1);
    hist->DrawNormalized("colz",1);
    c->GetPad(2)->cd(1);
    hpro1->Draw("");
    c->GetPad(2)->cd(2);
    hpro2->Draw("");
    c->Draw();
}