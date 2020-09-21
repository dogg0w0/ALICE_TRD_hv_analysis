{
    TFile *file = new TFile("testfile.root", "UPDATE");
    //TH1D* hist = new TH1D("hist", "TITLE", 10, 0, 10);
    gDirectory->pwd();

    //TDirectory *cdtof = file->mkdir("tof");
    TDirectory *ctof = (gDirectory->FindObjectAny("tof")) ? (TDirectory *)gDirectory->FindObjectAny("tof") : file->mkdir("tof");
    ctof->cd();
    gDirectory->pwd();

    file->Write();
    delete file;
    new TBrowser;
}