void working(Int_t sector_n)
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
    auto h = new TH2D("h", Form("Sector %d", sector_n), 5, 0, 5, 6, 0, 6);
    if (sector_n == 15)
    {
        h->GetXaxis()->SetTitle("Stack");
        h->GetYaxis()->SetTitle("Layer");
    }

    for (Int_t layer = 1; layer < 7; layer++)
    {
        h->GetYaxis()->SetBinLabel(layer, Form("%d", layer - 1));
    }

    for (Int_t stack = 1; stack < 6; stack++)
    {
        h->GetXaxis()->SetBinLabel(stack, Form("%d", stack - 1));
    }

    vector<Double_t> x, y;
    string drift[30];
    TLatex *t = new TLatex();
    t->SetTextFont(62);
    t->SetTextColor(1);
    t->SetTextSize(0.02);

    TCanvas c("c", "Canvas", 10, 10, 800, 600);
    Int_t counter = 0;
    c.SetGrid();
    std::ifstream file("/media/felix/flush/ALICE_TRD/create_chamber_map/outfile.txt");
    if (!file.is_open())
    {
        std::cout << "outfile not open" << std::endl;
        return;
    }
    while (file >> crate_name >> file_index >> install_year >> measurement_type >> channel_name)
    {
        if ((file_index == 567) || (file_index == 2159))
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
                Int_t nentries = tree->GetEntries();

                Double_t mean = 0;
                vector<Double_t> temp;
                // 2017
                for (j = 0; j < nentries; j++)
                {
                    tree->GetEntry(j);
                    if ((fSec > 1504605300) && (fSec < 1504607400))
                    {
                        temp.push_back(HV);
                    }
                    if (fSec > 1504607400)
                    {
                        break;
                    }
                }
                // 2018
                // for (j = 0; j < nentries; j++)
                // {
                //     tree->GetEntry(j);
                //     if ((fSec > 1535962800) && (fSec < 1535972400))
                //     {
                //         temp.push_back(HV);
                //     }
                //     if (fSec > 1535972400)
                //     {
                //         break;
                //     }
                // }
                tree = 0, s = 0;
                fSec = 0;
                mean = accumulate(temp.begin(), temp.end(), 0.0) / temp.size();
                cout << mean << endl;
                if (plate == 'A')
                {
                    working = 1;
                    if ((mean < 1450) && (mean > 200))
                    {
                        working = 0;
                    }
                    else if (mean <= 200)
                    {
                        working = -1;
                    }
                    h->SetBinContent(stack + 1, layer + 1, working);
                    counter++;
                }
                if (plate == 'D')
                {
                    drift[stack * 6 + layer] = "drift on";
                    if ((mean < 1900) && (mean > 200))
                    {
                        drift[stack * 6 + layer] = "drift red.";
                    }
                    else if (mean <= 200)
                    {
                        drift[stack * 6 + layer] = "drift off";
                    }
                    if (h->GetBinContent(stack + 1, layer + 1) == -1)
                    {
                        drift[stack * 6 + layer] = "drift off";
                    }
                    x.push_back(h->GetXaxis()->GetBinCenter(h->GetXaxis()->FindBin(stack + 1)) - 1.2);
                    y.push_back(h->GetYaxis()->GetBinCenter(h->GetYaxis()->FindBin(layer + 1)) - 1.1);
                    counter++;
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
    h->Draw("col");
    for (Int_t i = 0; i < 30; i++)
    {
        t->DrawText(x[i], y[i], drift[i].c_str());
    }

    file.close();
    c.SaveAs(Form("2017/sector_%d.pdf", sector_n));
}