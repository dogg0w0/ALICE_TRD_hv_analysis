void working()
{
    gROOT->SetStyle("Pub");
    gStyle->SetOptTitle(kTRUE);
    gROOT->ForceStyle();

    Int_t colors[3];
    colors[0] = kRed;
    colors[1] = kWhite;
    colors[2] = kGreen;
    gStyle->SetPalette(3, colors);

    std::string file_dir = "/media/felix/flush/ALICE_TRD/hv_ALICE_Data/sorted_%d.csv.root";
    std::string line, channel_name, crate_name, measurement_type;
    Int_t file_index, install_year;

    TFile *s = 0;
    TTree *tree = 0;
    Double_t HV;
    //TTimeStamp      *time;
    Int_t fSec;
    Int_t fNanoSec;

    // List of branches
    TBranch *b_HV;            //!
    TBranch *b_time_fSec;     //!
    TBranch *b_time_fNanoSec; //!
    Int_t working = 0;
    Int_t j = 0;
    TH2D *h[18];
    for (auto &&hs : h)
    {
        hs = new TH2D("h", "", 5, 0, 5, 6, 0, 6);
        hs->SetTitle(Form("Sector %d", j));
        for (Int_t layer = 1; layer < 7; layer++)
        {
            hs->GetYaxis()->SetBinLabel(layer, Form("%d", layer - 1));
        }

        for (Int_t stack = 1; stack < 6; stack++)
        {
            hs->GetXaxis()->SetBinLabel(stack, Form("%d", stack - 1));
        }
        j++;
    }
    j = 0;
    vector<Double_t> x, y;
    vector<string> drift;
    TLatex *t = new TLatex();
    t->SetTextFont(62);
    t->SetTextColor(36);
    t->SetTextSize(0.05);

    auto c = new TCanvas("c", "Canvas", 10, 10, 1000, 6000);
    c->Divide(3, 6);
    for (Int_t sector_n = 0; sector_n < 1; sector_n++)
    {
        Int_t counter = 0;
        c->cd(sector_n + 1);
        c->GetPad(sector_n)->SetGrid();
        std::ifstream file("/media/felix/flush/ALICE_TRD/create_chamber_map/outfile.txt");
        if (!file.is_open())
        {
            std::cout << "outfile not open" << std::endl;
            return;
        }
        while (file >> crate_name >> file_index >> install_year >> measurement_type >> channel_name)
        {
            if (file_index == 567)
            {
                continue;
            }
            
            if (measurement_type[0] == 'V')
            {
                Int_t sector = std::stoi(channel_name.substr(0, 2));
                Int_t stack = std::stoi(channel_name.substr(3, 1));
                Int_t layer = std::stoi(channel_name.substr(5, 1));
                char plate = channel_name[6];
                if (sector == sector_n)
                {
                    cout << file_index << endl;
                    s = TFile::Open(Form(file_dir.c_str(), file_index), "READ");
                    tree = (TTree *)s->Get("Tree_TRD_HV");
                    if (!tree || !s)
                    {
                        continue;
                    }
                    tree->SetMakeClass(1);
                    tree->SetBranchAddress("HV", &HV, &b_HV);
                    tree->SetBranchAddress("fSec", &fSec, &b_time_fSec);
                    tree->SetBranchAddress("fNanoSec", &fNanoSec, &b_time_fNanoSec);
                    //nentries = tree->GetEntries();
                    j = 0;
                    
                    while (fSec < 1536352131)
                    {
                        tree->GetEntry(j);
                        j++;
                    }
                    tree = 0, s = 0;
                    fSec = 0;
                    cout << HV << endl;
                    if (plate == 'A')
                    {
                        cout << "looking closer at anode " << file_index << endl;
                        working = 1;
                        if ((HV < 1450) && (HV > 20))
                        {
                            working = 0;
                        }
                        else if (HV <= 20)
                        {
                            working = -1;
                        }
                        h[sector_n]->SetBinContent(stack + 1, layer + 1, working);
                        counter++;
                        cout << "stop looking at anode" << endl;
                    }
                    if (plate == 'D')
                    {
                        cout << "looking closer at Drift " << file_index << endl;
                        drift.push_back("1");
                        if ((HV < 1900) && (HV > 20))
                        {
                            drift.push_back("0");
                        }
                        else if (HV <= 20)
                        {
                            drift.push_back("-1");
                        }
                        x.push_back(h[sector_n]->GetXaxis()->GetBinCenter(h[sector_n]->GetXaxis()->FindBin(stack + 1)) - 1);
                        y.push_back(h[sector_n]->GetYaxis()->GetBinCenter(h[sector_n]->GetYaxis()->FindBin(layer + 1)) - 1);
                        counter++;
                        cout << "stop looking at drift" << endl;
                    }
                }
            }
            if (counter == 120)
            {
                cout << "breaking free" << endl;
                break;
            }
        }
        cout << "drawing hist" << endl;
        h[sector_n]->Draw("col");
        for (Int_t i = 0; i < 30; i++)
        {
            t->DrawText(x[i], y[i], drift[i].c_str());
        }

        file.close();
    }
    c->Draw();
}