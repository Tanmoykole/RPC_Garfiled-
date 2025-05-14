void plotTimingResolution3D() {
    gStyle->SetPalette(kRainBow);
    gStyle->SetNumberContours(50);

    TFile* file = new TFile("updated_pulse_height.root", "READ");

    const int nBins = 60;
    double xMin = 0.0, xMax = 5.0;
    double binWidth = (xMax - xMin) / nBins;

    std::vector<double> dGasVals;
    std::vector<std::vector<double>> histData;
    std::vector<double> avgTiming;

    TIter next(file->GetListOfKeys());
    TKey* key;
    while ((key = (TKey*)next())) {
        TString name = key->GetName();
        if (name.BeginsWith("pulseTree_dGas_")) {
            TString suffix = name;
            suffix.ReplaceAll("pulseTree_dGas_", "");
            double dGas = suffix.Atof();

            TTree* tree = (TTree*)file->Get(name);
            double timingResolution;
            tree->SetBranchAddress("timingResolution", &timingResolution);

            std::vector<double> counts(nBins, 0);
            Long64_t nEntries = tree->GetEntries();

            double sum = 0;
            int valid = 0;
            for (Long64_t i = 0; i < nEntries; ++i) {
                tree->GetEntry(i);
                if (timingResolution >= xMin && timingResolution < xMax) {
                    int bin = (timingResolution - xMin) / binWidth;
                    counts[bin]++;
                    sum += timingResolution;
                    valid++;
                }
            }

            if (valid > 0)
                avgTiming.push_back(sum / valid);
            else
                avgTiming.push_back(0);

            dGasVals.push_back(dGas);
            histData.push_back(counts);
        }
    }

    // Sort data by dGas
    int n = dGasVals.size();
    std::vector<int> indices(n);
    std::iota(indices.begin(), indices.end(), 0);
    std::sort(indices.begin(), indices.end(), [&](int i, int j) {
        return dGasVals[i] < dGasVals[j];
    });

    TGraph2D* g2d = new TGraph2D();
    int point = 0;

    for (int k = 0; k < n; ++k) {
        int i = indices[k];
        double dGas = dGasVals[i];
        for (int j = 0; j < nBins; ++j) {
            double timing = xMin + (j + 0.5) * binWidth;
            double count = histData[i][j];
            g2d->SetPoint(point++, timing, count, dGas);
        }
    }

    // Draw
    TCanvas* c1 = new TCanvas("c1", "3D Timing Resolution Distribution", 1000, 800);
    gPad->SetRightMargin(0.15);

    g2d->SetTitle("Timing Resolution Distribution;Timing Resolution (ns);Counts;dGas Thickness (mm)");
    g2d->Draw("surf1");

    // Save canvas
    c1->SaveAs("timing_resolution_surface_corrected.png");

    // Save graphs
    TFile* out = new TFile("timing_distribution_corrected.root", "RECREATE");
    g2d->Write("timingResolutionSurface");
    out->Close();
}

