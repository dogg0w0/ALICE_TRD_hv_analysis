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
    noisy m(50, 3.5, 0.1);

    // Declaration of leaf types
    Double_t HV;
    //TTimeStamp      *time;
    Int_t fSec;
    Int_t fNanoSec;

    // List of branches
    TBranch *b_HV;            //!
    TBranch *b_time_fSec;     //!
    TBranch *b_time_fNanoSec; //!

    TFile *f1 = TFile::Open("/home/doggo/Downloads/ALICE_TRD/hv_ALICE_Data/sorted_486.csv.root", "READ");
    TTree *tree1 = new TTree();
    tree1 = (TTree *)f1->Get("Tree_TRD_HV");
    tree1->SetMakeClass(1);
    tree1->SetBranchAddress("HV", &HV, &b_HV);
    tree1->SetBranchAddress("fSec", &fSec, &b_time_fSec);
    tree1->SetBranchAddress("fNanoSec", &fNanoSec, &b_time_fNanoSec);

    auto g = new TGraph();
    auto s = new TGraph();

    int i =0;
    TGraphTime *gt = new TGraphTime(7990, 1521057610, 0, 1521086341, 10);

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

        TMarker *m = new TMarker(ttime->AsDouble(), HV, 4);
        m->SetMarkerColor(2);
        gt->Add(m, i);
        if (gentry % 10 == 0)
        {
            i++;
        }
        gentry++;
    }
    s->GetXaxis()->SetRangeUser(1521064800, 1521086341);
    g->GetXaxis()->SetRangeUser(1521064800, 1521086341);

    g->GetXaxis()->SetTimeOffset(0, "gmt");
    g->GetXaxis()->SetTimeDisplay(1);
    g->GetXaxis()->SetLabelOffset(0.02);
    g->GetXaxis()->SetTimeFormat("#splitline{%Y}{#splitline{%d\/%m}{%H\:%M}}");
    g->GetXaxis()->SetTitle("");
    g->GetYaxis()->SetTitle("Current (#muA)");
    g->SetMarkerStyle(8);
    g->SetMarkerSize(0.5);
    g->SetMarkerColorAlpha(kRed, 0.35);

    s->GetXaxis()->SetTimeOffset(0, "gmt");
    s->GetXaxis()->SetTimeDisplay(1);
    s->GetXaxis()->SetLabelOffset(0.02);
    s->GetXaxis()->SetTimeFormat("#splitline{%Y}{#splitline{%d\/%m}{%H\:%M}}");
    s->GetXaxis()->SetTitle("");
    s->GetYaxis()->SetTitle("Current (#muA)");
    s->SetMarkerStyle(8);
    s->SetMarkerSize(0.5);
    s->SetMarkerColorAlpha(kRed, 0.35);

    //gt->SetTitle("Anim;time;Current (#muA)");
    //gt->Draw();
    //gt->SaveAnimatedGif("test.gif");

     auto c = new TCanvas();
     c->Divide(1,2);
     c->cd(1);
     g->Draw("AP");
     c->cd(2);
     s->Draw("APL");
}
