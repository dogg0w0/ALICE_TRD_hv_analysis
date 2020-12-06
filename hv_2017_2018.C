#include <fstream>
#include <iostream>
#include <TROOT.h>
#include <TMath.h>
#include <TChain.h>
#include <TCanvas.h>
#include <TGraph.h>
#include <TFile.h>
#include <TAxis.h>
#include <iostream>
#include <vector>
#include <TH1.h>
#include <TTimeStamp.h>
#include <TF1.h>
#include <TH2D.h>
using namespace std;

void hv_2017_2018()
{
    gROOT->SetStyle("Pub");
    gROOT->ForceStyle();
    TH1::SetDefaultSumw2(kTRUE);

    vector<double> v_2017(540, 0.0), v_2018(540, 0.0);
    vector<bool> v_b_2017(540, 0), v_b_2018(540, 0);
    TBranch *b_HV;            //!
    TBranch *b_time_fSec;     //!
    TBranch *b_time_fNanoSec; //!

    Long64_t nentries;
    TTimeStamp ttime(0, 0);

    Double_t HV, mean_hv, mean_hv_2;
    Int_t fSec;
    Int_t fNanoSec;
    TTree *tree = 0;
    TFile *s = 0;
    Int_t start_time = 1504576500;
    Int_t end_time = 1504626600;
    std::string line, channel_name, crate_name, measurement_type;
    Int_t file_index, install_year;

    std::ifstream file("/home/doggo/Downloads/ALICE_TRD/create_chamber_map/outfile.txt");
    if (!file.is_open())
    {
        std::cout << "outfile not open" << std::endl;
        return;
    }

    while (file >> crate_name >> file_index >> install_year >> measurement_type >> channel_name)
    {
        if (measurement_type[0] == 'V')
        {
            if (channel_name[6] == 'A')
            {
                Int_t sector = std::stoi(channel_name.substr(0, 2));
                Int_t stack = std::stoi(channel_name.substr(3, 1));
                Int_t layer = std::stoi(channel_name.substr(5, 1));

                s = TFile::Open(Form("/home/doggo/Downloads/ALICE_TRD/hv_ALICE_Data/sorted_%d.csv.root", file_index), "READ");
                tree = (TTree *)s->Get("Tree_TRD_HV");
                tree->SetMakeClass(1);
                tree->SetBranchAddress("HV", &HV, &b_HV);
                tree->SetBranchAddress("fSec", &fSec, &b_time_fSec);
                tree->SetBranchAddress("fNanoSec", &fNanoSec, &b_time_fNanoSec);

                nentries = tree->GetEntriesFast();
                auto g = new TGraph();
                Long64_t gentry = 0;
                for (Long64_t jentry = 0; jentry < nentries; jentry++)
                {
                    tree->GetEntry(jentry);
                    if (HV < 0)
                    {
                        continue;
                    }
                    if (fSec < start_time)
                    {
                        continue;
                    }
                    if (fSec > end_time)
                    {
                        break;
                    }

                    ttime.SetSec(fSec);
                    ttime.SetNanoSec(fNanoSec);
                    g->SetPoint(gentry, ttime.AsDouble(), HV);
                    gentry++;
                }
                g->Fit("pol0", "Q");
                mean_hv = g->GetFunction("pol0")->GetParameter(0);
                v_b_2017[sector * 30 + stack * 6 + layer] = (mean_hv > 1200) ? 1 : 0;
                v_2017[sector * 30 + stack * 6 + layer] = mean_hv;
                s->Close();

                // 2018

                s = TFile::Open(Form("/home/doggo/Downloads/ALICE_TRD/create_csv/output/date_0/sorted_%d.csv.root", file_index + 1), "READ");
                tree = (TTree *)s->Get("TOF_T0_Tree");
                tree->SetMakeClass(1);
                tree->SetBranchAddress("HV", &HV, &b_HV);
                tree->SetBranchAddress("fSec", &fSec, &b_time_fSec);
                tree->SetBranchAddress("fNanoSec", &fNanoSec, &b_time_fNanoSec);
                nentries = tree->GetEntriesFast();
                auto g2 = new TGraph();
                gentry = 0;
                for (Long64_t jentry = 0; jentry < nentries; jentry++)
                {
                    tree->GetEntry(jentry);
                    if (HV < 0)
                    {
                        continue;
                    }
                    ttime.SetSec(fSec);
                    ttime.SetNanoSec(fNanoSec);
                    g2->SetPoint(gentry, ttime.AsDouble(), HV);
                    gentry++;
                }
                g2->Fit("pol0", "Q");
                mean_hv_2 = g2->GetFunction("pol0")->GetParameter(0);
                v_b_2018[sector * 30 + stack * 6 + layer] = (mean_hv_2 > 1200) ? 1 : 0;
                v_2018[sector * 30 + stack * 6 + layer] = mean_hv_2;
                s->Close();

                cout << "Sector: " << sector << "\tStack: " << stack << "\tLayer: " << layer << "\tHV 2017: " << mean_hv << std::setw(20) << "HV 2018: " << mean_hv_2 << endl;

                if (g)
                {
                    delete g;
                }
                if (g2)
                    delete g2;
            }
        }
    }

    auto c = new TCanvas("c", "compare", 10, 10, 1600, 900);
    c->SetRightMargin(0.2);
    c->SetGrid();
    auto hist = new TH2D("hist", "HV 2017 vs. 2018;stack * 6 + layer;sector;percentage", 30, 0, 30, 18, 0, 18);
    auto hist_err = new TH2D("hist_err", "HV 2017 vs. 2018;stack * 6 + layer;sector;percentage", 30, 0, 30, 18, 0, 18);
    hist->GetXaxis()->SetNdivisions(8, 0, 0, kTRUE);
    hist->GetYaxis()->SetNdivisions(20, 0, 0, kTRUE);
    for (Int_t sector = 0; sector < 18; sector++)
    {
        hist->GetYaxis()->SetBinLabel(sector + 1, Form("%d", sector));
    }
    for (Int_t stack = 0; stack < 30; stack++)
    {
        hist->GetXaxis()->SetBinLabel(stack + 1, Form("%d", stack));
    }

    for (Int_t sector = 0; sector < 18; sector++)
    {
        for (Int_t stack = 0; stack < 5; stack++)
        {
            for (Int_t layer = 0; layer < 6; layer++)
            {
                if (v_b_2017[sector * 30 + stack * 6 + layer] && v_b_2018[sector * 30 + stack * 6 + layer])
                {
                    hist->SetBinContent(stack * 6 + layer + 1, sector + 1, v_2018[sector * 30 + stack * 6 + layer] / v_2017[sector * 30 + stack * 6 + layer]);
                }
                else
                {
                    hist_err->SetBinContent(stack * 6 + layer + 1, sector + 1, 1);
                }
            }
        }
    }
    hist->Draw("colz");
    //static Int_t colors[1] = {2};
    //gStyle->SetPalette(1, colors);
    //hist_err->Draw("colz same");
    c->Draw();
}

#ifndef __CLANG__
int main(int argc, char const *argv[])
{
    hv_2017_2018();
    return 0;
}
#endif
