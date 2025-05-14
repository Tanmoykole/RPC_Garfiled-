void plot_timing_resolution_dGas_0p1() {
    // Load the file and TTree
    TFile *file = TFile::Open("updated_pulse_height .root"); // fixed extra space in filename
    if (!file || file->IsZombie()) return;

    TTree *tree = (TTree*)file->Get("pulseTree_dGas_0.1");
    if (!tree) return;

    // Create histogram for timing resolution
    TH1D *hTiming = new TH1D("hTiming", "Timing Resolution for dGas = 0.1;Timing Resolution [ns];Entries", 100, 0.135, 0.17);
    tree->Draw("timingResolution >> hTiming");

    // Define Gaussian function across the whole histogram range
    TF1 *gausFit = new TF1("gausFit", "gaus", 0.135, 0.17);
    hTiming->Fit(gausFit, "R"); // R = fit in range only

    // Draw with styling
    hTiming->SetLineColor(kBlue);
    hTiming->SetLineWidth(2);
    gausFit->SetLineColor(kRed);
    gausFit->SetLineWidth(2);

    // Display the sigma value
    double sigma = gausFit->GetParameter(2);
    double sigmaErr = gausFit->GetParError(2);
    std::cout << "Gaussian Fit σ = " << sigma << " ± " << sigmaErr << " ns" << std::endl;
}

