{
    TTree tree_fit_a("fit_a", "Fit Param a");
    Double_t fit_a, fit_b;
    tree_fit_a.Branch("fit_a", &fit_a, "fit_a/D");
    tree_fit_a.Branch("fit_b", &fit_b, "fit_b/D");
}