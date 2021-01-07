Double_t Mean(const std::vector<Double_t> *v)
{
    Double_t sum = 0, n = 0;
    for (auto &&element : *v)
    {
        sum += element;
        n++;
    }
    return sum / n;
}

Double_t Mean_std(const std::vector<Double_t> *v, const Double_t mean)
{
    Double_t standardDeviation = 0;
    for (auto &&element : *v)
    {
        standardDeviation += TMath::Power(element - mean, 2);
    }
    return TMath::Sqrt(standardDeviation / ((Double_t)v->size() - 1));
}

void spreadOfHV()
{
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

    vector<Double_t> x, y;
    vector<string> drift;
    TLatex *t = new TLatex();
    t->SetTextFont(62);
    t->SetTextColor(1);
    t->SetTextSize(0.02);

    vector<Double_t> spread;
    Int_t nentries = 0;
    Double_t mean = 0;
    vector<Double_t> temp;

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
            char plate = channel_name[6];
            if (plate == 'A')
            {
                cout << file_index << endl;
                s = TFile::Open(Form(file_dir.c_str(), file_index), "READ");
                //auto tree = new TTree();
                tree = (TTree *)s->Get("Tree_TRD_HV");
                if (!tree || !s)
                {
                    continue;
                }
                tree->SetMakeClass(1);
                tree->SetBranchAddress("HV", &HV, &b_HV);
                tree->SetBranchAddress("fSec", &fSec, &b_time_fSec);
                tree->SetBranchAddress("fNanoSec", &fNanoSec, &b_time_fNanoSec);
                nentries = tree->GetEntries();
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
                //s->Close();
                tree->Delete();
                //tree = 0;
                 s = 0;
                fSec = 0;
                mean = accumulate(temp.begin(), temp.end(), 0.0) / temp.size();
                temp.clear();
                if (mean >= 200)
                {
                    spread.push_back(mean);
                }
            }
        }
    }
    Double_t mean_t = Mean(&spread);
    Double_t std_t = Mean_std(&spread, mean_t);
    cout << std_t << endl;
}