{
    TTree tree_fit_a("fit_a", "Fit Param a");
    Double_t para_a, para_b;
    TBranch *para_aBranch = 0;
    TBranch *para_bBranch = 0;

    std::vector<TString> root_File_Names;
    for (int i = 0; i < 18; i++)
    {
        root_File_Names.push_back(TString::Format("sm_%d.root", i));
    }
    const int numfiles = 18;

    TFile *s[numfiles];
    Int_t i = 0;
    s[i] = TFile::Open(root_File_Names[i], "READ");
    auto tree = (TTree *)s[i]->Get("fits/fit_params");
    tree->SetMakeClass(1);
    tree->SetBranchAddress("fit_a", &para_a, &para_aBranch);
    tree->SetBranchAddress("fit_b", &para_b, &para_bBranch);
    nentries = tree->GetEntries();
    cout << nentries << endl;

    // Then loop over all of them.
    for (Long64_t j = 0; j < nentries; j++)
    {
        tree->GetEntry(j);
        cout << j << "\t" << para_a << "\t" << para_b << endl;
    }
}