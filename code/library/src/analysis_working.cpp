#include "analysis_working.hpp"

analysis_working::analysis_working(/* args */)
{
}

analysis_working::~analysis_working()
{
}

void analysis_working::Update(Int_t stack, Int_t layer, Bool_t b)
{
    working_map[stack][layer] = b;
}

void analysis_working::Write(Int_t sector)
{
    auto out = new TFile(Form("sm_%d.root", sector), "UPDATE");
    // Write Tree for correlation with gain
    TTree tree("working_chambers", Form("Working Chambers for Sector %d", sector));
    Bool_t working;
    tree.Branch("working", &working, "working/B");
    for (Int_t stack = 0; stack < 5; stack++)
    {
        for (Int_t layer = 0; layer < 6; layer++)
        {
            working = working_map[stack][layer];
            tree.Fill();
        }
    }
    tree.Write();
}