#include <vector>
#include <cmath>

class noisy
{
private:
    double threshold;
    double lag;
    double influence;
    vector<double> array;
    double mean;
    double std;

public:
    noisy(double lag, double threshold, double influence);
    ~noisy();
    int update(double element);
};

noisy::noisy(double lag, double threshold, double influence)
{
    this->threshold = threshold;
    this->lag = lag;
    this->influence = influence;
}

noisy::~noisy()
{
}

int noisy::update(double element)
{
    if (array.size() >= lag)
    {
        array.erase(array.begin());
    }
    if (abs(element - mean) <= threshold * std)
    {
        array.push_back(element);
        double sum = std::accumulate(std::begin(array), std::end(array), 0.0);
        mean = sum / array.size();

        double accum = 0.0;
        std::for_each(std::begin(array), std::end(array), [&](const double d) {
            accum += (d - mean) * (d - mean);
        });
        std = sqrt(accum / (array.size() - 1));
        return 0;
    }
    else
    {
        array.push_back(influence * element + (1 - influence) * mean);
        double sum = std::accumulate(std::begin(array), std::end(array), 0.0);
        mean = sum / array.size();

        double accum = 0.0;
        std::for_each(std::begin(array), std::end(array), [&](const double d) {
            accum += (d - mean) * (d - mean);
        });
        std = sqrt(accum / (array.size() - 1));
        return 1;
    }
}

void detect()
{
    gROOT->SetStyle("Pub");
    noisy m(50, 3.7, 0.2);

    // Declaration of leaf types
    Double_t HV;
    //TTimeStamp      *time;
    Int_t fSec;
    Int_t fNanoSec;

    // List of branches
    TBranch *b_HV;            //!
    TBranch *b_time_fSec;     //!
    TBranch *b_time_fNanoSec; //!

    TFile *f1 = TFile::Open("/media/felix/flush/ALICE_TRD/hv_ALICE_Data/sorted_486.csv.root", "READ");
    TTree *tree1 = new TTree();
    tree1 = (TTree *)f1->Get("Tree_TRD_HV");
    tree1->SetMakeClass(1);
    tree1->SetBranchAddress("HV", &HV, &b_HV);
    tree1->SetBranchAddress("fSec", &fSec, &b_time_fSec);
    tree1->SetBranchAddress("fNanoSec", &fNanoSec, &b_time_fNanoSec);

    auto g = new TGraph();
    auto s = new TGraph();

    Long64_t nentries = tree1->GetEntries();
    //auto myfile = new TFile("output.root", "UPDATE");
    TTimeStamp *ttime = new TTimeStamp();
    Long64_t gentry = 0;

    int _temp;

    for (Long64_t jentry = 0; jentry < nentries; jentry++)
    {
        tree1->GetEntry(jentry);
        if (fSec < 1521057610)
        {
            continue;
        }
        if (fSec > 1521086341)
            break;
        ttime->SetSec(fSec);
        ttime->SetNanoSec(fNanoSec);
        _temp = m.update(HV);
        s->SetPoint(gentry, ttime->AsDouble(), _temp);
        g->SetPoint(gentry, ttime->AsDouble(), HV);
        gentry++;
    }

    g->GetXaxis()->SetTimeOffset(0, "gmt");
    g->GetXaxis()->SetTimeDisplay(1);
    g->GetXaxis()->SetLabelOffset(0.02);
    g->GetXaxis()->SetTimeFormat("#splitline{%Y}{#splitline{%d\/%m}{%H\:%M}}");
    g->GetXaxis()->SetTitle("");
    g->GetYaxis()->SetTitle("Current (#muA)");
    g->SetMarkerStyle(8);
    g->SetMarkerSize(0.5);
    g->SetMarkerColor(kRed);

    s->GetXaxis()->SetTimeOffset(0, "gmt");
    s->GetXaxis()->SetTimeDisplay(1);
    s->GetXaxis()->SetLabelOffset(0.02);
    s->GetXaxis()->SetTimeFormat("#splitline{%Y}{#splitline{%d\/%m}{%H\:%M}}");
    s->GetXaxis()->SetTitle("");
    s->GetYaxis()->SetTitle("Current (#muA)");
    s->SetMarkerStyle(8);
    s->SetMarkerSize(0.5);
    s->SetMarkerColor(kRed);

    TFile *f2 = TFile::Open("/media/felix/flush/ALICE_TRD/hv_ALICE_Data/sorted_2.csv.root", "READ");
    TTree *tree2 = new TTree();
    tree2 = (TTree *)f2->Get("Tree_TRD_HV");
    tree2->SetMakeClass(1);
    tree2->SetBranchAddress("HV", &HV, &b_HV);
    tree2->SetBranchAddress("fSec", &fSec, &b_time_fSec);
    tree2->SetBranchAddress("fNanoSec", &fNanoSec, &b_time_fNanoSec);
    noisy m2(50, 3.7, 0.2);
    auto g2 = new TGraph();
    auto s2 = new TGraph();
    s2->SetMarkerStyle(8);
    s2->SetMarkerSize(0.5);
    s2->SetMarkerColor(kBlue);
    g2->SetMarkerStyle(8);
    g2->SetMarkerSize(0.5);
    g2->SetMarkerColor(kBlue);

    nentries = tree2->GetEntries();
    gentry = 0;
    for (Long64_t jentry = 0; jentry < nentries; jentry++)
    {
        tree2->GetEntry(jentry);
        if (fSec < 1521057610)
        {
            continue;
        }
        if (fSec > 1521086341)
            break;
        ttime->SetSec(fSec);
        ttime->SetNanoSec(fNanoSec);
        _temp = m2.update(HV);
        s2->SetPoint(gentry, ttime->AsDouble(), _temp);
        g2->SetPoint(gentry, ttime->AsDouble(), HV);
        gentry++;
    }
    s->GetXaxis()->SetRangeUser(1521064800, 1521086341);
    g->GetXaxis()->SetRangeUser(1521064800, 1521086341);
    s2->GetXaxis()->SetRangeUser(1521064800, 1521086341);
    g2->GetXaxis()->SetRangeUser(1521064800, 1521086341);

    //g->Draw("AP");
    s->SetLineColor(kRed);
    s2->SetLineColor(kBlue);

    auto c = new TCanvas();
    c->Divide(1, 2);
    c->cd(1);
    g->Draw("AP");
    g2->Draw("SAME P");

    auto legend = new TLegend(0.7, 0.7, 0.8, 0.45);
    //legend->SetHeader("small offset in times","C"); // option "C" allows to center the header
    legend->AddEntry(g, "broken chamber", "p");
    legend->AddEntry(g2, "reference chamber", "p");
    legend->Draw();

    c->cd(2);
    s->Draw("APL");
    s2->Draw("SAME PL");
}
